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
    // set up ambient recordings
    string path = configs().one().getRootPath() + ofToDataPath( "recordings" );
    if( ofDirectory::doesDirectoryExist( path ) ) {

        // years
        ofDirectory recordingPath( path );
        recordingPath.listDir();
        for( int i = 0; i < recordingPath.size(); i++ ) {
            if( ofDirectory::doesDirectoryExist( recordingPath.getPath( i ) ) ) {
                // year
                ofDirectory dirYear( recordingPath.getPath( i ) );
                dirYear.listDir();


                for( int j = 0; j < dirYear.size(); j++ ) {
                    if( ofDirectory::doesDirectoryExist( dirYear.getPath( j ) ) ) {
                        // month
                        ofDirectory dirMonth( dirYear.getPath( j ) );
                        dirMonth.listDir();

                        for( int k = 0; k < dirMonth.size(); k++ ) {
                            if( ofDirectory::doesDirectoryExist( dirMonth.getPath( k ) ) ) {
                                // day
                                ofDirectory dirDay( dirMonth.getPath( k ) );
                                dirDay.listDir();

                                for( int l = 0; l < dirDay.size(); l++ ) {
                                    if( ofDirectory::doesDirectoryExist( dirDay.getPath( l ) ) ) {
                                        // individual
                                        ofDirectory individual( dirDay.getPath( l ) );
                                        individual.allowExt( "json" );
                                        individual.listDir();

                                        for( int m = 0; m < individual.size(); m++ ) {

                                            if( ofIsStringInString( individual.getPath( m ), "sentiment" )
                                                && paths.size() <= numAmbientEmotions ) {
                                                ofLogNotice( individual.getPath( m ) );
                                                paths.push_back( individual.getPath( m ) );
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else {
        ofLogNotice() << path << " does not exists!";
    }

    std::shuffle( std::begin( paths ), std::end( paths ), std::random_device{} );


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

        if( ledConfiguration ) {

            return;
        }
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
    case AppStates::IDLE: {

        float elapsedIndivudual = ofGetElapsedTimef() - startAmbientIndividualTime;
        float ambientElapsed = ofGetElapsedTimef() - startAmbientTime;

        if( ambientElapsed > ambientDuration ) {
            updateAmbientState();
        }

        if( individualAmbient ) {

            // Individual emotion animation
            if( elapsedIndivudual > individualAmbientDur ) {

                ( ambientIndex < paths.size() - 1 ) ? ambientIndex++ : ambientIndex = 0;

                if( configs().one().getUseArduino() )
                    arduino.sendSentimentMsg( parseSentiment( paths[ambientIndex] ) );

                startAmbientIndividualTime = ofGetElapsedTimef();
            }
        }

        break;
    }
    case AppStates::COUNTDOWN: {

        if( pMan.getDifference() > 3.0f )
            setAppState( AppStates::RECORDING );

        break;
    }
    case AppStates::RECORDING: {

        /*
        // Un comment for timed experience
        if( pMan.getTimer() > 10.0f )
            setAppState( AppStates::PROCESSING );
        */

        // if we are recording, send volume data to the arduino
        arduino.sendVolumeData( recorder.getMappedVolume() );


        break;
    }
    case AppStates::PROCESSING: {

        if( recorder.getState() == Recorder::AudioRecordingStates::STOP ) {
            recorder.setTranslation( oscMan.getTranscription() );
            recorder.setAudioState( Recorder::AudioRecordingStates::SPEECH_TO_TEXT );
        }
        else if( recorder.getIsDoneProcessing() ) {
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
        updateAmbientState();
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

        if( configs().one().getUseArduino() )
            arduino.sendSentimentMsg( parseSentiment( recorder.getSentimentPath() ) );

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

void AppManager::updateAmbientState()
{
    // update state
    individualAmbient = !individualAmbient;

    if( individualAmbient ) {
        // Set up ambient animation
        ( ambientIndex < paths.size() - 1 ) ? ambientIndex++ : ambientIndex = 0;

        if( configs().one().getUseArduino() )
            arduino.sendSentimentMsg( parseSentiment( paths[ambientIndex] ) );

        startAmbientIndividualTime = ofGetElapsedTimef();
    }
    else {
        if( configs().one().getUseArduino() )
            arduino.sendSentimentMsg( "255,95,50,255,255,255" );
    }
    startAmbientTime = ofGetElapsedTimef();
}

string AppManager::parseSentiment( string path )
{
    string rgb = "";

    // open json file
    if( ofFile::doesFileExist( path ) ) {
        ofLogNotice() << "Found  sentiment analysis " << path << ", loading";


        try {
            ofJson json = ofLoadJson( path );
            // ofLogNotice() << "JSON DUMP: " << json.dump();

            if( !json["emotion"]["color"].is_null() ) {

                for( int i = 0; i < json["emotion"]["color"].size(); i++ ) {

                    if( !json["emotion"]["color"][i]["rgb"].is_null() ) {

                        for( int j = 0; j < json["emotion"]["color"][i]["rgb"].size(); j++ ) {
                            int value = json["emotion"]["color"][i]["rgb"][j];
                            rgb += ofToString( value ) + ",";
                        }
                    }
                }
            }
        }
        catch( exception &exc ) {
            ofLogError() << "Unable to load json file";
        }
    }
    else {
        ofLogError() << "Sentiment file " << path << " does not exists!";
    }

    return rgb;
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
    case ' ':
        nextState();
        break;
    case 'b':
        oscMan.sendString( "stopRecording" );
        break;
    case 'c':
        break;
    case '1':
        // red to blue
        arduino.sendSentimentMsg( "255,0,0,255,0,0" );
        break;
    case '2':
        // pink to green
        arduino.sendSentimentMsg( "255,0,255,124,252,0" );
        break;
    case '3':
        arduino.sendSentimentMsg( "255,0,0,0,0,255" );
        break;
    default:
        break;
    }
}