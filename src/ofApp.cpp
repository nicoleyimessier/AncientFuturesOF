#include "ofApp.h"

using namespace AncientFutures; 

//--------------------------------------------------------------
void ofApp::setup()
{
    configs().setup(); 
    fonts().setupFonts(); 

    ofSetFrameRate( 30 );
    ofSetVerticalSync( true );
    ofEnableSmoothing();
    ofSetCircleResolution( 80 );
    ofBackground( configs().bgColor );

    
    app.setup();
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
        configs().one().appDebug = !configs().one().appDebug;
        break; 
    case 'r': {
        app.setAppState( AppStates::RECORDING );
        break;
    }
    case 's': {
        app.setAppState( AppStates::PROCESSING );
        break;
    }
    default:
        break;
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
    app.nextState(); 
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
