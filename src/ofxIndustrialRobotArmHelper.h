#ifndef _INDUSTRIAL_ROBOT_ARM_HELPER
#define _INDUSTRIAL_ROBOT_ARM_HELPER

#include "ofxIndustrialRobotCoreData.h"
#include "ofxIndustrialRobotTool.h"

class ofxIndustrialRobotArmHelper {
public:


	ofxIndustrialRobotArmHelper(ofxIndustrialRobotCoreData * _data);
	
	float getLength(int arm);
	float getRotationAngle(int arm);
	ofVec3f getAxis(int arm);	
	ofVec3f getOffset(int arm);	
//	float getRotationFromX(int arm);
	
	ofVec3f getStartPosition(int arm);
	ofVec3f getEndPosition(int arm);
	ofVec3f getEndPosition(int arm, float * data);
	
	ofVec3f getTargetDir();
	ofVec3f getDir(int arm);
	ofVec3f getDir(int arm, float * data);
	
	ofxIndustrialRobotTool * getTool();
	ofxIndustrialRobotCoreData * getCoreData();
	
	ofxIndustrialRobotCoreData * data;
};

#endif


