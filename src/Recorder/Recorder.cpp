#include "Recorder.h"

Recorder::Recorder()
{
}

Recorder::~Recorder()
{
    soundStream.close();
}

void Recorder::onExit( ofEventArgs &e )
{
    soundStream.close();
}

void Recorder::setup( string recordingPath )
{
    setupSoundBuffer();

    ofAddListener( ofEvents().exit, this, &Recorder::onExit );

    //! Setup audio recorder
    mRecorderAud.setup( false, true, glm::vec2( 0., 0. ) );
    mRecorderAud.setAudioConfig( 1024, 44100 );
    mRecorderAud.setOverWrite( true );

    // you don't need to set this if FFMPEG is in your system path //
    // mRecorderVideo.setFFmpegPathToAddonsPath();

    //! setup recording path
    string path = recordingPath;
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    path += "\\" + ofToString( ofGetYear() );
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    path += "\\" + ofToString( ofGetMonth() );
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    path += "\\" + ofToString( ofGetDay() );
    if( !ofDirectory::doesDirectoryExist( path ) )
        ofDirectory::createDirectory( path, true );

    mRecordingPath = path + "\\";

    // Setup audio buffer
    int bufferSize = 1024;
    inputFrames.assign( bufferSize, 0.0 );
    right.assign( bufferSize, 0.0 );
    left.assign( bufferSize, 0.0 );

    font.load( "DIN.otf", 20 );
}


void Recorder::setupSoundBuffer()
{
    ofSoundStreamSettings settings;

    /*
    auto devices = soundStream.getMatchingDevices( "Focusrite USB ASIO" );
    if( !devices.empty() )
        settings.setInDevice( devices[0] );
    */

    auto devices = soundStream.getDeviceList( ofSoundDevice::Api::MS_DS );
    for( auto &device : devices ) {
        // ofLogNotice() << device.name;
        if( device.name == "Microphone (JLAB TALK PRO MICROPHONE)" )
            settings.setInDevice( device );
    }


    settings.setApi( ofSoundDevice::Api::MS_DS );

    settings.setInListener( this );
    settings.sampleRate = 44100;
    settings.numInputChannels = 2;
    settings.bufferSize = 1024;
    soundStream.setup( settings );
}

void Recorder::update()
{
    // ofLogNotice() << "mSmoothedVol: " << mSmoothedVol;

    mScaledVol = ofMap( mSmoothedVol, 0.0, 0.05, 0.1, 1.0, true );

    if( mAudState == AudioRecordingStates::STOP ) {
        setAudioState( AudioRecordingStates::SPEECH_TO_TEXT );
    }
}

void Recorder::draw( int x, int y, float width, float height )
{
}

void Recorder::drawDebug()
{
    ofSetColor( 255, 255, 220 );

    if( mRecorderAud.isRecording() ) {
        ofDrawBitmapStringHighlight( "audio duration: " + std::to_string( mRecorderAud.getRecordedAudioDuration( 30.0f ) ), 40, 70 );
    }

    ofPushStyle();
    {
        ofDrawBitmapStringHighlight( "FPS: " + std::to_string( ofGetFrameRate() ), 10, 16 );

        if( mRecorderAud.isRecording() ) {
            ofSetColor( ofColor::red );
        }
        else {
            ofSetColor( ofColor::green );
        }
        ofDrawCircle( ofPoint( 50, 70 ), 10 );


        ofSetColor( 255 );

        // Draw the information
        string msg = "Press 'r' to start recording and 's' to stop recording.\n";
        msg += "translation: " + translation + "\n";
        msg += "sentiment: " + sentimentAnalysis + "\n";
        font.drawString( msg, 500, ofGetHeight() - 200 );
        // ofDrawBitmapStringHighlight( msg, 10, ofGetHeight() - 200 );

        ofSetColor( 0, 0, 0, 100 );
        ofFill();
        ofDrawCircle( ofGetWidth() / 2, ofGetHeight() / 2, mScaledVol * 300.0f );
    }
    ofPopStyle();


    // Draw audio
    drawAudio(1);
}

void Recorder::drawAudio( int withVolume )
{
    ofPushStyle();
    ofPushMatrix();
    ofTranslate( 0, ofGetHeight() * 0.8, 0 );

    ofNoFill();
    ofSetColor( configs().gridLight );

    ofSetLineWidth( 5 );
    ofBeginShape();
    for( unsigned int i = 0; i < inputFrames.size(); i++ ) {
        float y = 100 - inputFrames[i] * 580.0f;

        if( withVolume )
            ofVertex( i * 2, y );
        else
            ofVertex( i * 2, 100 );
    }
    ofEndShape( false );

    ofPopMatrix();
    ofPopStyle();
}

void Recorder::start()
{

    mVisitorPath = mRecordingPath + ofGetTimestampString();
    ofDirectory::createDirectory( mVisitorPath, false );
    string vid = mVisitorPath;

    //! record audio
    setAudioState( AudioRecordingStates::RECORDING );
}

void Recorder::stop()
{
    if( !mRecorderAud.isRecording() ) {
        ofLogError() << "Cant stop recording b/c it has not started!";
        return;
    }

    //! stop audio, we need to call this in the same update loop that the thread is spawned hence
    //! the state 'prep_stop'
    setAudioState( AudioRecordingStates::PREP_STOP );
}

//--------------------------------------------------------------
void Recorder::audioIn( ofSoundBuffer &input )
{

    switch( mAudState ) {

    case AudioRecordingStates::RECORDING: {
        if( mRecorderAud.isRecording() ) {
            mRecorderAud.addBuffer( input, 30.0f );
        }
        break;
    }
    case AudioRecordingStates::PREP_STOP: {
        setAudioState( AudioRecordingStates::STOP );
        break;
    }
    default:
        break;
    }

    float curVol = 0.0;
    int   numCounted = 0; // samples are "interleaved"

    for( size_t i = 0; i < input.getNumFrames(); i++ ) {
        // wave form animation
        float sample = input.getSample( i, 0 );
        float x = input[i] * 0.5;
        inputFrames[i] = x;

        // lets go through each sample and calculate the root mean square which is a rough way to calculate volume
        left[i] = input[i * 2] * 0.5;
        right[i] = input[i * 2 + 1] * 0.5;

        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        numCounted += 2;
    }


    curVol /= (float)numCounted; // mean of rms
    curVol = sqrt( curVol );     // root of rms

    mSmoothedVol *= 0.93;
    mSmoothedVol += 0.07 * curVol;

    /* for( size_t i = 0; i < input.getNumFrames(); i++ ) {
         float sample = input.getSample( i, 0 );
         float x = ofMap( i, 0, input.getNumFrames(), 0, ofGetWidth() );
         float y = ofMap( sample, -1, 1, 0, ofGetHeight() );
         waveform.addVertex( x, y );
     }*/
}

void Recorder::setAudioState( AudioRecordingStates state )
{
    mAudState = state;

    switch( mAudState ) {
    case AudioRecordingStates::IDLE: {
        resetVisitorPath();

        break;
    }
    case AudioRecordingStates::RECORDING: {

        translation = "";
        sentimentAnalysis = "";

        // mVisitorAudioPath = mVisitorPath + "\\audio.mp3";
        mVisitorAudioPath = mVisitorPath + "\\audio.wav";
        mRecorderAud.setAudOutputPath( mVisitorAudioPath );
        mRecorderAud.startCustomAudioRecord();
        break;
    }
    case AudioRecordingStates::PREP_STOP: {
        break;
    }
    case AudioRecordingStates::STOP: {
        if( mRecorderAud.isRecording() )
            mRecorderAud.stop();

        break;
    }
    case AudioRecordingStates::SPEECH_TO_TEXT: {
        translateSpeechToText();
        break;
    }
    case AudioRecordingStates::SENTIMENT_ANALYSIS: {
        performSentimentAnalysis();
        break;
    }
    default:
        break;
    }
}

void Recorder::translateSpeechToText()
{
    ofLogNotice() << "TRANSCRIBE AUDIO FILE";
    // uncomment for install computer
    // string batPath = "C:\\ancient_futures\\AncientFutures\\scripts\\transcribe.bat";
    // string keyPath = "C:\\ancient_futures\\ancient-futures-343119-c8149bde1b51.json";

    // uncomment for nicole's personal computer
    string batPath = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\AncientFutures\\scripts\\transcribe.bat";
    string keyPath = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\ancient-futures-343119-c8149bde1b51.json";
    // string audioPath = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\ManualTest.wav";

    string audioPath = mRootPath + mVisitorAudioPath;

    ofLogNotice() << "Audio path: " << audioPath;

    ofLogNotice() << "Transcribition command" << batPath + " " + keyPath + " " + audioPath;

    translation = ofSystem( batPath + " " + keyPath + " " + audioPath );

    translation.erase( remove( translation.begin(), translation.end(), '\n' ), translation.end() );


    ofLogNotice() << "Speech-to-text Result: " << translation;

    // NEED TO DO: save transcription to file
    ofJson text, json;
    text["text"] = translation;
    json.push_back( text );

    string textFile = mRootPath + mVisitorPath + "\\translation.json";
    ofSaveJson( textFile, json );
    setAudioState( AudioRecordingStates::SENTIMENT_ANALYSIS );
}

void Recorder::performSentimentAnalysis()
{
    ofLogNotice() << "PERFORM SENTIMENT ANALYSIS";
    // uncomment for install computer
    // string pythonFile = "C:\\ancient_futures\\AncientFutures\\nlk\\nlk_sentiment.py";

    // uncomment for nicole's personal computer
    string pythonFile = "C:\\Users\\nicol\\Documents\\creative\\projects\\ancient_futures\\code\\AncientFutures\\nlk\\nlk_sentiment.py";

    mVisitorSentimentPath = mRootPath + mVisitorPath + "\\sentiment.json";
    string path = ofSystem( "echo %path%" );
    string cmd = "set PATH=" + path;
    // ofLogNotice() << cmd;
    ofSystem( cmd );
    cmd = "python " + pythonFile + " " + "\"" + translation + "\"" + " " + "\"" + mVisitorSentimentPath + "\"";
    ofLogNotice() << cmd;
    sentimentAnalysis = ofSystem( cmd );
    ofLogNotice() << "Sentiment Analysis Result: " << sentimentAnalysis;

    setAudioState( AudioRecordingStates::IDLE );
}

bool Recorder::getIsDoneProcessing()
{

    if( mAudState == AudioRecordingStates::IDLE )
        return true;
    else
        return false;
}

int Recorder::getMappedVolume()
{
    return (int)ofMap( mSmoothedVol, 0.0, 0.05, 0.0, 255.0, true );
}

float Recorder::getNormalizedVolume()
{
    return ofMap( mSmoothedVol, 0.0, 0.05, 0.0, 1.0, true );
}