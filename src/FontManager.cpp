#include "FontManager.h"

void FontManager::setupFonts()
{
    if( setup_done )
        return;

    // ofTrueTypeFont::setGlobalDpi(72);
    NunitoSansBold.setup( "fonts/Nunito_Sans/NunitoSans-Bold.ttf", lineHeightPercent, texDimension, createMipMaps, intraCharPadding, dpiScale );
    NunitoSansReg.setup(
        "fonts/Nunito_Sans/NunitoSans-Regular.ttf", lineHeightPercent, texDimension, createMipMaps, intraCharPadding, dpiScale );
   
    NunitoSansBold.setLineHeight( 0.8f ); 
    NunitoSansReg.setLineHeight( 1.0f ); 

    //*/
    setup_done = true;
}

