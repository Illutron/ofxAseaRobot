#ifndef _INDUSTRIAL_ROBOT_TIMELINE
#define _INDUSTRIAL_ROBOT_TIMELINE

#include "ofxIndustrialRobotCue.h"
#include "ofxIndustrialRobotPositionCue.h"
#include "ofxMSASpline.h"


class ofxIndustrialRobotTimeline{
public:
	
	ofxMSASpline3D spline;
	ofxMSASpline3D handSpline;
	
	float splineTime;
	
	float lengthToNextCue;
	float avgSpeedReq;
	float a ;
	int nCuesInSpline, splineStart;

	
	unsigned long cueStartTime;
	
	void step(ofxVec3f v, float framerate);
	ofxVec3f getPosition();
	ofxVec3f getHandPosition();

	ofxVec3f getNextCuePosition();
	ofxVec3f getNextCueHandPosition();
	void clear();
	
	ofxIndustrialRobotTimeline();
	
	int numberCues();
	int numberPositionCues();
	int getLastPositionCue();
	
	void generateSpline(int first, int last);
	ofxIndustrialRobotPositionCue* getPositionCue(int n);
	vector<ofxIndustrialRobotCue*> cues;
	volatile int currentCue;
	
	bool isPaused;
	
	void play();
	void pause();

	
private:
	float currentCueMaxSpeed; //
	float fadeDownStartSpeed;
	unsigned long timeStartFadeDown;
	float tmp;
	int mode ;
	bool atSplineEnd;
};


#endif