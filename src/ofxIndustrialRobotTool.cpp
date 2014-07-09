#include "ofxIndustrialRobotTool.h"

#include "ofMain.h"


ofxIndustrialRobotTool::ofxIndustrialRobotTool(ofVec3f _dir, float _l, float _w, float _h){
	dir = _dir;
	l = _l;
	w = _w;
	h = _h;
}

ofVec3f ofxIndustrialRobotTool::getRotation(){
	ofVec3f v;
	v.z = ofVec3f(0.0, 1.0, 0.0).angle(dir);	
	return v;
}