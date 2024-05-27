#pragma once

#include "AppSettings.h"
#include "FontManager.h"
#include "ofMain.h"

class Page {
  public:
    Page();
    ~Page();

    void setup();

    // --- DRAWING --- //
    void drawTitle();
    void drawArtists();
    void drawDescription();
    void drawPrompt();
    void drawTemplate();
    void drawIntro();
    void drawText(string txt); 
    void drawMessageStored();
    void drawTimer();
    void drawLang();

    //////////// TIMING ///////////////////
    void  setStartTime( float startTime ) { mStartTime = startTime; }
    float getStartTime() { mStartTime; }

    // TITLE ///////////////////
    void setTitle( string title ) { mTitle = title; }
    void setDescription( string description ) { mDescription = description; }
    void setCountdown( string countdown ) { mCountdown = countdown; }
    void setTimer( string timer ) { mTimer = timer; }

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
        "Inspired by the textiles that have held humanity’s stories across time and space, this triple-woven installation is made with "
        "soft electronics to collect, store and cumulatively visualize solicited messages from viewers using sentiment analysis and long "
        "term data-textile storage. \n\nThis piece is a prototype iteration where the artists are experimenting with textile sculpture "
        "through weaving, engineering, and coding. Collaborative iteration is essential to both craft and engineering."
    };
    string mPrompt{ "Touch the screen to share your message as a sonic texture." };
    string mLang{ "This prototype understands english for now." };
    string mCountdown{ "3" };
    string mTimer{ "00:20" };
    string mStored{ "Sonic message stored." };

    glm::vec2 titlePos{ 38.0f, 100.0f };
    glm::vec2 artistPos{ 38.0f, 230.0f };
    glm::vec2 descriptionPos{ 550.0f, 350.0f };
    glm::vec2 promptPos{ 38.0f, 350.0f };
    glm::vec2 langPos{ 38.0f, 915.0f };
    glm::vec2 countDownPos{ 38.0f, 550.0f };
    glm::vec2 storedPos{ 550.0f, 250.0f };

    float mTitleSize{ 100.0f };
    float mRegTextSize{ 30.0f };
    float mDescriptionTextSize{ 45 };
    float mPromptTextSize{ 50.0f };
    float mLangTextSize{ 20.0f };
    float mColWidth{ 668.0f };
    float mColWidthDescription{ 800.0f };

    ofImage bg;
};