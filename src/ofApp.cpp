#include "ofApp.h"

using namespace AncientFutures;

//--------------------------------------------------------------
void ofApp::setup()
{
    configs().setup();
    fonts().setupFonts();

    // screen setup
    ofAddListener( ss.setupChanged, this, &ofApp::setupChanged );
    // ss.setup( ofGetWidth(), ofGetHeight(), ofxScreenSetup::FULL_ONE_MONITOR );
    ss.setup( ofGetWidth(), ofGetHeight(), ofxScreenSetup::WINDOWED );
    

    ofSetFrameRate( 30 );
    ofSetVerticalSync( true );
    ofEnableSmoothing();
    ofSetCircleResolution( 80 );
    ofBackground( configs().bgColor );

    app.setup();

    // Setup touch manager

    // enable the Windows Touch Hook
    ofxWinTouchHook::EnableTouch();

    // add touch listeners
    ofAddListener( ofxWinTouchHook::touchDown, this, &ofApp::touchDown );
    ofAddListener( ofxWinTouchHook::touchUp, this, &ofApp::touchUp );

    // Time stamps
    TIME_SAMPLE_SET_FRAMERATE( 30.0f ); // specify a target framerate
}

//--------------------------------------------------------------
void ofApp::update()
{
    app.update( 1 / 30.0f );
}

//--------------------------------------------------------------
void ofApp::draw()
{
    app.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
    switch( key ) {
    case 'd':
        configs().one().mAppDebug = !configs().one().mAppDebug;
        break;
    case 's': {
        ss.cycleToNextScreenMode();
        break;
    }
    default:
        break;
    }
}

void ofApp::setupChanged( ofxScreenSetup::ScreenSetupArg &arg )
{

    ofLogNotice() << "ofxScreenSetup setup changed from " << ss.stringForMode( arg.oldMode ) << " (" << arg.oldWidth << "x" << arg.oldHeight
                  << ") "
                  << " to " << ss.stringForMode( arg.newMode ) << " (" << arg.newWidth << "x" << arg.newHeight << ")";
}


//--------------------------------------------------------------
void ofApp::touchDown( ofTouchEventArgs &touch )
{
    ofLogNotice() << "touch down: " << touch.x << ", " << touch.y << ", id: " << touch.id;
}


//--------------------------------------------------------------
void ofApp::touchUp( ofTouchEventArgs &touch )
{
    if( !configs().getUseMouse() ) {
        ofLogNotice() << "touch up: " << touch.x << ", " << touch.y << ", id: " << touch.id;
        app.nextState();
    }
}


//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    if( configs().getUseMouse() ) {
        ofLogNotice() << "mouseReleased: " << x << ", " << y;

        if( app.getAppState() == AppStates::IDLE || app.getAppState() == AppStates::STOPPING )
            app.nextState();
    }
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h )
{
}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg )
{
}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo )
{
}
