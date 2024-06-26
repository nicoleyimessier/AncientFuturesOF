#pragma once
#include "AFStructs.h"
#include "ofMain.h"

struct Settings {
    virtual ~Settings() = default;
    virtual void   parseJson( const ofJson &json ) = 0;
    virtual ofJson toJson() const = 0;
};

class AppSettings final {
  public:
    static AppSettings &one()
    {
        static AppSettings instance;
        return instance;
    }
    ~AppSettings() = default;

    void setup();

    // --- APP SETTINGS --- //
    bool getLogToFile() const { return mLogToFile; }
    bool getUseMouse() const { return mUseMouse; }

    // --- COLORS --- //
    ofColor bgColor = ofColor( 117, 95, 134 );
    ofColor textColor{ ofColor( 229, 229, 229 ) };
    ofColor gridDark{ ofColor( 124, 65, 91, 255 * .6 ) };
    ofColor gridLight{ ofColor( 173, 129, 177, 255 * .7 ) };

    // --- Changable flags --- //
    bool mAppDebug{ false }; // global debug state for application

  private:
    // --- FLAGS --- //
    bool mLogToFile{ true };
    bool mUseMouse{ true };

    // --- CLASS SPECIFIC --- //
    string mAppSettingsFile = "settings/AppSettings.json";
};

static AppSettings &configs()
{
    return AppSettings::one();
}