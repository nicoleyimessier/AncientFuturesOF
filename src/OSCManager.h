#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscMessage.h"
#include "ofxOscReceiver.h"
#include "ofxOscSender.h"


class OSCManager {

  public:
    OSCManager();
    ~OSCManager();
    void setup( const string &serverIp, const string &appId, int txPort, int rxPort);
    void update( float dt );
    bool isRunning() const { return mRunning; }
    void sendFloat( float value );
    void sendString( string txt );
    int  getInProgress() { return inProgress; }
    string getTranscription();
    void   clearTxt(); 

  protected:
    bool   mRunning{ false };
    bool   mIsSetup{ false };
    string mServerIp; // who to send  to
    string mClientIp; // who to listen to
    string mAppId{ "APP" };
    int    mTxPort;

    ofxOscSender  oscTx; // to send to server
    ofxOscMessage encodeOscFloat( float value ) const;
    ofxOscMessage encodeOscString( string txt ) const;

    ofxOscReceiver receiver;
    string         transcript{ "" };
    vector<string> allTxt; 
    bool           inProgress = false;
};