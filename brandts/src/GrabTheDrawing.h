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



class GrabTheDrawing : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	
	bool doGrab();
	ofxCvColorImage  grabbedImg;
	ofxCvColorImage rawImg;
	ofxCvGrayscaleImage interpretatedImg;
	ofxCvGrayscaleImage interpretatedImgTmp1;
	ofxCvGrayscaleImage interpretatedImgTmp2;
	ofxCvGrayscaleImage calibrationImg;
	
	ofxCvColorImage tmpColorImg;
	ofxCvGrayscaleImage tmpGrayImg;
	ofxCvGrayscaleImage tmpGrayImg2;
	bool doGrab(bool byPassNewCheck);
	
	bool * convertToBool();
	QPushButton * buttonGrab;
	
	myData * data;
	
	QSlider * sliderPaperThreshold1;
	QSlider * sliderPaperThreshold2;
	QSlider * sliderInterpretationThreshold1;
	QSlider * sliderInterpretationThreshold2;
	QSlider * posSlidersX[4];
	QSlider * posSlidersY[4];
	bool tmpBool;

	

	

	int paperProbability;
	
	void crop( const ofPoint& A, const ofPoint& B,
			  const ofPoint& C, const ofPoint& D );
	int getColor(int x,int y, IplImage* img);
	
	
};


