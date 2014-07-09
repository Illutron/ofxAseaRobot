#ifndef _INDUSTRIAL_ROBOT
#define _INDUSTRIAL_ROBOT

#include "ofConstants.h"

#include "ofxIndustrialRobotThread.h"

#include "ofxIndustrialRobotVisualizer.h"
#include "ofxIndustrialRobotDefines.h"


class ofxIndustrialRobot {
public:

    enum Input {
        Timeline,
        Slider, 
		Gravity,
		ManualPosition
    };
    
	ofxIndustrialRobot(ofBaseApp * app);
	void drawVisualizer(int x, int y, int w, int h);
	
	ofxIndustrialRobotThread thread;
	ofxIndustrialRobotVisualizer * visualizer;
	ofBaseApp * app;
	
	bool addPositionCue(float speed, ofVec3f target, ofVec3f dir, bool cubicSpline = true,  bool endPause=false,  bool endInZeroSpeed=false,bool lock=true);
	int timelineCuesLeft(bool lock=true);
	bool setGravityTarget(ofVec3f target, ofVec3f dir, float force = 1.0, float maxSpeed = 0,  ofxIndustrialRobotDefines::HandVariant variant =  ofxIndustrialRobotDefines::Auto, bool lock=true);
	bool isLegalTarget(ofVec3f target, ofVec3f dir, ofxIndustrialRobotDefines::HandVariant variant =  ofxIndustrialRobotDefines::Auto, bool lock=true);
    
    void setInput(Input input);

	ofVec3f getCurrentTarget(bool lock=true);
	ofVec3f getCurrentDir(bool lock=true);
	
	ofVec3f getCurrentGravityTarget(bool lock=true);
	ofVec3f getCurrentGravityDir(bool lock=true);
	
	float getHandLength(bool lock=true);
	float getHandHeight(bool lock=true);

	ofVec3f getDistanceToPointv(ofVec3f p,bool lock=true);
	float getDistanceToPointf(ofVec3f p,bool lock=true);

	ofVec3f getDistanceToGravityTargetv(bool lock=true);
	float getDistanceToGravityTargetf(bool lock=true);

	float getVariant(bool lock=true);
	ofxIndustrialRobotDefines::HandVariant getVariantGoal(bool lock=true);

	
	bool isRobotReady(float radius = 1.0,bool lock=true);
	bool isRobotDirReady(bool lock=true);
	bool isRobotDirReady(double margin, bool lock=true);

	bool isRobotVariantReady(bool lock=true);
	bool isRobotVariantReady(float margin, bool lock=true);
	bool isRobotPositionReady(float radius = 1.0,bool lock=true);
	bool isRobotFlagsReady();
	bool getEmergencyStop();
	bool getLocked();
	
	//Locks the variant so it can't be changed
	void lockVariant(bool lock,bool threadlock=true);
	//Returns true if the robot would like to change the variant (also its locked)
	bool willChangeVariant(bool lock=true);
	void panic(string msg);
	
	void gotoStartPosition();
    
    //The position the robot is in after a reset
    void gotoResetPosition();
    
    ofxIndustrialRobotSerial * getSerial();
    
    void resetRobot();
    
	bool startReady;
	bool startPhase2;

};


#endif
