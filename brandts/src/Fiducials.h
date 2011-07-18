/*
 *  Fiducials.h
 *  openframeworksQT
 *
 *  Created by mads hobye on 27/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once
#include "BasePlugin.h"
#include "Data.h"
#include "ofxFidMain.h"
#include "ofxVec2f.h"
#include "ofxVectorMath.h"

class Fiducials : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	void crop( const ofPoint& A, const ofPoint& B,
			  const ofPoint& C, const ofPoint& D );
	void draw_box(IplImage *image, CvBox2D box, CvScalar color=CV_RGB(0,255,128));
	float degrees (float radians); 
	myData * data;
	ofxFiducialTracker	fidfinder;
	
	ofxCvGrayscaleImage imgFiducial;
	ofxCvGrayscaleImage imgFiducialCrop;
	QSlider * sliderThreshold;
	QSlider * sliderTop;
	QSlider * sliderBottom;
	QSlider * sliderLeft;
	QSlider * sliderRight;
	
};