#include "OSCManager.h"

OSCManager::OSCManager()
{
}

OSCManager::~OSCManager()
{
}

void OSCManager::setup( const string &serverIp, const string &appId, int txPort, const string &devIp )
{
    mServerIp = devIp;
    mServerIp = serverIp;
    mTxPort = txPort;

    if( !mIsSetup ) {
        oscTx.setup( mServerIp, mTxPort );
        mIsSetup = true;
    }
    mRunning = true;

    ofLogNotice( "OSCManager" ) << "setup osc sender" << mServerIp << ": " << mTxPort;

    receiver.setup( PORT );
}


void OSCManager::update( float dt )
{
    while( receiver.hasWaitingMessages() ) {

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage( m );

        // check for start val
        if( m.getAddress() == "/AF" ) {

            // both the arguments are floats
            startExperience = m.getArgAsInt( 0 );

            ofLogNotice() << "New message: " << ofGetElapsedTimef() << " " <<  startExperience;
        }
    }
}

void OSCManager::send(float value) 
{
    const ofxOscMessage m = encodeOsc( value );
    oscTx.sendMessage( m, false );

    //ofLogNotice() << "Sending OSC";
}

ofxOscMessage OSCManager::encodeOsc( float value ) const
{
    ofxOscMessage m;
    m.setAddress( "/app" );
    m.addFloatArg( value );

    return m;
}