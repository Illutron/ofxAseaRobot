/*
 *  VideoGrabber.cpp
 *  openframeworksQT
 *
 *  Created by mads hobye on 06/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "VideoGrabber.h"




void VideoGrabber::init(myData * _data, QVBoxLayout * boxLayout)
{
	
	addBaseGui(boxLayout);
	data = _data;
	nameid = "VideoGrabber";
	
	if(!USEPOINTGREY)
	{
		vidGrabber.initGrabber(CAPWIDTH,CAPHEIGHT);
		buttonSettings = new QPushButton("Settings");
		boxLayout->addWidget(buttonSettings);
	}
	else
	{
		
		QGroupBox * sliderArea = new QGroupBox("Threshold");
		QGridLayout *sliderAreaLayout = new QGridLayout;
		sliderArea->setLayout(sliderAreaLayout);
		boxLayout->addWidget(sliderArea);	
		sliderWhiteBalance1 = AddQSlider("White balance 1",512,0,1023,sliderAreaLayout,1,false);
		sliderWhiteBalance2 = AddQSlider("White balance 2",512,0,1023,sliderAreaLayout,2,false);
		sliderTemp = AddQSlider("Temp",1,0,5120,sliderAreaLayout,3,true);
		
		//of1394 grabber
		int vidError = 0;
		vidError = vidGrabberPoinGrey.initGrabber();
		if(vidError != 0)
		{
			//we have an error so exit
			printf("Could not initialise Camera correctly, quitting \n");
			vidGrabberPoinGrey.failedToInit = true;
		}
	
	/*	vidGrabberPoinGrey.setGamma(100);
		vidGrabberPoinGrey.setAutoExposure(true);
*/	
			setExposure(62);
		/*
		vidGrabberPoinGrey.setAutoShutter(true);
		vidGrabberPoinGrey.setShutter(100);*/
		tmpImg.allocate(640, 480);
		//vidGrabberPoinGrey.setWhiteBalance(120,120);
		pixels	= new unsigned char[vidGrabberPoinGrey.getWidth()*vidGrabberPoinGrey.getHeight()*3]; // x3 for RGB
	}
	
}

void VideoGrabber::setExposure(int exp){
	if(USEPOINTGREY && !vidGrabberPoinGrey.failedToInit) 
	{
		vidGrabberPoinGrey.setExposure(exp);	
	}
}

void VideoGrabber::update()
{
	if(USEPOINTGREY)
	{
		//vidGrabberPoinGrey.setWhiteBalance(sliderWhiteBalance1->value(), sliderWhiteBalance2->value());
//		vidGrabberPoinGrey.setTemperature(sliderTemp->value());
		if(!vidGrabberPoinGrey.failedToInit){
			if(vidGrabberPoinGrey.isFrameNew()){
				pixels = vidGrabberPoinGrey.getPixels();
				if(pixels!=NULL){
					vidGrabberPoinGrey.unlock();
					data->srcImgGrayLast = data->srcImgGray; //hack hack!
					tmpImg.setFromPixels(pixels,  vidGrabberPoinGrey.getWidth(),vidGrabberPoinGrey.getHeight());
					
					data->srcImg.scaleIntoMe(tmpImg);
					data->srcImgGray = data->srcImg;
					
					faceTracker->updateFrame();

				} 
			}
		
		}
	}
	else
	{
		if(buttonSettings->isDown()){
			vidGrabber.videoSettings();
			buttonSettings->setDown(false);
		}
	 
	
	 	vidGrabber.grabFrame();
		if (vidGrabber.isFrameNew())
		{
			data->srcImgGrayLast = data->srcImgGray;
			data->srcImg.setFromPixels(vidGrabber.getPixels(), CAPWIDTH,CAPHEIGHT);
			data->srcImgGray = data->srcImg;
			faceTracker->updateFrame();
		}
	}
}





void VideoGrabber::draw()
{
	if(USEPOINTGREY)
	{
		tmpImg.draw(0,0);
	}
	else
	{
		vidGrabber.draw(0,0);
	}
}