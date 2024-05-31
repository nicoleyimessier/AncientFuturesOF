#include "PageManager.h"

PageManager::PageManager()
{
}

PageManager::~PageManager()
{
}

void PageManager::setup()
{
    page.setup();
}

void PageManager::update( float dt )
{
    if( mPage == Pages::LISTENING ) {
        timeDiffTimer = ofGetElapsedTimef() - startTimer;

        timerIndex = timerDur - floor( timeDiffTimer );
        if( timerIndex > 10 ) {
            page.setCountdown( "00:" + ofToString( timerIndex ) );
            // page.setTimer( "00:" + ofToString( timerIndex ) );
        }
        else {
            page.setCountdown( "00:0" + ofToString( timerIndex ) );
            // page.setTimer( "00:0" + ofToString( timerIndex ) );
        }
    }
    else if(mPage == Pages::END_RECORDING) {
        timeDiffTimer = ofGetElapsedTimef() - startTimer;
    }
}

void PageManager::draw()
{

    //page.drawTemplate();
    string stay = "Stay with us on the telephone, and watch your story animate across the textiles through a representation of color.";

    switch( mPage ) {
    case Pages::IDLE: {
        page.drawText( "We invite you to share a story using this telephone, in the same way ancient cultures wove their histories into cloth, and see it preserved in this sonic textile. Your narrative will be stored for the future, becoming part of our collective memory.\n\nPick up the telephone and press the button to start." ); 
        //page.drawLang();
        break;
    }
    case Pages::INTRO:
        page.drawText( "" ); 
        //page.drawIntro();
        break;
    case Pages::LISTENING:
        page.drawText( "Recording in progress.\n\nPress the button to end the recording, and please stay on the telephone." ); 
        break;
    case Pages::END_RECORDING:
        page.drawText( stay );
        break;
    case Pages::PROCESSING:
        page.drawText( stay ); 
        break;
    case Pages::ANIMATING:
        page.drawText( stay );
        break; 
    case Pages::CLOSE_OUT:
        page.drawText( "Thank you for sharing your story in this sonic textile." ); 
        //page.drawMessageStored();
        //page.drawDescription(); 
        break;
    default:
        break;
    }
}

void PageManager::setPage( Pages _page )
{
    mPage = _page;

    switch( mPage ) {
    case Pages::IDLE:
        countDownIndex = 3;
        timerIndex = 20;
        break;
    case Pages::INTRO: {
        startCountdownTime = ofGetElapsedTimef();
        break;
    }
    case Pages::LISTENING:
        countDownIndex = 3;
        // page.setCountdown( "..." );
        page.setTimer( "" ); 
        startTimer = ofGetElapsedTimef();
        break;
    case Pages::END_RECORDING:
        startTimer = ofGetElapsedTimef();
    case Pages::PROCESSING:
        timerIndex = 20;
        page.setCountdown( "Processing your sonic message." );
        break;
    case Pages::ANIMATING:
        page.setCountdown( "Message stored as sonic light." );
        break;
    case Pages::CLOSE_OUT:
        break;
    default:
        break;
    }
}