/*
 *  InteractionDesign.h
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "Robot.h"

#include "ofxIndustrialRobot.h"
#include "RobotControllerItem.h"

#include "Mode.h"
#include "LightButtonMode.h"
#include "IdleMode.h"
#include "KeyboardMode.h"
#include "DrawingMode.h"
#include "FaceTrackerMode.h"


#include "VideoGrabber.h"

class InteractionDesign : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	

//	ofxIndustrialRobot * industrialRobot;
	myData * data;
	Robot * robot;
	
	vector<Mode *> modes;	
	vector<QPushButton *> modePushButtons;
	vector<QProgressBar *> modeProgressBars;
	vector<QCheckBox *> modeActivated;
	int currentMode;
	
	
	
	FaceTrackerMode* faceMode;
	
	RobotControllerItem * faceTrackingControllerItem;
	RobotControllerItem * controlPanelControllerItem;
	GrabTheDrawing * grabTheDrawing;
	ButtonMatrixMode* matrixMode;
	MotionDetector * motionDetector;
	
	ofxVec3f idlePosition;
	ofxVec3f idleDirection;
	
	ofxVec3f buttonPosition;
		ofxVec3f buttonDir;
	
	QPushButton * button;
	QCheckBox * autoMode;
	bool foundToSafePos;	
	QCheckBox * securityCheck;
	QPushButton * startButton;
	
	VideoGrabber * vidGrabber;
	
	bool startMode;
};

