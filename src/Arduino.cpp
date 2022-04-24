#include "Arduino.h"

Arduino::Arduino()
{
}

Arduino::~Arduino()
{
}

void Arduino::setup()
{

    serial.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();

    cout << deviceList[0].getDeviceName();
    int baud = 9600;
    serial.setup( 0, baud ); // open the first device
    // serial.setup("COM4", baud); // windows example
    // serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
    // serial.setup("/dev/ttyUSB0", baud); //linux example

    memset( bytesReadString, 0, 4 );
}

void Arduino::update()
{
    if( bSendSerialMessage ) {

        // (1) write the letter "a" to serial:
        ( sendPostiveAnimation ) ? serial.writeByte( 'p' ) : serial.writeByte( 'n' );
        serial.writeByte( '\n' );

        // (2) read
        // now we try to read 3 bytes
        // since we might not get them all the time 3 - but sometimes 0, 6, or something else,
        // we will try to read three bytes, as much as we can
        // otherwise, we may have a "lag" if we don't read fast enough
        // or just read three every time. now, we will be sure to
        // read as much as we can in groups of three...

        nTimesRead = 0;
        nBytesRead = 0;
        int nRead = 0; // a temp variable to keep count per read

        unsigned char bytesReturned[3];

        memset( bytesReadString, 0, 4 );
        memset( bytesReturned, 0, 3 );

        while( ( nRead = serial.readBytes( bytesReturned, 3 ) ) > 0 ) {
            nTimesRead++;
            nBytesRead = nRead;
        };

        memcpy( bytesReadString, bytesReturned, 3 );

        bSendSerialMessage = false;
        readTime = ofGetElapsedTimef();
    }

    if( sendStopAnimation ) {
        serial.writeByte( 's' );
        serial.writeByte( '\n' );
        sendStopAnimation = false;
    }
}

void Arduino::drawDebug()
{
    if( nBytesRead > 0 && ( ( ofGetElapsedTimef() - readTime ) < 0.5f ) ) {
        ofSetColor( 0 );
    }
    else {
        ofSetColor( 220 );
    }
    string msg;
    msg += "serial data:\n";
    msg += "nBytes read " + ofToString( nBytesRead ) + "\n";
    msg += "nTimes read " + ofToString( nTimesRead ) + "\n";
    msg += "read: " + ofToString( bytesReadString ) + "\n";
    msg += "(at time " + ofToString( readTime, 3 ) + ")";
    ofDrawBitmapString( msg, 50, 100 );
}

void Arduino::sendSerialMsg( float pos, float neg )
{
    if( pos > neg )
        sendPostiveAnimation = true;
    else if( neg > pos )
        sendPostiveAnimation = false;

    ofLogNotice() << "Send positive animation: " << sendPostiveAnimation;
    bSendSerialMessage = true;
}

void Arduino::sendStopMsg()
{
    sendStopAnimation = true;
}