#pragma once
#include "ofMain.h"

class Arduino {
  public:
    Arduino();
    ~Arduino();

    void setup();
    void update();
    void drawDebug();

    // Communcations
    void sendSentimentMsg( float pos, float neg, float netrual );
    void sendRecording();
    void sendAnalyzing();
    void sendStopMsg();
    void resetAllMsgValues();
    void sendVolumeData( int volume );

  private:
    bool     bSendSerialMessage{ false }; // a flag for sending serial
    char     bytesRead[3];                // data from serial, we will be trying to read 3
    char     bytesReadString[4];          // a string needs a null terminator, so we need 3 + 1 bytes
    int      nBytesRead{ 0 };             // how much did we read?
    int      nTimesRead{ 0 };             // how many times did we read?
    float    readTime{ 0.0f };
    int      mVolume{ 0 };
    ofSerial serial;

    // NEED TO DO: make into enum state machine
    bool sendPostiveAnimation{ false };
    bool sendNegativeAnimation{ false };
    bool sendNetrualAnimation{ false };
    bool sendRecordingFlag{ false };
    bool sendAnalyzingFlag{ false };
    bool sendStopAnimation{ true };
    bool sendVolumeFlag{ false };
};