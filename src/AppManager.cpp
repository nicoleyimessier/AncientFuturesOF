#include "AppManager.h"

using namespace AncientFutures;

AppManager::AppManager()
{
}

AppManager::~AppManager()
{
}

void AppManager::setup()
{
    // set up app settings

    //! setup recorder
    recorder.setup( ofToDataPath( "recordings" ) );
    recorder.setApiKey( configs().one().getAPIKey() );

    //! setup Arduino
    if( configs().one().getUseArduino() ) {
        arduino.setup( configs().one().getArduinoPort() );
        arduino.sendStopMsg();
    }

    pMan.setup();


    if( configs().one().getUseOSC() ) {
        // setup osc manager
        oscMan.setup( "127.0.0.1", "app", configs().one().getTxPort(), configs().one().getRxPort() );
    }

    ofAddListener( ofEvents().keyPressed, this, &AppManager::onKeyPressed );
}

void AppManager::update( float dt )
{

    if( configs().one().getUseArduino() ) {
        TS_START( "arduino" );
        arduino.update();
        TS_STOP( "arduino" );
    }

    TS_START( "recorder" );
    recorder.update();
    TS_STOP( "recorder" );

    TS_START( "pMan" );
    pMan.update( dt );
    TS_STOP( "pMan" );


    if( configs().one().getUseOSC() ) {
        oscMan.update( dt );
    }

    if( configs().one().getTesting() ) {
        float elapsed = ofGetElapsedTimef() - startTime;
        if( mAppState == AppStates::IDLE && elapsed > 5.0f )
            setAppState( AppStates::COUNTDOWN );
    }

    switch( mAppState ) {
    case AppStates::IDLE:
        break;
    case AppStates::COUNTDOWN: {

        if( pMan.getDifference() > 3.0f )
            setAppState( AppStates::RECORDING );

        break;
    }
    case AppStates::RECORDING: {

        if( pMan.getTimer() > 10.0f )
            setAppState( AppStates::PROCESSING );

        // if we are recording, send volume data to the arduino
        arduino.sendVolumeData( recorder.getMappedVolume() );


        break;
    }
    case AppStates::PROCESSING: {

        if( recorder.getState() == Recorder::AudioRecordingStates::STOP ) {
            recorder.setTranslation( oscMan.getTranscription() );
            recorder.setAudioState( Recorder::AudioRecordingStates::SPEECH_TO_TEXT );
        }
        else if( mAppState == AppStates::PROCESSING && recorder.getIsDoneProcessing() ) {
            setAppState( AppStates::ANIMATING );
        }

        break;
    }
    case AppStates::ANIMATING: {
        float elapsedTime = ofGetElapsedTimef() - startAnimationTime;

        if( elapsedTime > animationTime )
            setAppState( AppStates::STOPPING );
        break;
    }
    case AppStates::STOPPING: {

        float elapsed = ofGetElapsedTimef() - startTyTimer;
        if( elapsed > tyTimeDur )
            setAppState( AppStates::IDLE );

        break;
    }
    default:
        break;
    }
}

void AppManager::draw()
{
    TS_START( "pMan draw" );
    pMan.draw();
    TS_STOP( "pMan draw" );

    TS_START( "recorder draw" );
    recorder.drawAudio( 1 );
    TS_STOP( "recorder draw" );

    if( configs().one().mAppDebug ) {

        if( configs().one().getUseArduino() )
            arduino.drawDebug();

        recorder.drawDebug();
    }
}

// ---- APP STAETS ---- //
void AppManager::setAppState( AppStates state )
{
    mAppState = state;

    ofLogNotice( "AppManager::setAppState" ) << "Changing state to " << getAppStateString();

    startTime = ofGetElapsedTimef();

    switch( mAppState ) {
    case AppStates::IDLE: {
        oscMan.clearTxt();
        pMan.setPage( Pages::INTRO );
        break;
    }
    case AppStates::COUNTDOWN: {
        pMan.setPage( Pages::COUNTDOWN );

        if( configs().one().getUseArduino() )
            arduino.sendRecording();
        break;
    }
    case AppStates::RECORDING: {
        pMan.setPage( Pages::LISTENING );
        recorder.start();
        oscMan.sendString( "record" );
        break;
    }
    case AppStates::PROCESSING: {
        pMan.setPage( Pages::PROCESSING );
        recorder.stop();
        oscMan.sendString( "stopRecording" );

        if( configs().one().getUseArduino() )
            arduino.sendAnalyzing();
        break;
    }
    case AppStates::ANIMATING: {
        pMan.setPage( Pages::ANIMATING );

        float pos = 0.0f;
        float neg = 0.0f;
        float neu = 0.0f;

        // open json file
        if( ofFile::doesFileExist( recorder.getSentimentPath() ) ) {
            ofLogNotice() << "Found  sentiment analysis " << recorder.getSentimentPath() << ", loading";

            try {
                ofJson json = ofLoadJson( recorder.getSentimentPath() );
                // ofLogNotice() << "JSON DUMP: " << json.dump();

                if( !json["neg"].is_null() )
                    neg = json["neg"];

                if( !json["pos"].is_null() )
                    pos = json["pos"];

                if( !json["neu"].is_null() )
                    neu = json["neu"];
            }
            catch( exception &exc ) {
                ofLogError() << "Unable to load json file";
            }
        }
        else {
            ofLogError() << "Sentiment file " << recorder.getSentimentPath() << " does not exists!";
        }

        if( configs().one().getUseArduino() )
            arduino.sendSentimentMsg( pos, neg, neu );

        startAnimationTime = ofGetElapsedTimef();
        break;
    }
    case AppStates::STOPPING: {
        if( configs().one().getUseArduino() )
            arduino.sendStopMsg();

        pMan.setPage( Pages::CLOSE_OUT );
        startTyTimer = ofGetElapsedTimef();
        break;
    }
    default:
        break;
    }
}


void AppManager::nextState()
{
    int       count = static_cast<int>( mAppState );
    const int max = static_cast<int>( AppStates::NUM_STATES );

    if( count < ( max - 1 ) ) {
        count++;
    }
    else {
        count = 0;
    }

    setAppState( static_cast<AppStates>( count ) );
}

string AppManager::getAppStateString()
{
    switch( mAppState ) {
    case AppStates::IDLE:
        return "IDLE";
    case AppStates::COUNTDOWN:
        return "COUNTDOWN";
    case AppStates::RECORDING:
        return "RECORDING";
    case AppStates::PROCESSING:
        return "PROCESSING";
    case AppStates::ANIMATING:
        return "ANIMATING";
    case AppStates::STOPPING:
        return "STOPPING";
    default:
        break;
    }
}

void AppManager::onKeyPressed( ofKeyEventArgs &e )
{
    switch( e.key ) {
    case '1':
        arduino.sendRecording();
        break;
    case '2':
        arduino.sendAnalyzing();
        break;
    case '3':
        arduino.sendSentimentMsg( 1.0f, 0.0f, 0.0f );
        break;
    case '4':
        arduino.sendSentimentMsg( 0.0f, 1.0f, 0.0f );
        break;
    case '5':
        arduino.sendSentimentMsg( 0.0f, 0.0f, 1.0f );
        break;
    case '6':
        arduino.sendStopMsg();
        break;
    case '7':
        arduino.sendVolumeData( 1 );
        break;
    case '8':
        setAppState( AppStates::STOPPING );
        break;
    case 'a':
        oscMan.sendString( "record" );
        break;
    case 'b':
        oscMan.sendString( "stopRecording" );
        break;
    case 'c':
        break;
    default:
        break;
    }
}