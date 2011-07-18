/*
 *  MotionDetector.cpp
 *  openframeworksQT
 *
 *  Created by mads hobye on 14/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "MotionDetector.h"


void MotionDetector::init(myData * _data, QVBoxLayout * boxLayout)
{
	
	addBaseGui(boxLayout);
	data = _data;
	nameid = "MotionDetector";
	hor.allocate(CAPWIDTH,CAPHEIGHT);
	ver.allocate(CAPWIDTH, CAPHEIGHT);
	tmpImg.allocate(CAPWIDTH, CAPHEIGHT);
	motionImg.allocate(MOTIONWIDTH, MOTIONHEIGHT);
	lastImg.allocate(CAPWIDTH, CAPHEIGHT);
	lastCounter = 0;
	motionImgFadeOut.allocate(MOTIONWIDTH, MOTIONHEIGHT);
	motionImgFadeOut.set(0);
	
	vidGrabber.initGrabber(CAPWIDTH,CAPHEIGHT);
	buttonSettings = new QPushButton("Settings");
	boxLayout->addWidget(buttonSettings);
	srcImg.allocate(CAPWIDTH, CAPHEIGHT);
	srcImgGray.allocate(CAPWIDTH, CAPHEIGHT);
}

void MotionDetector::update()
{
	if(buttonSettings->isDown()){
		vidGrabber.videoSettings();
		buttonSettings->setDown(false);
		twitter.send("N7331227", "freddyisnice", "Jep jep");
	}
	
	
	vidGrabber.grabFrame();
	if (vidGrabber.isFrameNew())
	{
		
		srcImg.setFromPixels(vidGrabber.getPixels(), CAPWIDTH,CAPHEIGHT);
		srcImgGray =srcImg;
		motionImg.set(0);
		ofImage img;
		img.setFromPixels(vidGrabber.getPixels(), CAPWIDTH,CAPHEIGHT,true);
		img.saveImage("/brandtstest.jpg");
		cvCalcOpticalFlowLK(srcImgGray.getCvImage(), lastImg.getCvImage(), cvSize(3,3), hor.getCvImage(), ver.getCvImage());
	
		int widthStep = hor.width / MOTIONWIDTH;
		int heightStep = hor.height /MOTIONHEIGHT;

		for(int y=0;y<hor.height;y++)
		{
			float* ptrH = (float*) (hor.getCvImage()->imageData  + y * hor.getCvImage()->widthStep); 
			float* ptrV = (float*) (ver.getCvImage()->imageData  + y * ver.getCvImage()->widthStep); 
		
			for(int x=0;x< hor.width;x++)		
			{
				int value = MAX((float)getValueMotion((int)(x/widthStep),(int)(y/heightStep)) ,(fabs(ptrV[x])+fabs(ptrH[x]))*5);
				setValueMotion((int)(x/widthStep),(int)(y/heightStep), value);
			}
		}
		
		motionImg.threshold(200);
	
		for(int x=0;x<MOTIONWIDTH;x++)
		{
			for(int y=0;y<MOTIONHEIGHT;y++)
			{	
				float value = getValueMotion(x,y);
				float valueFadeOut = getValue(x,y);
			
				if(valueFadeOut < value)
				{
					setValue(x,y,255);
				}
				else
				{
					setValue(x,y, (int)valueFadeOut*0.99f);
				}
			}
		}
		motionImgFadeOut.flagImageChanged();

		lastCounter++;
		if(lastCounter > 50)
		{
			lastCounter =0;
			lastImg = srcImgGray.getCvImage();
		}
		
	}
	
}

void MotionDetector::setValueMotion(int x,int y,int value)
{
	uchar* ptr = (uchar*) (motionImg.getCvImage()->imageData + y * motionImg.getCvImage()->widthStep); 
	ptr[x] = value;
}

int MotionDetector::getValueMotion(int x,int y)
{
	uchar* ptr = (uchar*) (motionImg.getCvImage()->imageData + y * motionImg.getCvImage()->widthStep); 
	return ptr[x];
}

void MotionDetector::setValue(int x,int y,int value)
{
	uchar* ptr = (uchar*) (motionImgFadeOut.getCvImage()->imageData + y * motionImgFadeOut.getCvImage()->widthStep); 
	ptr[x] = value;

}

int MotionDetector::getValue(int x,int y)
{
	uchar* ptr = (uchar*) (motionImgFadeOut.getCvImage()->imageData + y * motionImgFadeOut.getCvImage()->widthStep); 
	return ptr[x];
}


bool MotionDetector::isAreaActive(int _x,int _y, int _width, int _height, float threshold)
{
	float sum=0;
	int count=0;
	for (int y = _y; y < _y + _height;y++)
	{
		uchar* ptr = (uchar*) (motionImgFadeOut.getCvImage()->imageData + y * motionImgFadeOut.getCvImage()->widthStep);
		for (int x = _x;x < _x+_width; x++)
		{
			sum += ptr[x];
			count++;
		}
		
	}
//	cout << "isAreaActive: " << (sum/count)/255 << "\n";
	return threshold < (sum/count)/255;
}

void MotionDetector::draw()
{
	//cout << isAreaActive(0, 0, 3, 2, 0.5) << "actuve \n";
	motionImg.draw(0, 0, 100, 100);
	motionImgFadeOut.draw(0,100,100,100);
	srcImg.draw(0, 200);
}