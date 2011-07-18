#pragma once

#include "ofxVectorMath.h"

class ofxIndustrialRobotTool {
public:
	ofxIndustrialRobotTool(ofxVec3f _dir, float _l, float _w, float _h);
	ofxVec3f getRotation();
	
	ofxVec3f dir;
	float l, w, h;
};