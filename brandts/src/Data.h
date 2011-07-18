/*
 *  data.h
 *  openFrameworks
 *
 *  Created by mads hobye on 25/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 
 */
#pragma once
#include "ofxOpenCv.h"
#include "Defines.h"
#include "ofBaseApp.h"


class myData {
	
public:
	myData(ofBaseApp * _baseApp);
	ofxCvColorImage srcImg;
	ofxCvGrayscaleImage srcImgGray;
	ofBaseApp * baseApp;
	void setSrcImg(ofxCvColorImage _srcImg);
	float * soundLeft;
	float * soundRight;
	ofxCvGrayscaleImage srcImgGrayLast;
	bool newSound;

};