#ifndef _INDUSTRIAL_ROBOT_DATA
#define _INDUSTRIAL_ROBOT_DATA

#include "ofxIndustrialRobotCoreData.h"
#include "ofxIndustrialRobotArmHelper.h"
#include "ofxIndustrialRobotTimeline.h"
#include "ofxIndustrialRobotPlotter.h"
#include "ofxIndustrialRobotMotorConverter.h"
#include "ofxIndustrialRobotDefines.h"
#include "ofMain.h"



class ofxIndustrialRobotController {

public:

	ofxIndustrialRobotController(ofxIndustrialRobotCoreData * _data, ofxIndustrialRobotArmHelper * _helper, ofxIndustrialRobotTimeline * _timeline, ofxIndustrialRobotPlotter * _plotter);
	
	void step(float framerate);
	
	float time;
	
	ofVec3f targetPosition, lastTargetPosition;
	ofVec3f targetDir;
	
	ofVec3f vTarget;
	float getMaxToolDistance();
	
	int input;
	
//private:
	ofxIndustrialRobotCoreData * data;
	ofxIndustrialRobotArmHelper * helper;
	ofxIndustrialRobotTimeline * timeline;
		ofxIndustrialRobotPlotter * plotter;
	ofxIndustrialRobotMotorConverter * converter;


	void setHandToPosition(ofVec3f position, ofVec3f dir,  float variant = -1.0, float variant2 = 0.0);
	
	bool legalPosition(ofVec3f position, ofVec3f dir, float variant= 0.0, float variant2 = -1.0);
	
	bool getArmRotations(ofVec3f toolEnd, ofVec3f dir, float variant, float* ret, bool limitMotors = true, float variant2 = 0.0);
	bool getArmRotations2(ofVec3f position, double rotation1, double rotation2, float* ret, bool limitMotors = true, float variant2 = 0.0);

	void gotoResetPosition();
	
	bool addCue(float speed, ofVec3f target, ofVec3f dir, bool cubicSpline = true,  bool endPause=false, bool endInZeroSpeed=false);

//	float* calculateArmToBase(ofVec3f position, double rotation1, double rotation2);
//	float* calculateHandToArm(ofVec3f position, ofVec3f dir);
	
	void mousePressed(ofVec3f target);
	
	void setInput(int _input);
	
	ofxIndustrialRobotDefines::HandVariant variant;
	
	//Gravity
	ofVec3f gravityTarget, gravityTargetDir;
	ofVec3f gravityV, gravityVDir;
		ofVec3f gravityA, gravityADir;
	float gravityDirV;
	int mode;
	float gravityForce, gravityMaxSpeed;
	ofxIndustrialRobotDefines::HandVariant gravityVariant;
	bool variantLocked;
	
	int tmpInputVariant;
	bool changingVariant, changingVariant2;
	
	float axis4variant;
	float axis4varianttmp;
	int axis4variantgoal;
	
	bool inZoneSafetyMode;
	float safeRotationA, safeRotationV;
};

#endif
