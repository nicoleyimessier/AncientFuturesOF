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
        for( int i = recordingPath.size() - 1; i >= 0; i-- ) {
            if( ofDirectory::doesDirectoryExist( recordingPath.getPath( i ) ) ) {
                // year
                ofDirectory dirYear( recordingPath.getPath( i ) );
                dirYear.listDir();

                for( int j = dirYear.size() - 1; j >= 0; j-- ) {
                    if( ofDirectory::doesDirectoryExist( dirYear.getPath( j ) ) ) {
                        // month
                        ofDirectory dirMonth( dirYear.getPath( j ) );
                        dirMonth.listDir();

                        for( int k = dirMonth.size() - 1; k >= 0; k-- ) {
                            if( ofDirectory::doesDirectoryExist( dirMonth.getPath( k ) ) ) {
                                // day
                                ofDirectory dirDay( dirMonth.getPath( k ) );
                                dirDay.listDir();

                                for( int l = dirDay.size() - 1; l >= 0; l-- ) {
                                    if( ofDirectory::doesDirectoryExist( dirDay.getPath( l ) ) ) {
                                        // individual
                                        ofDirectory individual( dirDay.getPath( l ) );
                                        individual.allowExt( "json" );
                                        individual.listDir();

                                        for( int m = individual.size() - 1; m >= 0; m-- ) {
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
        ofLogNotice() << path << " does not exist!";
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

    // set up GUI
    sendColorsBtn.addListener( this, &AppManager::sendColorsBtnPressed );
    gui.setup(); // most of the time you don't need a name
    gui.add( r0.setup( "red 0", 100, 0, 255 ) );
    gui.add( g0.setup( "green 0", 100, 0, 255 ) );
    gui.add( b0.setup( "blue 0", 100, 0, 255 ) );
    gui.add( r1.setup( "red 1", 100, 0, 255 ) );
    gui.add( g1.setup( "green 1", 100, 0, 255 ) );
    gui.add( b1.setup( "blue 1", 100, 0, 255 ) );
    gui.add( sendColorsBtn.setup( "Send Colors" ) );

    // setup audio
    intro.load( "tracks\\intro.wav" );
    processing.load( "tracks\\processing.wav" );
    end.load( "tracks\\ending.wav" );

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
            setAppState( AppStates::INTRO );
        if( mAppState == AppStates::RECORDING && elapsed > 15.0f )
            setAppState( AppStates::END_RECORDING );
    }

    if( ledConfiguration ) {
        return;
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
            if( elapsedIndivudual > individualAmbientDur && paths.size() ) {

                ( ambientIndex < paths.size() - 1 ) ? ambientIndex++ : ambientIndex = 0;

                if( configs().one().getUseArduino() )
                    arduino.sendSentimentMsg( parseSentiment( paths[ambientIndex] ) );

                startAmbientIndividualTime = ofGetElapsedTimef();
            }
        }

        break;
    }
    case AppStates::INTRO: {

        if( !intro.getIsPlaying() )
            setAppState( AppStates::RECORDING );

        ofSoundUpdate();
        break;
    }
    case AppStates::RECORDING: {

        /*
        // Un comment for timed experience
        if( pMan.getTimer() > 10.0f )
            setAppState( AppStates::PROCESSING );
        */

        // if we are recording, send volume data to the arduino
        //arduino.sendVolumeData( recorder.getMappedVolume() );


        break;
    }
    case AppStates::END_RECORDING: {


        // Un comment for timed experience
        if( pMan.getTimer() > 5.0f )
            setAppState( AppStates::PROCESSING );


        break;
    }
    case AppStates::PROCESSING: {

        if( recorder.getState() == Recorder::AudioRecordingStates::STOP ) {
            recorder.setTranslation( oscMan.getTranscription() );
            recorder.setAudioState( Recorder::AudioRecordingStates::SPEECH_TO_TEXT );
        }
        else if( recorder.getIsDoneProcessing() && !processing.getIsPlaying() ) {
            setAppState( AppStates::ANIMATING );
        }

        ofSoundUpdate();

        break;
    }
    case AppStates::ANIMATING: {
        // float elapsedTime = ofGetElapsedTimef() - startAnimationTime;

        // if( elapsedTime > animationTime )
        //     setAppState( AppStates::STOPPING );

        if( !end.getIsPlaying() )
            setAppState( AppStates::STOPPING );

        ofSoundUpdate();
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

    /*
    TS_START( "recorder draw" );
    recorder.drawAudio( 1 );
    TS_STOP( "recorder draw" );
    */

    if( configs().one().mAppDebug ) {

        if( configs().one().getUseArduino() )
            arduino.drawDebug();

        recorder.drawDebug();

        ofDrawBitmapString( "Smoothed Volume" + ofToString( recorder.getSmoothedVolume() ), ofGetWidth() / 2, ofGetHeight() / 2 );
    }

    
    if( ledConfiguration ) {
        ofSetColor( 0 );
        ofDrawBitmapString( "TESTING LEDS: " + getStateString(), ofGetWidth() / 2, ofGetHeight() / 2 );
        ofSetColor( 255 );

        gui.draw();
    }
}

// ---- APP STAETS ---- //
void AppManager::stopAllAudio()
{
    intro.stop();
    processing.stop();
    end.stop();
}

void AppManager::setAppState( AppStates state )
{
    mAppState = state;

    ofLogNotice( "AppManager::setAppState" ) << "Changing state to " << getAppStateString();

    startTime = ofGetElapsedTimef();

    switch( mAppState ) {
    case AppStates::IDLE: {
        startAmbientIndividualTime = ofGetElapsedTimef();
        startAmbientTime = ofGetElapsedTimef();
        oscMan.clearTxt();
        pMan.setPage( Pages::IDLE );
        updateAmbientState();
        break;
    }
    case AppStates::INTRO: {
        pMan.setPage( Pages::INTRO );

        if( configs().one().getUseArduino() )
            arduino.sendRecording();

        stopAllAudio();
        intro.play();
        break;
    }
    case AppStates::RECORDING: {
        stopAllAudio();
        pMan.setPage( Pages::LISTENING );
        recorder.start();
        oscMan.sendString( "record" );
        break;
    }
    case AppStates::END_RECORDING: {
        pMan.setPage( Pages::END_RECORDING );
        recorder.stop();
        oscMan.sendString( "stopRecording" );

        stopAllAudio();
        processing.play();

        if( configs().one().getUseArduino() )
            arduino.sendAnalyzing();
        break;
    }
    case AppStates::PROCESSING: {
        pMan.setPage( Pages::PROCESSING );
        break;
    }
    case AppStates::ANIMATING: {
        stopAllAudio();
        end.play();

        pMan.setPage( Pages::ANIMATING );

        string txt = parseSentiment(recorder.getSentimentPath()) + "g";

        if( configs().one().getUseArduino() )
            arduino.sendSentimentMsg( txt );

        startAnimationTime = ofGetElapsedTimef();
        break;
    }
    case AppStates::STOPPING: {
        stopAllAudio();

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

    if( count < ( max - 1 ) )
        count++;
    else
        count = 0;

    setAppState( static_cast<AppStates>( count ) );
}

// ---- LED STATE ---- /
void AppManager::nextLEDState()
{
    int       count = static_cast<int>( mLedState );
    const int max = static_cast<int>( LedTestStates::NUM_LED_TESTING );

    if( count < ( max - 1 ) )
        count++;
    else
        count = 0;

    setLEDState( static_cast<LedTestStates>( count ) );
}

void AppManager::setLEDState( LedTestStates state )
{
    mLedState = state;

    switch( mLedState ) {
    case SINGLE_LED:
        arduino.sendSerialString( "z" );
        break;
    case SECOND_LED:
        arduino.sendSerialString( "y" );
        break;
    case SINGLE_COLOR:
        // Code to handle SINGLE_COLOR state
        break;
    case SINGLE_COLOR_TO_GRADIENT:
        // Code to handle SINGLE_COLOR state
        break;
    case MIX_TWO_COLORS:
        // Code to handle MIX_TWO_COLORS state
        break;
    case NUM_LED_TESTING:
        // Code to handle NUM_LED_TESTING state
        break;
    default:
        // Optional: Code to handle an unknown state
        break;
    }
}

string AppManager::getStateString()
{
    switch( mLedState ) {
    case SINGLE_LED:
        return "SINGLE_LED";
    case SECOND_LED:
        return "SECOND_LED";
    case SINGLE_COLOR:
        return "SINGLE_COLOR";
    case SINGLE_COLOR_TO_GRADIENT:
        return "SINGLE_COLOR_TO_GRADIENT";
    case MIX_TWO_COLORS:
        return "MIX_TWO_COLORS";
    case NUM_LED_TESTING:
        return "NUM_LED_TESTING";
    default:
        return "UNKNOWN_STATE";
    }
}

void AppManager::sendColorsBtnPressed()
{

    if( mLedState == MIX_TWO_COLORS ) {
        string rgb = "";

        int value_r0 = r0;
        int value_g0 = g0;
        int value_b0 = b0;
        int value_r1 = r1;
        int value_g1 = g1;
        int value_b1 = b1;

        rgb += ofToString( value_r0 ) + ",";
        rgb += ofToString( value_g0 ) + ",";
        rgb += ofToString( value_b0 ) + ",";
        rgb += ofToString( value_r1 ) + ",";
        rgb += ofToString( value_g1 ) + ",";
        rgb += ofToString( value_b1 );

        arduino.sendSentimentMsg( rgb );
    }
    else if( mLedState == SINGLE_COLOR ) {
        string rgb = "";

        int value_r0 = r0;
        int value_g0 = g0;
        int value_b0 = b0;

        rgb += ofToString( value_r0 ) + ",";
        rgb += ofToString( value_g0 ) + ",";
        rgb += ofToString( value_b0 ) + ",";
        rgb += "c";

        arduino.sendSentimentMsg( rgb );
    }
    else if( mLedState == SINGLE_COLOR_TO_GRADIENT ) {
        string rgb = "";

        int value_r0 = r0;
        int value_g0 = g0;
        int value_b0 = b0;
        int value_r1 = r1;
        int value_g1 = g1;
        int value_b1 = b1;

        rgb += ofToString( value_r0 ) + ",";
        rgb += ofToString( value_g0 ) + ",";
        rgb += ofToString( value_b0 ) + ",";
        rgb += ofToString( value_r1 ) + ",";
        rgb += ofToString( value_g1 ) + ",";
        rgb += ofToString( value_b1 ) + ",";
        rgb += "g";

        arduino.sendSentimentMsg( rgb );
    }
}

// ---- SENTIMENT ---- /
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


string AppManager::getAppStateString()
{
    switch( mAppState ) {
    case AppStates::IDLE:
        return "IDLE";
    case AppStates::INTRO:
        return "INTRO";
    case AppStates::RECORDING:
        return "RECORDING";
    case AppStates::END_RECORDING:
        return "END_RECORDING";
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
        if( mAppState == AppStates::IDLE || mAppState == AppStates::RECORDING )
            nextState();
        break;
    case 'a':
        nextState();
        break;
    case 'b':
        oscMan.sendString( "stopRecording" );
        break;
    case 'l':
        ledConfiguration = !ledConfiguration;
        if( ledConfiguration )
            nextLEDState();
        break;
    case 'n':
        nextLEDState();
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
        // ambient
        arduino.sendSentimentMsg( "255,95,50,255,255,255" );
        break;
    default:
        break;
    }
}