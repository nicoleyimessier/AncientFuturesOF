//
//  FontManager.hpp
//  TypeTest
//
//  Created by Nicole Yi Messier on 4/23/20.
//

#pragma once
#include "ofMain.h"
#include "ofxFontStash.h"

class FontManager {
    bool setup_done = false;

  public:
    static FontManager &one()
    {
        static FontManager instance;
        return instance;
    };

    void setupFonts();

    // --- FONTS ---- //
    ofxFontStash NunitoSansBold;
    ofxFontStash NunitoSansReg;

    float lineHeightPercent = 1.0f;
    int   texDimension = 1024;
    bool  createMipMaps = false;
    int   intraCharPadding = 10;
    float dpiScale = 1.5f;
};

static FontManager &fonts()
{
    return FontManager::one();
}
