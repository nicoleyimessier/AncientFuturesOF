#pragma once

#include "AFStructs.h"
#include "AppManager.h"
#include "AppSettings.h"
#include "FontManager.h"
#include "ofMain.h"
#include "ofxScreenSetup.h"
#include "ofxWinTouchHook.h"

class ofApp : public ofBaseApp {

  public:
    void setup();
    void update();
    void draw();

    void keyPressed( int key );
    void keyReleased( int key );
    void mouseMoved( int x, int y );
    void mouseDragged( int x, int y, int button );
    void mousePressed( int x, int y, int button );
    void mouseReleased( int x, int y, int button );
    void mouseEntered( int x, int y );
    void mouseExited( int x, int y );
    void windowResized( int w, int h );
    void dragEvent( ofDragInfo dragInfo );
    void gotMessage( ofMessage msg );

    AppManager     app;
    void           setupChanged( ofxScreenSetup::ScreenSetupArg &arg );
    ofxScreenSetup ss;

    // --- Touch Manager --- // 
    // add the touch listeners
    void touchDown( ofTouchEventArgs &touch );
    void touchUp( ofTouchEventArgs &touch );
};
