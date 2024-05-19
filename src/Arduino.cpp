#include "Arduino.h"

Arduino::Arduino()
{
}

Arduino::~Arduino()
{
}

void Arduino::setup( string port )
{

    serial.listDevices();
    vector<ofSerialDeviceInfo> deviceList = serial.getDeviceList();

    // cout << deviceList[0].getDeviceName();
    int baud = 9600;
    // serial.setup( 1, baud ); // open the first device
    // serial.setup( "COM3", baud );
    serial.setup( port, baud ); // windows example
    // serial.setup("/dev/tty.usbserial-A4001JEC", baud); // mac osx example
    // serial.setup("/dev/ttyUSB0", baud); //linux example

    memset( bytesReadString, 0, 4 );
}

void Arduino::update()
{
    if( bSendSerialMessage ) {

        /*
            // (1) write the letter "a" to serial:
            if( sendPostiveAnimation )
                serial.writeByte( 'p' );
            else if( sendNegativeAnimation )
                serial.writeByte( 'n' );
            else if( sendNetrualAnimation )
                serial.writeByte( 'c' );
                    */


        for( int i = 0; i < mTxt.size(); i++ ) {
            serial.writeByte( mTxt[i]); 
        }

        serial.writeByte( '\n' );
        bSendSerialMessage = false;
    }
    else if( sendStopAnimation ) {
        serial.writeByte( 's' );
        serial.writeByte( '\n' );
        sendStopAnimation = false;
        resetAllMsgValues();
    }
    else if( sendRecordingFlag ) {
        serial.writeByte( 'r' );
        serial.writeByte( '\n' );
        sendRecordingFlag = false;
        resetAllMsgValues();
    }
    else if( sendAnalyzingFlag ) {
        serial.writeByte( 'a' );
        serial.writeByte( '\n' );
        sendAnalyzingFlag = false;
        resetAllMsgValues();
    }
    else if( sendVolumeFlag ) {
        string volume = ofToString( mVolume );

        serial.writeByte( 'v' );

        for( auto &letter : volume ) {
            serial.writeByte( (char)letter );
        }


        serial.writeByte( '\n' );
        serial.flush();
        // ofLogNotice() << "send volume " << volume;
        sendVolumeFlag = false;
        resetAllMsgValues();
    }


    // (2) read
    // now we try to read 3 bytes
    // since we might not get them all the time 4 - but sometimes 0, 6, or something else,
    // we will try to read three bytes, as much as we can
    // otherwise, we may have a "lag" if we don't read fast enough
    // or just read three every time. now, we will be sure to
    // read as much as we can in groups of three...

    /*
    nTimesRead = 0;
    nBytesRead = 0;
    int nRead = 0; // a temp variable to keep count per read

    unsigned char bytesReturned[4];

    memset( bytesReadString, 0, 5 );
    memset( bytesReturned, 0, 4 );

    while( ( nRead = serial.readBytes( bytesReturned, 4 ) ) > 0 ) {
        nTimesRead++;
        nBytesRead = nRead;
    };

    memcpy( bytesReadString, bytesReturned, 4 );

    bSendSerialMessage = false;
    readTime = ofGetElapsedTimef();
    */
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
    ofDrawBitmapString( msg, ofGetWidth() / 2, 100 );
}

void Arduino::sendSentimentMsg( float pos, float neg, float netrual )
{

    if( pos > neg ) {
        if( pos > netrual ) {
            sendPostiveAnimation = true;
            ofLogNotice() << "Send positive animation: " << sendPostiveAnimation;
        }
        else {
            sendNetrualAnimation = true;
            ofLogNotice() << "Send netrual animation: " << sendNetrualAnimation;
        }
    }
    else if( neg > netrual ) {
        sendNegativeAnimation = true;
        ofLogNotice() << "Send negative animation: " << sendNegativeAnimation;
    }
    else {
        sendNetrualAnimation = true;
        ofLogNotice() << "Send netrual animation: " << sendNetrualAnimation;
    }


    bSendSerialMessage = true;
}

void Arduino::sendSerialString( string txt )
{

    mTxt = txt;
    bSendSerialMessage = true;
}


void Arduino::sendRecording()
{
    sendRecordingFlag = true;
}

void Arduino::sendAnalyzing()
{
    sendAnalyzingFlag = true;
}

void Arduino::sendStopMsg()
{
    sendStopAnimation = true;
}


void Arduino::resetAllMsgValues()
{
    sendRecordingFlag = false;
    sendAnalyzingFlag = false;
    sendPostiveAnimation = false;
    sendNegativeAnimation = false;
    sendNetrualAnimation = false;
    sendVolumeFlag = false;
}

void Arduino::sendVolumeData( int volume )
{
    sendVolumeFlag = true;
    mVolume = volume;
}
