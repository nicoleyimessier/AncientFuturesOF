#pragma once

#include "Arduino.h"
#include "Recorder.h"
#include "ofMain.h"
#include "AFStructs.h"
#include "AppSettings.h"
#include "PageManager.h"
#include "OSCManager.h"

using namespace AncientFutures; 

class AppManager {

  public:
    AppManager();
    ~AppManager();

    void setup();
    void update( float dt );
    void draw();

    // App states
    void setAppState( AppStates state );
    void nextState(); 

    // Getters
    string getAppStateString(); 
    AppStates getAppState() { return mAppState; }; 

  private:

    // Functions
    void onKeyPressed( ofKeyEventArgs &e );
    
    // App states
    AppStates mAppState{ AppStates::IDLE };

    // Pages
    PageManager pMan; 

    // Recorder
    Recorder recorder;

    // Arduino + Serial Comms
    Arduino arduino;
    bool useArduino{ true }; 

    // Animation timer
    float startAnimationTime{ 0.0f };
    float animationTime{ 10.0f };

    // CLOSING TIMER
    float startTyTimer{ 0.0f };
    float tyTimeDur{ 20.0f };

    // osc
    OSCManager oscMan; 
    bool usingOsc = false; 
  
};