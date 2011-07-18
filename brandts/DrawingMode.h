#pragma once
#include "Mode.h"
#include "GrabTheDrawing.h"
#include "ButtonMatrixMode.h"

#include "VideoGrabber.h"
#include "MotionDetector.h"

class DrawingMode : public Mode{
public:
	DrawingMode(Robot * robot, myData * data);
	
	void init();
	void activate();
	bool deactivate();
	void step();
	ButtonMatrixMode * buttonMatrixMode;
	float pressedTimer;
	
	bool foundMasterSafePos;
	MotionDetector * motionDetector;
	QPushButton * storePosition[3];

	bool gotPicture;
	float* positionInformation[3*3*2];
	
	GrabTheDrawing * grabTheDrawing;
	ofxVec3f getPos(int n);
	ofxVec3f getDir(int n);
	
	int timer;
	int lastCaptureTime;
	
		VideoGrabber * vidGrabber;
};