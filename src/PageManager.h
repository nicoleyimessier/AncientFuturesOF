#pragma once
#include "AFStructs.h"
#include "ofMain.h"
#include "Page.h"

using namespace AncientFutures;

class PageManager {
  public:
    PageManager();
    ~PageManager();

    void setup();
    void update( float dt );
    void draw();

    // Page states
    void setPage( Pages _page );

    // Getters
    float getDifference() { return timeDiffCd; }
    float getTimer() { return timeDiffTimer; }

  private:
    // App states
    Pages mPage{ Pages::INTRO };

    // Pages
    Page page; 

    // Countdown 
    float startCountdownTime{ 0.0f }; 
    float countdownDur{ 3.0f }; 
    int   countDownIndex{ 3 }; 
    float timeDiffCd{ 0.0f }; 

    // Timer
    float startTimer{ 0.0f };
    float timerDur{ 20.0f };
    int   timerIndex{ 20 };
    float timeDiffTimer{ 0.0f }; 

};

/*
switch( page ) {
    case Pages::INTRO:
        break;
        break;
    case Pages::COUNTDOWN:
        break;
    case Pages::LISTENING:
        break;
    case Pages::PROCESSING:
        break;
    case Pages::ANIMATING:
        break;
    case Pages::CLOSE_OUT:
        break;
    default:
        break;
    }
*/