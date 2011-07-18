/*
 *  FaceTracker.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "FaceTracker.h"





void FaceTracker::init(myData * _data,  QVBoxLayout * boxLayout)
{
	
	data = _data;
	haarFinder.setup("haarXML/haarcascade_frontalface_alt.xml");
	pthread_mutexattr_t att;
	pthread_mutexattr_settype(&att,PTHREAD_MUTEX_NORMAL);
	pthread_mutex_init(&newdataMutex, &att); 
	srcImgGrayFacial.allocate(CAPWIDTH,CAPHEIGHT);
	srcImgGrayFacialScaled.allocate(FACEWIDTH,FACEHEIGHT);
	srcImgGrayFacial.set(0);
	srcImgGrayFacialScaled.set(0);

	haarFinder.findHaarObjects(srcImgGrayFacialScaled, 10, 10, 1); // HACK ELSE IT CRASHES WEIRD??
	pthread_mutex_lock(&newdataMutex);
	newData = false;
	newFrame = false;
	startThread(true, false);   // blocking, verbose
	nameid = "FaceTracker";
	
	addBaseGui(boxLayout);
	imgBiggestFace.allocate(MATRIX_WIDTH,MATRIX_HEIGHT);
	imgBiggestFace.set(255);
	
	QGroupBox * sliderArea = new QGroupBox("Face picture");
	QGridLayout *sliderAreaLayout = new QGridLayout;
	sliderArea->setLayout(sliderAreaLayout);
	boxLayout->addWidget(sliderArea);	
	
	sliderFaceThreshold = AddQSlider("FaceThreshold",10,1,300,sliderAreaLayout,1,true);
	imgTmp.allocate(10, 10);
	
	lastCaptureTime = 0;
	
}

void FaceTracker::updateFrame()
{
	newFrame = true;
}

void FaceTracker::update()
{
	
	
	if(newFrame && !newData && pthread_mutex_lock(&newdataMutex) )
	{
		
		//		cvFlip(data->srcImg, srcImgGrayFacial, 0);
		srcImgGrayFacial = data->srcImg; 
		
		srcImgGrayFacialScaled.scaleIntoMe(srcImgGrayFacial);
		
		srcImgGrayFacial.setROI(100, 100,100,100);
		imgTmp.resetROI();
		
		//cvCopy(srcImgGrayFacial.getCvImage(), imgBiggestFace.getCvImage());
		imgTmp.scaleIntoMe(srcImgGrayFacial);
		srcImgGrayFacial.resetROI();
		//	cvNormalize(imgBiggestFace.getCvImage(),imgBiggestFace.getCvImage());
		imgTmp.threshold(sliderFaceThreshold->value(),false);
		
		
		if(robot->industrialRobot->thread.lock()){
			if(robot->industrialRobot->thread.coreData.reverseHeadPercent < 0.5){
				srcImgGrayFacialScaled.mirror(true, true);
			}
			robot->industrialRobot->thread.unlock();
		}
		newData = true;
		newFrame = false;
		pthread_mutex_unlock(&newdataMutex);
		
	}
	
	
	
	
	
}



void FaceTracker::threadedFunction(){
	while( isThreadRunning() != 0 ){
/*		if(newData && pthread_mutex_lock(&newdataMutex))
		{
			
			//		    cout << "tracking \n";
			haarFinder.findHaarObjects(srcImgGrayFacialScaled, 10, 10, 1);
			newData = false;
			if(lock())
			{
				blobs =  haarFinder.blobs;
				
				
				unlock();
			}
			
			
			pthread_mutex_unlock(&newdataMutex);
		}*/
		ofSleepMillis(10);
	}
}


ofxCvBlob *  FaceTracker::getBiggestFace()
{
	
	//Moved by jonas
	if(newData && pthread_mutex_lock(&newdataMutex))
	{
		
		//		    cout << "tracking \n";
		haarFinder.findHaarObjects(srcImgGrayFacialScaled, 10, 10, 1);
		newData = false;
		if(lock())
		{
			blobs =  haarFinder.blobs;
			
			
			unlock();
		}
		
		
		pthread_mutex_unlock(&newdataMutex);
	}
	
	
	if( lock() )
	{
		float size = 0;
		float x = 0;
		float y = 0;
		int numFace = blobs.size();
		if(numFace == 0){
			unlock();
			return NULL;
		}
		int biggestFace = -1;
		for(int i = 0; i < numFace; i++){
			float tmpSize = blobs[i].boundingRect.width * blobs[i].boundingRect.height;
			if (size < tmpSize)
			{
				//		x = blobs[i].centroid.x;
				//		y = blobs[i].centroid.y;
				size = tmpSize;
				biggestFace = i;
			}
			
		}
		unlock();
		if( biggestFace != -1)
		{
		/*	cout<<lastCaptureTime + 30000 <<"  "<< ofGetElapsedTimeMillis()<<endl;
			if(lastCaptureTime + 30000 < ofGetElapsedTimeMillis() || ofGetElapsedTimeMillis()+100 < lastCaptureTime){
				ofImage img;
				img.setFromPixels(imgBiggestFace.getPixels(), imgBiggestFace.width, imgBiggestFace.height, true);
				img.saveImage("/Brandts/FacePositive/facepositive "+ofToString(ofGetDay(),0)+"."+ofToString(ofGetMonth(),0)+"."+ofToString(ofGetYear(),0)+" "+ofToString(ofGetMinutes(),0)+":"+ofToString(ofGetMinutes(),0)+":"+ofToString(ofGetSeconds(),0));
				cout<<"Save"<<endl;
				
				lastCaptureTime = ofGetElapsedTimeMillis();
			}*/
			//srcImgGrayFacialScaled.setROI(blobs[biggestFace].centroid.x-  blobs[biggestFace].boundingRect.width, blobs[biggestFace].centroid.y- blobs[biggestFace].boundingRect.width,blobs[biggestFace].boundingRect.width*2,blobs[biggestFace].boundingRect.width*2);
			//srcImgGrayFacialScaled.resetROI();
			
			//cvCopy(srcImgGrayFacial.getCvImage(), imgBiggestFace.getCvImage());
			imgBiggestFace.scaleIntoMe(srcImgGrayFacialScaled);
			srcImgGrayFacialScaled.resetROI();
			//	cvNormalize(imgBiggestFace.getCvImage(),imgBiggestFace.getCvImage());
			//imgBiggestFace.threshold(sliderFaceThreshold->value(),false);
			
			return &blobs[biggestFace];
		}
		
		
		
	}
	
}

void FaceTracker::draw()
{
	
	if( lock() )
	{
		
		int numFace = blobs.size();

		ofBackground(0, 0, 0);
		if(numFace > 0){
			ofBackground(255, 0, 0);
		}
		
		srcImgGrayFacialScaled.draw(0,0);
		
		imgTmp.draw(100,300,100,100);
		
		imgBiggestFace.draw(0, 300,100,100);
		
		
		
		glPushMatrix();
		
		glTranslatef(0, 0, 0);
		
		for(int i = 0; i < numFace; i++){
			
			float x = blobs[i].boundingRect.x;
			float y = blobs[i].boundingRect.y;
			float w = blobs[i].boundingRect.width;
			float h = blobs[i].boundingRect.height;
			
			float cx = blobs[i].centroid.x;
			float cy = blobs[i].centroid.y;
			
			ofSetColor(0xFF0000);
			ofRect(x, y, w, h);
			
			ofSetColor(0xFFFFFF);
			ofDrawBitmapString("face "+ofToString(i), cx, cy);
			
		}
		
		glPopMatrix();
		unlock();
	}
}

FaceTracker::~FaceTracker(){ 
#ifndef TARGET_WIN32 
	pthread_mutex_destroy(&newdataMutex); 
#endif 
	
} 