/*
 *  MotionDetector.h
 *  openframeworksQT
 *
 *  Created by mads hobye on 14/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

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
#include "ofxTwitter.h"
#define MOTIONWIDTH 10
#define MOTIONHEIGHT 10


class MotionDetector : public BasePlugin {
	void setValueMotion(int x,int y,int value);
	int getValueMotion(int x,int y);
public:
	void setValue(int x,int y,int value);
	int getValue(int x,int y);
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	ofVideoGrabber		vidGrabber;
	
	ofxCvGrayscaleImage motionImg;
	ofxCvGrayscaleImage motionImgFadeOut;

	ofxCvFloatImage hor;
	ofxCvFloatImage ver;
	ofxCvGrayscaleImage tmpImg;
	ofxTwitter twitter;

	myData * data;
	ofxCvGrayscaleImage lastImg;
	ofxCvColorImage srcImg;
	ofxCvGrayscaleImage srcImgGray;
	bool isAreaActive(int _x,int _y, int _width, int _height, float threshold);
	int lastCounter;
	QPushButton * buttonSettings;
	
	
	
};