#ifndef _INDUSTRIAL_ROBOT_CORE_DATA
#define _INDUSTRIAL_ROBOT_CORE_DATA

#include "ofxIndustrialRobotArmData.h"
#include "ofxIndustrialRobotTool.h"
#include "ofxVectorMath.h"

class ofxIndustrialRobotCoreData {
public:
	ofxIndustrialRobotCoreData();
	
	ofxIndustrialRobotArmData arms[5];	
	ofxIndustrialRobotTool * tool;
	
	vector<ofxVec3f> armSpeedHistory;
	vector<float> armAccHistory;
	
	float reverseHeadPercent;
	float reverseHeadPercentTmp;
	float reverseHeadPercentGoal;
	
};

#endif