/*
 *  SoundInput.cpp
 *  openframeworksQT
 *
 *  Created by mads hobye on 09/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SoundInput.h"




void SoundInput::init(myData * _data, QVBoxLayout * boxLayout)
{
	
	addBaseGui(boxLayout);
	data = _data;
	nameid = "SoundInput";
	
		
	
	
	bufferCounter = 0;
	drawCounter = 0;
	smoothVolume = 0;
	fastVolume = 0;
	beat = false;
	QGroupBox * sliderArea = new QGroupBox("Threshold");
	QGridLayout *sliderAreaLayout = new QGridLayout;
	sliderArea->setLayout(sliderAreaLayout);
	boxLayout->addWidget(sliderArea);	
	
	sliderLower = AddQSlider("Lover",1,1,500,sliderAreaLayout,1,true);
	sliderHigher = AddQSlider("Higher", 1, 1, 500, sliderAreaLayout, 2, true);
}

void SoundInput::update()
{
	float tmpval = 0;	

	if (data->newSound)
	{
		for (int i = 0; i < 256; i++)
		{
			tmpval = data->soundLeft[i] * 100;
			if ( tmpval < 0)
			{
				tmpval = tmpval * -1;
			}
		
			smoothVolume = smoothVolume * 0.9999f + tmpval  * 0.0001f;
			fastVolume = fastVolume * 0.7f + tmpval * 0.3f;
		}
		data->newSound = false;
	}
	if(fastVolume > smoothVolume && !beat && smoothVolume > sliderHigher->value())
	{
		beat = true;
	}
	else if (fastVolume < smoothVolume && smoothVolume < sliderLower->value())
	{
		beat = false;
	}
	
	if (fastVolume > smoothVolume )
	{
		smoothVolume = fastVolume;
	}
}



void SoundInput::draw()
{
	
	// draw the left:
	ofSetColor(0x333333);
	ofRect(100,100,256,200);
	ofSetColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(100+i,200,100+i,200+data->soundLeft[i]*100.0f);


	}
	
	// draw the right:
	ofSetColor(0x333333);
	ofRect(600,100,256,200);
	ofSetColor(0xFFFFFF);
	for (int i = 0; i < 256; i++){
		ofLine(600+i,200,600+i,200+data->soundRight[i]*100.0f);
	}
	
    
	
	ofSetColor(0x333333);
	

	
	
	ofBackground(255 * beat, 0, 0);
	
	drawCounter++;
	char reportString[255];
	sprintf(reportString, "fastvolume: %f\n smooth volume: %f\n", fastVolume,smoothVolume);
	
	ofDrawBitmapString(reportString,80,380);
}


