#include "OSCManager.h"

OSCManager::OSCManager()
{
}

OSCManager::~OSCManager()
{
}

void OSCManager::setup( const string &serverIp, const string &appId, int txPort, int rxPort )
{
    mServerIp = serverIp;
    mTxPort = txPort;

    if( !mIsSetup ) {
        oscTx.setup( mServerIp, mTxPort );
        receiver.setup( rxPort );
        mIsSetup = true;
    }
    mRunning = true;

    ofLogNotice( "OSCManager" ) << "setup osc sender" << mServerIp << ": " << mTxPort;
}


void OSCManager::update( float dt )
{
    while( receiver.hasWaitingMessages() ) {

        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage( m );

        // check for start val
        if( m.getAddress() == "/ancient_futures" ) {

            // both the arguments are floats
            allTxt.push_back( m.getArgAsString( 0 ) );

            ofLogNotice() << "New message: " << ofGetElapsedTimef() << " " << transcript;
        }
    }
}

string OSCManager::getTranscription()
{
    transcript = "";
    for( int i = 0; i < allTxt.size(); i++ )
        transcript += allTxt[i];

    return transcript;
}

void OSCManager::clearTxt()
{
    allTxt.clear();
}


void OSCManager::sendFloat( float value )
{
    const ofxOscMessage m = encodeOscFloat( value );
    oscTx.sendMessage( m, false );

    ofLogNotice() << "Sending OSC";
}

void OSCManager::sendString( string txt )
{
    const ofxOscMessage m = encodeOscString( txt );
    oscTx.sendMessage( m, false );

    ofLogNotice() << "Sending OSC";
}

ofxOscMessage OSCManager::encodeOscFloat( float value ) const
{
    ofxOscMessage m;
    m.setAddress( "app" );
    m.addFloatArg( value );

    return m;
}


ofxOscMessage OSCManager::encodeOscString( string txt ) const
{
    ofxOscMessage m;
    m.setAddress( "app" );
    m.addStringArg( txt );

    return m;
}