#include "AppSettings.h"

void AppSettings::setup()
{
	ofFile file;

	if (file.open(ofToDataPath(mAppSettingsFile))) {
		ofJson app_settings;

		try {
			// Load the JSON.
			file >> app_settings;

			// Parse the JSON.
			if (app_settings.contains("configurable")) {
				const auto& configurable = app_settings["configurable"];

				if (configurable.contains("app")) {
					const auto& app = configurable.value("app", ofJson{});
					mWindowMode = app["window_mode"];
					mLogToFile = app.value("log_to_file", true);
					mTesting = app.value("testing", false);
					mMouseOn = app.value("mouse_on", true);
					mAppDebug = app.value("app_debug", false);
					mAppSize.x = app.value("app_width", 1920);
					mAppSize.y = app.value("app_height", 500);
                    mUseArduino = app.value( "use_arduino", false );
                    mUseOSC = app.value( "use_OSC", false );


					// paths
                    mRootPath = app.value( "root_path", "" );
                    mBatPath = app.value( "bat_path", "" );
                    mKeyPath = app.value( "key_path", "" );
                    mSentimentPath = app.value( "key_path", "" );

				}
				else
					ofLogWarning("AppSettings") << "No app settings found.";

			}
		}
		catch (const exception& exc) {
			ofLogError() << exc.what() << " while parsing " << app_settings.dump(1);
		}
	}
	else {
		ofLogError("AppSettings::setup") << "Unable to read " << mAppSettingsFile << "!";

		// NEED TO DO: Add pause and then kill app
	}
}

