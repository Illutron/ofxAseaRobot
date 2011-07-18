#pragma once
#include "Mode.h"

#include "VideoGrabber.h"

class ButtonMatrixMode : public Mode{
public:
	ButtonMatrixMode(Robot * robot, myData * data);
	
	void init();
	void activate();
	bool deactivate();
	void step();
	void securityCheck();

	
	float pressedTimer;
	
	bool foundMasterSafePos;
	bool foundToSafePos;	

	int curButton;
	
	QCheckBox *checkboxes[96];
	QPushButton * storePosition[3];
	QSlider * parSliders[3*3];
	
	QPushButton * allOn;
	QPushButton * allOff;

	QPushButton * resetFlags;
	
	float lightTurnedOn[96];
	
	float* positionInformation[3*3*2];
	
	ofxVec3f cornerPos(int n);
	ofxVec3f cornerDir(int n);
	ofxVec3f buttonPosition(int n);

	void pictureReady(bool points[]);
	
	
	ofxCvGrayscaleImage isLightOnImg;
	QSlider * sliderLightOnThreshold1;
	QSlider * sliderLightOnThreshold2;
	bool isLightOn();
	
	QGridLayout * gridLayout;
	
	bool isPointInBoundingbox(ofxVec3f p, float depth);
	
	ofxVec3f  xdir();
	ofxVec3f ydir();
	
	ofxIndustrialRobotDefines::HandVariant v;
	int lastButton;
	
	bool confirmedPress;
	int buttonRepeat;
	VideoGrabber * vidGrabber;

};