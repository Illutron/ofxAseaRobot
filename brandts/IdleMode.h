#pragma once
#include "Mode.h"
#include "MotionDetector.h"

class IdleMode : public Mode {
public:
	IdleMode(Robot * robot, myData * data);
	
	void init();
	void activate();
	bool deactivate();
	void step();
	MotionDetector * motionDetector;
	
	ofxVec3f idlePosition;
	ofxVec3f idleDirection;
};