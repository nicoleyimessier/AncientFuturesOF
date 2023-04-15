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
    if( mPage == Pages::COUNTDOWN ) {
        timeDiffCd = ofGetElapsedTimef() - startCountdownTime;

        countDownIndex = countdownDur - floor( timeDiffCd );
        page.setCountdown( ofToString( countDownIndex ) );
    }
    else if( mPage == Pages::LISTENING ) {
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
}

void PageManager::draw()
{

    page.drawTemplate();

    switch( mPage ) {
    case Pages::INTRO: {
        //page.drawDescription();
        page.drawPrompt();
        page.drawLang();
        break;
    }
    case Pages::COUNTDOWN:
        page.drawCountdown();
        break;
    case Pages::LISTENING:
        page.drawCountdown();
        page.drawTimer();
        break;
    case Pages::PROCESSING:
        page.drawCountdown();
        break;
    case Pages::ANIMATING:
        page.drawCountdown();
        break;
    case Pages::CLOSE_OUT:
        page.drawMessageStored();
        page.drawDescription(); 
        break;
    default:
        break;
    }
}

void PageManager::setPage( Pages _page )
{
    mPage = _page;

    switch( mPage ) {
    case Pages::INTRO:
        countDownIndex = 3;
        timerIndex = 20;
        break;
    case Pages::COUNTDOWN: {
        startCountdownTime = ofGetElapsedTimef();
        break;
    }
    case Pages::LISTENING:
        countDownIndex = 3;
        // page.setCountdown( "..." );
        page.setTimer( "" ); 
        startTimer = ofGetElapsedTimef();
        break;
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