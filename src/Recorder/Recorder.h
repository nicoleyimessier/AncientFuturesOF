#pragma once

#include "AppSettings.h"
#include "ofMain.h"
#include "ofxFFmpegRecorder.h"

class Recorder {
  public:
    Recorder();
    ~Recorder();

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
    string getVisitorPath() { return mVisitorPath; }
    bool   getAudioEnabled() { return mAudio; }
    bool   getIsRecordingVideo();
    bool   getIsDoneProcessing();
    string getSentimentPath() { return mVisitorSentimentPath; }
    int    getMappedVolume();
    float  getNormalizedVolume(); 

    //! setters
    void setAudioEnabled( bool enable ) { mAudio = enable; }
    void toggleAudio() { mAudio = !mAudio; }
    void resetVisitorPath() { mVisitorPath = ""; }

    //! outputs
    void start();
    void stop();


  private:
    //! state management
    /*
    Notes:
    - PREP_STOP is setup so that the individual ffmpeg pipes for audio and video are closed in the same
      update function and thread they are opened on. i.e. Video stops in the update funciton and audio
      stops in the audioin function.
    */
    enum class AudioRecordingStates { IDLE, RECORDING, PREP_STOP, STOP, SPEECH_TO_TEXT, SENTIMENT_ANALYSIS, NUM_STATES };
    AudioRecordingStates mAudState{ AudioRecordingStates::IDLE };
    void                 setAudioState( AudioRecordingStates state );

    //! Paths
    string mRootPath{ //"C:\\ancient_futures\\of_v0.11.2_vs2017_release\\apps\\myApps\\AncientFuturesOF\\bin\\"
        "C:\\Users\\nicol\\Documents\\code\\openFrameworks\\of_v0.11.2_vs2017_release\\of_v0.11.2_vs2017_"
        "release\\apps\\myApps\\\AncientFuturesApp\\bin\\"
    };
    string mRecordingPath{ "recordings\\" };
    string mVisitorPath{ "" };
    string mVisitorAudioPath{ "" };
    string mVisitorSentimentPath{ "" };

    //! audio
    ofSoundStream soundStream;
    short *       shortBuffer;
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

    //! Recorders
    ofxFFmpegRecorder mRecorderAud;

    //! output pipe
    FILE * mComebinedAudioVid;
    string output_file{ "" };
    string output_cmd{ "" };
    void   openOutPipe();

    //! speech to text translate + sentiment
    void   translateSpeechToText();
    void   performSentimentAnalysis();
    string translation{ "" };
    string sentimentAnalysis{ "" };

    ofTrueTypeFont font;
};