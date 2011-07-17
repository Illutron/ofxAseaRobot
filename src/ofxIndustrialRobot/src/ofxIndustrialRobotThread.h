#pragma once

#include "ofMain.h"
#include "ofxThread.h"
#include "ofxVectorMath.h"

#include "ofxIndustrialRobotController.h"
#include "ofxIndustrialRobotTimeline.h"
#include "ofxIndustrialRobotSerial.h"
#include "ofxIndustrialRobotMotorConverter.h"
#include "ofxIndustrialRobotPlotter.h"

class ofxIndustrialRobotThread : public ofxThread {
public:
	float framerate;
	int lastTimeMillist;
	//--------------------------
	ofxIndustrialRobotThread();
	void start();
	
	void stop(){
		stopThread();
	}
	
	//--------------------------
	void threadedFunction();
	
	void step();
	
	ofxIndustrialRobotArmHelper * helper;
	ofxIndustrialRobotController * controller;
	ofxIndustrialRobotTimeline * timeline;
	ofxIndustrialRobotPlotter * plotter;
	ofxIndustrialRobotMotorConverter * converter;
	ofxIndustrialRobotSerial * serial;
	ofBaseApp * app;
	ofxIndustrialRobotCoreData coreData;
	
	bool sendToRobot;
	//Emergency stop on programming level. Can be activated from the code
	bool panic;
	string panicMessage;

private:
};