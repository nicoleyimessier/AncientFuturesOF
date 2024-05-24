#pragma once

#include "AFStructs.h"
#include "AppSettings.h"
#include "Arduino.h"
#include "OSCManager.h"
#include "PageManager.h"
#include "Recorder.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxTimeMeasurements.h"

#include <random>

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
    string    getAppStateString();
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

    // Animation timer
    float       startAnimationTime{ 0.0f };
    float const animationTime{ 10.0f };

    // CLOSING TIMER
    float startTyTimer{ 0.0f };
    float tyTimeDur{ 20.0f };

    // osc
    OSCManager oscMan;

    // testing
    float startTime{ 0.0f };
    float durationTesting{ 10.0f };

    // json sentiment parser
    string parseSentiment( string path );

    // ambient animations
    float          startAmbientTime{ 0.0f };
    float const    ambientDuration{ 300.0f };
    float          startAmbientIndividualTime{ 0.0f };
    float const    individualAmbientDur{ 10.0f };
    int            numAmbientEmotions{ 200 };
    int            ambientIndex{ 0 };
    vector<string> paths;
    bool           individualAmbient{ true };
    void           updateAmbientState();

    // LED ANIMATION SETUP
    bool          ledConfiguration{ false };
    LedTestStates mLedState{ NUM_LED_TESTING };
    void          setLEDState( LedTestStates state );
    void          nextLEDState();
    string        getStateString();

    // GUI
    void         sendColorsBtnPressed();
    ofxPanel     gui;
    ofxIntSlider r0;
    ofxIntSlider g0;
    ofxIntSlider b0;
    ofxIntSlider r1;
    ofxIntSlider g1;
    ofxIntSlider b1;
    ofxButton    sendColorsBtn;

    // sound player
    ofSoundPlayer intro;
    ofSoundPlayer processing;
    ofSoundPlayer end;
    void          stopAllAudio(); 
};