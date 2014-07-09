#pragma once

#include "ofMain.h"

class ofxIndustrialRobotArmData {
public:
	ofxIndustrialRobotArmData();
	volatile float rotation;
	float length;
	float minRotation;
	float maxRotation;
	float omax, omin;
	ofVec3f axis;  //rotation axis
	ofVec3f offset; //The offset added to the end of the arm. Has a length
};
