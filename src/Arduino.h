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
    void sendSerialMsg( float pos, float neg );
    void sendStopMsg(); 

  private:
    bool  bSendSerialMessage{ false }; // a flag for sending serial
    char  bytesRead[3];                // data from serial, we will be trying to read 3
    char  bytesReadString[4];          // a string needs a null terminator, so we need 3 + 1 bytes
    int   nBytesRead{ 0 };             // how much did we read?
    int   nTimesRead{ 0 };             // how many times did we read?
    float readTime{ 0.0f };

    ofSerial serial;

    // NEED TO DO: make into enum state machine
    bool sendPostiveAnimation{ false };
    bool sendStopAnimation{ true };
};