#pragma once
#include "Mode.h"

class KeyboardMode : public Mode {
public:
	KeyboardMode(Robot * robot, myData * data);
	
	void init();
	void activate();
	bool deactivate();
	void step();
	
	float pressedTimer;
	
	bool foundMasterSafePos;
	bool foundToSafePos;	
	ofxVec3f buttonQ;
	ofxVec3f buttonP;
	ofxVec3f buttonA;
	ofxVec3f buttonZ;
	int curButton;
	int curRow;
	ofxVec3f buttonDir;
};