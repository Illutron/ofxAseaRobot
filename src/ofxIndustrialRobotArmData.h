#pragma once

#include "ofxVectorMath.h"

class ofxIndustrialRobotArmData {
public:
	ofxIndustrialRobotArmData();
	volatile float rotation;
	float length;
	float minRotation;
	float maxRotation;
	float omax, omin;
	ofxVec3f axis;  //rotation axis
	ofxVec3f offset; //The offset added to the end of the arm. Has a length
};
