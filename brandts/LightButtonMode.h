#pragma once
#include "Mode.h"

class LightButtonMode : public Mode {
public:
	LightButtonMode(Robot * robot, myData * data);
	
	void init();
	void activate();
	bool deactivate();
	void step();

	float pressedTimer;
	
		bool foundToSafePos;	
	ofxVec3f buttonPosition;
	ofxVec3f buttonDir;

};