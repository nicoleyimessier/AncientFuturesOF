#pragma once

#include "ofMain.h"
//#include "ofxAnimatableFloat.h"

namespace AncientFutures {


enum class AppStates { IDLE, INTRO, RECORDING, END_RECORDING, PROCESSING, ANIMATING, STOPPING, NUM_STATES };
enum class Pages { IDLE, INTRO, LISTENING, END_RECORDING, PROCESSING, ANIMATING, CLOSE_OUT, NUM_STATES };
enum LedTestStates { SINGLE_LED, SECOND_LED, SINGLE_COLOR, MIX_TWO_COLORS, NUM_LED_TESTING};


}; // namespace AncientFutures

