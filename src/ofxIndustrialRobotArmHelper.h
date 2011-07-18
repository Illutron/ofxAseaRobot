#ifndef _INDUSTRIAL_ROBOT_ARM_HELPER
#define _INDUSTRIAL_ROBOT_ARM_HELPER

#include "ofxIndustrialRobotCoreData.h"
#include "ofxIndustrialRobotTool.h"
#include "ofxVectorMath.h"

class ofxIndustrialRobotArmHelper {
public:


	ofxIndustrialRobotArmHelper(ofxIndustrialRobotCoreData * _data);
	
	float getLength(int arm);
	float getRotationAngle(int arm);
	ofxVec3f getAxis(int arm);	
	ofxVec3f getOffset(int arm);	
//	float getRotationFromX(int arm);
	
	ofxVec3f getStartPosition(int arm);
	ofxVec3f getEndPosition(int arm);
	ofxVec3f getEndPosition(int arm, float * data);
	
	ofxVec3f getTargetDir();
	ofxVec3f getDir(int arm);
	ofxVec3f getDir(int arm, float * data);
	
	ofxIndustrialRobotTool * getTool();
	ofxIndustrialRobotCoreData * getCoreData();
	
	ofxIndustrialRobotCoreData * data;
};

#endif


