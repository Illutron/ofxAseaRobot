/*
 *  FaceTracker.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "ofxCvHaarFinder.h"
#include "ofxVectorMath.h"
#include "Robot.h"


class FaceTracker : public BasePlugin {
	public:
	virtual ~FaceTracker();
	virtual void init(myData * _data,  QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	virtual void threadedFunction();

	ofxCvBlob *  getBiggestFace();
	void updateFrame();
		Robot * robot;

	
	
	myData * data;
	
	pthread_mutex_t  newdataMutex;
	ofxCvGrayscaleImage srcImgGrayFacial;
	ofxCvGrayscaleImage srcImgGrayFacialScaled;
	ofxCvHaarFinder	haarFinder;;
	bool newData;
	bool newFrame;
	vector<ofxCvBlob> blobs;
	ofxCvGrayscaleImage imgBiggestFace;
	ofxCvGrayscaleImage imgTmp;
	
	QSlider * sliderFaceThreshold;
	
	int lastCaptureTime;
	
protected:

	
	};
