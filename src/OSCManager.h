#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxOscMessage.h"
#include "ofxOscSender.h"

// listening port
#define PORT 5000

class OSCManager {

  public:
    OSCManager();
    ~OSCManager();
    void setup( const string &serverIp,  // who to send  to
        const string &        appId,     //
        int                   txPort,    //
        const string &        devIp = "" //
    );
    void update( float dt );
    bool isRunning() const { return mRunning; }
    void send( float value );

    int getStartExpereince() { return startExperience; }
    int getInProgress() { return inProgress; }

  protected:
    bool   mRunning{ false };
    bool   mIsSetup{ false };
    string mServerIp; // who to send  to
    string mDeviceIp; // my own IP
    string mAppId{ "APP" };
    int    mTxPort;

    ofxOscSender  oscTx; // to send to server
    ofxOscMessage encodeOsc( float value ) const;


    ofxOscReceiver receiver;
    int            startExperience = 0;
    bool           inProgress = false;
};