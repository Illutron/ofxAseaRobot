#pragma once

#include "ofMain.h"

class ofxIndustrialRobotTool {
public:
	ofxIndustrialRobotTool(ofVec3f _dir, float _l, float _w, float _h);
	ofVec3f getRotation();
	
	ofVec3f dir;
	float l, w, h;
};