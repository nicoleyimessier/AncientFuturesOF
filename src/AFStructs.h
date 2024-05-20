#pragma once

#include "ofMain.h"
//#include "ofxAnimatableFloat.h"

namespace AncientFutures {


enum class AppStates { IDLE, COUNTDOWN, RECORDING, PROCESSING, ANIMATING, STOPPING, NUM_STATES };
enum class Pages { INTRO, COUNTDOWN, LISTENING, PROCESSING, ANIMATING, CLOSE_OUT, NUM_STATES };
enum LedTestStates { SINGLE_LED, SECOND_LED, SINGLE_COLOR, MIX_TWO_COLORS, NUM_LED_TESTING};


}; // namespace AncientFutures

