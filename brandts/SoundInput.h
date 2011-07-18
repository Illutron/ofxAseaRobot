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
#include "stdio.h"
#include "ofSoundStream.h"


class SoundInput : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();
	
	
	
	myData * data;
	
	void audioReceived 	(float * input, int bufferSize, int nChannels); 
	

	int 	bufferCounter;
	int 	drawCounter;
	float smoothVolume;
	float fastVolume;
	bool beat;
	
	QSlider	* sliderLower;
	QSlider * sliderHigher;
	
	
	
};
