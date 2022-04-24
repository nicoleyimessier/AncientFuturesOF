#include "AppSettings.h"

void AppSettings::setup()
{
    ofFile file;

    if( file.open( ofToDataPath( mAppSettingsFile ) ) ) {
        ofJson app_settings;
    }
    else {
        ofLogError( "AppSettings::setup" ) << "Unable to read " << mAppSettingsFile << "!";

        // NEED TO DO: Add pause and then kill app
    }
}
