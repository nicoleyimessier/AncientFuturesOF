#pragma once

#include "ofMain.h"
#include "FontManager.h"
#include "AppSettings.h"

class Page {
  public:
    Page();
    ~Page();

    void setup(); 

    // --- DRAWING --- //
    void drawTitle(); 
    void drawArtists(); 
    void drawDescription(); 
    void drawTemplate(); 
    void drawCountdown(); 
    void drawTimer(); 

    //////////// TIMING ///////////////////
    void  setStartTime( float startTime ) { mStartTime = startTime;  }
    float getStartTime() { mStartTime;  }

    // TITLE ///////////////////
    void   setTitle( string title ) { mTitle = title; }
    void setDescription( string description ) { mDescription = description; }
    void   setCountdown( string countdown ) { mCountdown = countdown; } 
    void   setTimer( string timer ) { mTimer = timer;  }

  private:
    //////////// VISITORPROFILE ///////////////////
    // VisitorProfile *vpRef;

    //////////// TIMING ///////////////////
    float mStartTime{ 0.0f };
    float mResetTime{ 60.0f };
    float mTestingTime{ 30.0f };

    ////////////  TITLE ///////////////////
    string mTitle{ "" };
    string mDescription{
        "INSERT PROJECT DESCRIPTION/PROMPT. Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore "
        "et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
        "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
        "occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum"
    };
    string    mCountdown{ "3" }; 
    string    mTimer{ "00:20" }; 

    glm::vec2 titlePos{ 38.0f, 100.0f }; 
    glm::vec2 artistPos{ 38.0f, 230.0f }; 
    glm::vec2 descriptionPos{ 38.0f, 350.0f }; 
    glm::vec2 countDownPos{ 38.0f, 550.0f }; 

    float     mTitleSize{ 100.0f }; 
    float     mRegTextSize{ 30.0f }; 
    float     mColWidth{ 368.0f }; 

    ofImage bg; 
};