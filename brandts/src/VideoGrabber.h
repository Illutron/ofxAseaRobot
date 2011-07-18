/*
 *  VideoGrabber.h
 *  openframeworksQT
 *
 *  Created by mads hobye on 06/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "Robot.h"
#include "ofxIndustrialRobot.h"
#include "FaceTracker.h"
#include "of1394VideoGrabber.h"

class VideoGrabber : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	
	
	
	myData * data;
	FaceTracker * faceTracker;
	QSlider * sliderWhiteBalance1;
	QSlider * sliderWhiteBalance2;
	QSlider * sliderTemp;
	of1394VideoGrabber 	vidGrabberPoinGrey;
	ofVideoGrabber		vidGrabber;
	QPushButton * buttonSettings;
	//ofTexture			videoTexture;
	unsigned char * pixels;
	ofxCvColorImage tmpImg;


	void setExposure(int exp);
	
};
