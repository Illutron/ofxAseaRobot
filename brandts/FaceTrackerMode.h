#pragma once
#include "Mode.h"
#include "FaceTracker.h"
#include "VideoGrabber.h"
#include "MotionDetector.h"

class FaceTrackerMode : public Mode {
public:
	FaceTrackerMode(Robot * robot, myData * data);
	
	void init();
	void activate();
	bool deactivate();
	void step();
	
	ofxVec3f idlePosition;
	ofxVec3f idleDirection;
	
	
	void updateFacetracking();
	
	FaceTracker * faceTracker;
	int noFaceCount;
	MotionDetector * motionDetector;
	ofxVec3f idlePositionRandomized ;
	int randomCount;
	
	
	float maxX;
	float minX;
	float maxY;
	float minY;
	float maxZ;
	
	VideoGrabber * vidGrabber;
	int lastCaptureTime;
};