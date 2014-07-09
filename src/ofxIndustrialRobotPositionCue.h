#pragma once

#include "ofxIndustrialRobotCue.h"
#include "ofMain.h"

class ofxIndustrialRobotPositionCue : public ofxIndustrialRobotCue {
public:
	ofVec3f position, hand;
	ofxIndustrialRobotPositionCue(int speed, ofVec3f _position, ofVec3f _hand=ofVec3f(), bool cubicSpline = true, bool endPause=false, bool _endInZeroSpeed=false);	
	bool endPause;
	bool isCubicSpline;
	bool endInZeroSpeed;
};
