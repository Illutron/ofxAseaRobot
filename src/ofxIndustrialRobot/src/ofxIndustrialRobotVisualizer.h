#ifndef _INDUSTRIAL_ROBOT_VISUALIZER
#define _INDUSTRIAL_ROBOT_VISUALIZER

#include "ofMain.h"
#include "ofConstants.h"

#include "ofxVectorMath.h"
#include "ofx3DUtils.h"

#include "ofxIndustrialRobotArmHelper.h"
#include "ofxIndustrialRobotController.h"
#include "ofxIndustrialRobotTimeline.h"
#include "ofxMSAInteractiveObject.h"
#include "ofx3DModelLoader.h"

class View : public ofxMSAInteractiveObject {
public:
	View(){
		enableMouseEvents();
		enableKeyEvents();	
		click = false;
		release = false;
	}
	bool click;
	bool release;
	virtual void onPress(int x, int y, int button) {
		click = true;
		release = false;
	}
	
	virtual void onRelease(int x, int y, int button) {
		click = false;
		release = true;
	}	
};

class ofxIndustrialRobotVisualizer {
public:
	ofxIndustrialRobotVisualizer(ofxIndustrialRobotArmHelper * _helper, ofxIndustrialRobotController * _controller, ofBaseApp * _app, ofxIndustrialRobotTimeline * _timeline, ofxIndustrialRobotPlotter * _plotter);
	
	void drawGeneric();
	void draw3d(int x, int y, int w, int h);
	void drawtop(int x, int y, int w, int h);
	void drawside(int x, int y, int w, int h);
	void drawGraphs(int x, int y, int w, int h);

	void drawArm(int i, int w, int h);
	void drawArm0();
	void drawArm1();
	void drawArm2();
	void drawArm3();
	void drawArm4();
	
	View topView, sideView;

	
	ofxIndustrialRobotArmHelper * helper;
	ofxIndustrialRobotController * controller;
	ofxIndustrialRobotTimeline * timeline;
	ofxIndustrialRobotPlotter * plotter;
	ofBaseApp * app;
	
	float rotationSpeed;
	
	ofxCamera camera;
	ofxLight light1;
	
	ofx3DModelLoader model;
	
};

#endif