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
    // recorder.setup( "D:\\recordings\\" );
    recorder.setup( ofToDataPath( "recordings" ) );

    //! setup Arduino
    if( useArduino ) {
        arduino.setup();
        arduino.sendStopMsg();
    }

    pMan.setup();
}

void AppManager::update( float dt )
{
    recorder.update();
    arduino.update();
    pMan.update( dt );

    if( mAppState == AppStates::COUNTDOWN && pMan.getDifference() > 3.0f ) {
        setAppState( AppStates::RECORDING );
    }
    else if( mAppState == AppStates::RECORDING && pMan.getTimer() > 20.0f ) {
        setAppState( AppStates::PROCESSING );
    }
    else if( mAppState == AppStates::PROCESSING && recorder.getIsDoneProcessing() ) {
        setAppState( AppStates::ANIMATING );
    }
    else if( mAppState == AppStates::ANIMATING ) {
        float elapsedTime = ofGetElapsedTimef() - startAnimationTime;

        if( elapsedTime > animationTime )
            setAppState( AppStates::STOPPING );
    }
    if( mAppState == AppStates::STOPPING && ( ofGetElapsedTimef() - startTyTimer )> tyTimeDur ) {
        setAppState( AppStates::IDLE );
    }
}

void AppManager::draw()
{
    pMan.draw();
    recorder.drawAudio();

    if( configs().one().appDebug ) {

        if( useArduino )
            arduino.drawDebug();

        recorder.drawDebug();
    }
}

// ---- APP STAETS ---- //
void AppManager::setAppState( AppStates state )
{
    mAppState = state;

    ofLogNotice( "AppManager::setAppState" ) << "Changing state to " << getAppStateString();

    switch( mAppState ) {
    case AppStates::IDLE: {
        pMan.setPage( Pages::INTRO );
        break;
    }
    case AppStates::COUNTDOWN: {
        pMan.setPage( Pages::COUNTDOWN );

        break;
    }
    case AppStates::RECORDING: {
        pMan.setPage( Pages::LISTENING );
        recorder.start();
        break;
    }
    case AppStates::PROCESSING: {
        pMan.setPage( Pages::PROCESSING );
        recorder.stop();
        break;
    }
    case AppStates::ANIMATING: {
        pMan.setPage( Pages::ANIMATING );

        float pos = 0.0f;
        float neg = 0.0f;

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
            }
            catch( exception &exc ) {
                ofLogError() << "Unable to load json file";
            }
        }
        else {
            ofLogError() << "Sentiment file " << recorder.getSentimentPath() << " does not exists!";
        }

        if( useArduino )
            arduino.sendSerialMsg( pos, neg );

        startAnimationTime = ofGetElapsedTimef();
        break;
    }
    case AppStates::STOPPING: {
        if( useArduino )
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