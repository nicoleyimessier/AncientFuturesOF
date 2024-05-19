#pragma once

#include "AppSettings.h"
#include "ofMain.h"

class Recorder {
  public:
    Recorder();
    ~Recorder();

    enum class AudioRecordingStates { IDLE, RECORDING, PREP_STOP, STOP, SPEECH_TO_TEXT, SENTIMENT_ANALYSIS, NUM_STATES };


    void setup( string recordingPath );
    void setupOptions();
    void setupSoundBuffer();
    void update();
    void audioIn( ofSoundBuffer &input );
    void draw( int x, int y, float width, float height );
    void drawDebug();
    void drawAudio( int withVolume );
    void onExit( ofEventArgs &e );

    //! getters
    string                         getVisitorPath() { return mVisitorPath; }
    bool                           getAudioEnabled() { return mAudio; }
    bool                           getIsRecordingVideo();
    bool                           getIsDoneProcessing();
    string                         getSentimentPath() { return mVisitorSentimentPath; }
    int                            getMappedVolume();
    float                          getNormalizedVolume();
    Recorder::AudioRecordingStates getState() { return mAudState; }

    //! setters
    void setAudioEnabled( bool enable ) { mAudio = enable; }
    void toggleAudio() { mAudio = !mAudio; }
    void resetVisitorPath() { mVisitorPath = ""; }
    void setTranslation( string txt ) { translation = txt; }
    void setAudioState( AudioRecordingStates state );
    void setApiKey( string key ) { mApiKey = key; }

    //! outputs
    void start();
    void stop();


  private:
    //! state management
    AudioRecordingStates mAudState{ AudioRecordingStates::IDLE };

    //! Paths
    string mApiKey{ "" };
    string mRootPath{ "" };
    string mSentimentPath{ "" };
    string mRecordingPath{ "recordings\\" };
    string mVisitorPath{ "" };
    string mVisitorAudioPath{ "" };
    string mVisitorSentimentPath{ "" };

    //! audio
    ofSoundStream soundStream;
    short        *shortBuffer;
    vector<float> inputFrames;
    vector<float> left;
    vector<float> right;
    size_t        lastAudioTimeReset;
    int           bufferCounter;
    int           audioCounter;
    float         audioFPS;        //! used to calculate audio fps
    bool          mAudio{ false }; // setDisableAudio
    float         mScaledVol{ 0.0f };
    float         mSmoothedVol{ 0.0f };

    //! speech to text translate + sentiment
    void   translateSpeechToText();
    void   performSentimentAnalysis();
    string translation{ "" };
    string sentimentAnalysis{ "" };

    ofTrueTypeFont font;
};