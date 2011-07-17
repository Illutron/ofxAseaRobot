#include "ofxIndustrialRobotCoreData.h"

ofxIndustrialRobotCoreData::ofxIndustrialRobotCoreData(){
	arms[0].length = 800;
	arms[0].offset = ofxVec3f(00,130,0);
	arms[0].axis = ofxVec3f(0.0,1.0,0.0);
	arms[0].minRotation = -180;
	arms[0].maxRotation = 180;
	
	arms[1].length = 800;
	arms[1].axis = ofxVec3f(0.0,0.0,1.0);
	arms[1].minRotation = -14.5;
	arms[1].maxRotation = 46.7;
	
	arms[2].length = 1150;
	arms[2].axis = ofxVec3f(0.0,0.0,1.0);
	arms[2].minRotation = 87.5;
	arms[2].maxRotation = 141;

	arms[3].length = 100;
	arms[3].axis = ofxVec3f(0.0,0.0,1.0);
	arms[3].minRotation = -90;
	arms[3].maxRotation = 138.8;
	
	arms[4].length = 100;
	arms[4].axis = ofxVec3f(0.0,1.0,0.0);
	arms[4].minRotation = -177.2;
	arms[4].maxRotation = 248;
	arms[4].omin = -354.0;
	arms[4].omax = 476.0;

	
//	tool = new ofxIndustrialRobotTool(ofxVec3f(1.0,0.0,0.0),1, 150, -64);
	tool = new ofxIndustrialRobotTool(ofxVec3f(1.0,0.0,0.0), 290, 150, -64);
//	tool = new ofxIndustrialRobotTorol(ofxVec3f(1.0,0.0,0.0), 240, 150, 200);
//	tool = new ofxIndustrialRobotTool(ofxVec3f(1.0,0.0,0.0), 600, 150, 200);
	
	reverseHeadPercent = 0.0; reverseHeadPercentTmp = 0.0;
	reverseHeadPercentGoal = 0.0;
	
}