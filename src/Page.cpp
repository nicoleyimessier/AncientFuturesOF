#include "Page.h"

Page::Page()
{
}

Page::~Page()
{
}

void Page::setup()
{
    bg.load( "images/background.png" ); 
}

void Page::drawTitle()
{
    int    numLines;
    bool   wordsWereCropped;
    string msg = "ANCIENT\nFUTURES"; 


    ofSetColor( configs().textColor );
    fonts().NunitoSansBold.drawMultiLineColumn( msg,     // string
        mTitleSize,               // size
        titlePos.x, titlePos.y, // position
        mColWidth,                // column width
        numLines,                 // get back the number of lines
        false,                    // true would not draw, just get back the rectangle
        5,                        // max number of lines
        true,                     // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,        // this is set to true if the box was too small to fit all of the text
        false                     // centered
    );
}

void Page::drawArtists()
{
    int  numLines;
    bool wordsWereCropped;
    string msg = "Victoria Manganiello &\nNicole Yi Messier"; 

    ofSetColor( configs().textColor );
    fonts().NunitoSansReg.drawMultiLineColumn( msg, // string
        mRegTextSize,                               // size
        artistPos.x, artistPos.y,                   // position
        mColWidth,                                  // column width
        numLines,                                   // get back the number of lines
        false,                                      // true would not draw, just get back the rectangle
        5,                                          // max number of lines
        true,                                       // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                          // this is set to true if the box was too small to fit all of the text
        false                                       // centered
    );
}


void Page::drawDescription()
{
    int    numLines;
    bool   wordsWereCropped;

    ofSetColor( configs().textColor );
    fonts().NunitoSansReg.drawMultiLineColumn( mDescription, // string
        mRegTextSize,                               // size
        descriptionPos.x, descriptionPos.y,                  // position
        mColWidth,                                  // column width
        numLines,                                   // get back the number of lines
        false,                                      // true would not draw, just get back the rectangle
        55,                                          // max number of lines
        true,                                       // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                          // this is set to true if the box was too small to fit all of the text
        false                                       // centered
    );
}

void Page::drawPrompt()
{
    int  numLines;
    bool wordsWereCropped;

    ofSetColor( configs().textColor );
    fonts().NunitoSansBold.drawMultiLineColumn( ofToUpper(mPrompt), // string
        mPromptTextSize,                                      // size
        promptPos.x, promptPos.y,                        // position
        mColWidth,                                           // column width
        numLines,                                            // get back the number of lines
        false,                                               // true would not draw, just get back the rectangle
        55,                                                  // max number of lines
        true,                                                // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                                   // this is set to true if the box was too small to fit all of the text
        false                                                // centered
    );
}

void Page::drawLang()
{
    int  numLines;
    bool wordsWereCropped;

    ofSetColor( configs().textColor );
    fonts().NunitoSansReg.drawMultiLineColumn(  mLang , // string
        mLangTextSize,                                             // size
        langPos.x, langPos.y,                                      // position
        mColWidth,                                                    // column width
        numLines,                                                     // get back the number of lines
        false,                                                        // true would not draw, just get back the rectangle
        55,                                                           // max number of lines
        true,              // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped, // this is set to true if the box was too small to fit all of the text
        false              // centered
    );
}

void Page::drawCountdown()
{
    int  numLines;
    bool wordsWereCropped;

    ofSetColor( configs().textColor );
    fonts().NunitoSansBold.drawMultiLineColumn( mCountdown, // string
        mTitleSize,                                        // size
        countDownPos.x, countDownPos.y,                     // position
        mColWidth,                                           // column width
        numLines,                                            // get back the number of lines
        false,                                               // true would not draw, just get back the rectangle
        55,                                                  // max number of lines
        true,                                                // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                                   // this is set to true if the box was too small to fit all of the text
        true                                                // centered
    );
}

void Page::drawTimer() 
{
    fonts().NunitoSansBold.draw( mTimer, mRegTextSize, artistPos.x, ofGetHeight() * 0.87);
}

void Page::drawTemplate()
{
    bg.draw( ofGetWidth() / 2 - bg.getWidth() / 2, 0.0f ); 
    drawTitle();
    drawArtists();
}