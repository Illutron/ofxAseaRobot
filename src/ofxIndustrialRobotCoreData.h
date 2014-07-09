#ifndef _INDUSTRIAL_ROBOT_CORE_DATA
#define _INDUSTRIAL_ROBOT_CORE_DATA

#include "ofxIndustrialRobotArmData.h"
#include "ofxIndustrialRobotTool.h"

class ofxIndustrialRobotCoreData {
public:
	ofxIndustrialRobotCoreData();
	
	ofxIndustrialRobotArmData arms[5];	
	ofxIndustrialRobotTool * tool;
	
	vector<ofVec3f> armSpeedHistory;
	vector<float> armAccHistory;
	
	float reverseHeadPercent;
	float reverseHeadPercentTmp;
	float reverseHeadPercentGoal;
	
};

#endif