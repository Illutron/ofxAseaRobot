#ifndef _INDUSTRIAL_ROBOT_DATA
#define _INDUSTRIAL_ROBOT_DATA

#include "ofxVectorMath.h"
#include "ofxIndustrialRobotCoreData.h"
#include "ofxIndustrialRobotArmHelper.h"
#include "ofxIndustrialRobotTimeline.h"
#include "ofxIndustrialRobotPlotter.h"
#include "ofxIndustrialRobotMotorConverter.h"
#include "ofxIndustrialRobotDefines.h"
#include "ofMain.h"



class ofxIndustrialRobotController {

	
public:
	enum Input {
        Timeline,
        Slider, 
		Gravity,
		ManualPosition, 
		Test3
    };

	ofxIndustrialRobotController(ofxIndustrialRobotCoreData * _data, ofxIndustrialRobotArmHelper * _helper, ofxIndustrialRobotTimeline * _timeline, ofxIndustrialRobotPlotter * _plotter);
	
	void step(float framerate);
	
	float time;
	
	ofxVec3f targetPosition, lastTargetPosition;
	ofxVec3f targetDir;
	
	ofxVec3f vTarget;
	float getMaxToolDistance();
	
	int input;
	
//private:
	ofxIndustrialRobotCoreData * data;
	ofxIndustrialRobotArmHelper * helper;
	ofxIndustrialRobotTimeline * timeline;
		ofxIndustrialRobotPlotter * plotter;
	ofxIndustrialRobotMotorConverter * converter;


	void setHandToPosition(ofxVec3f position, ofxVec3f dir,  float variant = -1.0, float variant2 = 0.0);
	
	bool legalPosition(ofxVec3f position, ofxVec3f dir, float variant= 0.0, float variant2 = -1.0);
	
	bool getArmRotations(ofxVec3f toolEnd, ofxVec3f dir, float variant, float* ret, bool limitMotors = true, float variant2 = 0.0);
	bool getArmRotations2(ofxVec3f position, double rotation1, double rotation2, float* ret, bool limitMotors = true, float variant2 = 0.0);

	void gotoResetPosition();
	
	bool addCue(float speed, ofxVec3f target, ofxVec3f dir, bool cubicSpline = true,  bool endPause=false, bool endInZeroSpeed=false);

//	float* calculateArmToBase(ofxVec3f position, double rotation1, double rotation2);
//	float* calculateHandToArm(ofxVec3f position, ofxVec3f dir);
	
	void mousePressed(ofxVec3f target);
	
	void setInput(int _input);
	
	ofxIndustrialRobotDefines::HandVariant variant;
	
	//Gravity
	ofxVec3f gravityTarget, gravityTargetDir;
	ofxVec3f gravityV, gravityVDir;
		ofxVec3f gravityA, gravityADir;
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
