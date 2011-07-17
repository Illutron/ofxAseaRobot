#include "ofxIndustrialRobotArmData.h"
ofxIndustrialRobotArmData::ofxIndustrialRobotArmData(){
	rotation = 0;
	length = 0;
	minRotation = 0;
	maxRotation = 0;
	axis = ofxVec3f();  //rotation axis
	offset = ofxVec3f(); //The offset added to the end of the arm. Has a length
	omax = 0.0, omin = 0.0;
	
	
}