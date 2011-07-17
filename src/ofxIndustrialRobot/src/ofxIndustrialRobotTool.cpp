#include "ofxIndustrialRobotTool.h"


ofxIndustrialRobotTool::ofxIndustrialRobotTool(ofxVec3f _dir, float _l, float _w, float _h){
	dir = _dir;
	l = _l;
	w = _w;
	h = _h;
}

ofxVec3f ofxIndustrialRobotTool::getRotation(){
	ofxVec3f v;
	v.z = ofxVec3f(0.0, 1.0, 0.0).angle(dir);	
	return v;
}