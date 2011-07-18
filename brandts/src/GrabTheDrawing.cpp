/*
 *  SoundInput.cpp
 *  openframeworksQT
 *
 *  Created by mads hobye on 09/08/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "GrabTheDrawing.h"




void GrabTheDrawing::init(myData * _data, QVBoxLayout * boxLayout)
{
	
	addBaseGui(boxLayout);
	data = _data;
	nameid = "GrabTheDrawing";
	
	
	QGroupBox * sliderArea = new QGroupBox("Threshold");
	QGridLayout *sliderAreaLayout = new QGridLayout;
	sliderArea->setLayout(sliderAreaLayout);
	boxLayout->addWidget(sliderArea);	
	
	buttonGrab = new QPushButton("Grab");
	
	rawImg.allocate(CAPWIDTH,CAPHEIGHT);
	grabbedImg.allocate(CAPWIDTH, CAPHEIGHT);
	tmpGrayImg.allocate(CAPWIDTH, CAPHEIGHT);
	tmpGrayImg2.allocate(CAPWIDTH, CAPHEIGHT);
	tmpColorImg.allocate(CAPWIDTH, CAPHEIGHT);
	interpretatedImg.allocate(LIGHTS_WIDTH, LIGHTS_HEIGHT);
	interpretatedImgTmp1.allocate(LIGHTS_WIDTH, LIGHTS_HEIGHT);
	interpretatedImgTmp2.allocate(LIGHTS_WIDTH, LIGHTS_HEIGHT);

	calibrationImg.allocate(CAPWIDTH, CAPHEIGHT);
	
	grabbedImg.set(255);
	boxLayout->addWidget(buttonGrab);
	char reportString[255];
	
	for(int i =0; i < 4;i++)
	{
		sprintf(reportString, "Pos X %i", i);
		posSlidersX[i] = AddQSlider(reportString,10,1,CAPWIDTH,sliderAreaLayout,i*2,true);
		
		sprintf(reportString, "Pos Y %i", i);
		posSlidersY[i] = AddQSlider(reportString,10,1,CAPHEIGHT,sliderAreaLayout,(i)*2+1,true);
	}
	sliderPaperThreshold1 = AddQSlider("Paper threshold 1",1,0,300,sliderAreaLayout,9,true);
	sliderPaperThreshold2 = AddQSlider("Paper threshold 2",1,0,300,sliderAreaLayout,10,true);
	sliderInterpretationThreshold1 = AddQSlider("Interpretation threshold 1",1,0,300,sliderAreaLayout,11,true);
	sliderInterpretationThreshold2 = AddQSlider("Interpretation threshold 2",1,0,300,sliderAreaLayout,12,true);
	
	paperProbability = 0;
	tmpBool = false;

	

}

 

bool GrabTheDrawing::doGrab(bool byPassNewCheck)
{
	rawImg = data->srcImg;
	//GRAB THE IMAGE
	grabbedImg = rawImg;
	crop(
		 ofPoint(posSlidersX[0]->value(),posSlidersY[0]->value()),
		 ofPoint(posSlidersX[1]->value(),posSlidersY[1]->value()),
		 ofPoint(posSlidersX[2]->value(),posSlidersY[2]->value()),
		 ofPoint(posSlidersX[3]->value(),posSlidersY[3]->value())
		 );
	grabbedImg.mirror(false,true);
	cvEqualizeHist(tmpGrayImg.getCvImage(), tmpGrayImg.getCvImage());
	tmpGrayImg = grabbedImg;
	tmpGrayImg2= grabbedImg;

	tmpGrayImg = grabbedImg;
	
	tmpGrayImg.erode_3x3();
	tmpGrayImg.erode_3x3();
	tmpGrayImg.erode_3x3();
	
	tmpGrayImg.threshold(sliderPaperThreshold1->value(),false);
	paperProbability =(int)((cvSumPixels(tmpGrayImg.getCvImage())/255)/(CAPWIDTH*CAPHEIGHT)*100);
	
	if(sliderPaperThreshold2->value()< (int)paperProbability)
	{
		
	//	tmpGrayImg2.erode_3x3();
		tmpGrayImg2.erode_3x3();
		tmpGrayImg2.erode_3x3();
		tmpGrayImg2.threshold( sliderInterpretationThreshold1->value());
		
		interpretatedImgTmp1.scaleIntoMe(tmpGrayImg2,CV_INTER_LINEAR);
		interpretatedImgTmp1.threshold(sliderInterpretationThreshold2->value());
		interpretatedImgTmp2 = interpretatedImgTmp1;
		interpretatedImgTmp2.absDiff(interpretatedImg);
		//cout << "new?: " << cvSumPixels(interpretatedImgTmp2.getCvImage())/255 << "\n";
		if(byPassNewCheck)
		{
			return true;
		}
		
		else if (cvSumPixels(interpretatedImgTmp2.getCvImage())/255 > 25)
		{
			
			interpretatedImg = interpretatedImgTmp1;
		//	interpretatedImg.mirror(false, true);
			return true;
		}
		else
		{
			return false;
			cout << "Same picture";
		}
	}
	else
	{
		return false;
	}
	
		
}

bool * GrabTheDrawing::convertToBool()
{
	bool*  tmpBool = new bool[LIGHTS_WIDTH*LIGHTS_HEIGHT];
	int n = 0;
	for(int y=0;y< LIGHTS_HEIGHT;y++)
	{
		for(int x=0;x < LIGHTS_WIDTH;x++)
		{
		
			tmpBool[n] = getColor(x,y,interpretatedImg.getCvImage()) == 0;
			
			n++;
			cout << (getColor(x,y,interpretatedImg.getCvImage()) == 0);
		}
		cout << "\n";
	}
	
	return tmpBool;
}

void GrabTheDrawing::update()
{
	
	
	//doGrab(true);
	if(buttonGrab->isDown()){
		doGrab(true);
		buttonGrab->setDown(false);
	}
	
}



void GrabTheDrawing::draw()
{
	
	ofSetColor(255,255, 255);
	rawImg.draw(0,0);
	grabbedImg.draw(0,CAPHEIGHT);
	//calibrationImg.draw(CAPWIDTH,0,CAPWIDTH/2,CAPHEIGHT/2);
	tmpGrayImg.draw(CAPWIDTH,0, CAPWIDTH/2,CAPHEIGHT/2);
	tmpGrayImg2.draw(CAPWIDTH,300, CAPWIDTH/2,CAPHEIGHT/2);
	ofSetColor(0, 255, 0);
	ofFill();
	for(int i =0; i < 4;i++)
	{
		ofCircle(posSlidersX[i]->value(),posSlidersY[i]->value(), 4);
	}
	ofSetColor(255, 255, 255);
	char reportString[255];
	sprintf(reportString, "probability: %i\n", paperProbability);
	
	ofDrawBitmapString(reportString,20,CAPHEIGHT*2+10);
	

	interpretatedImg.draw(CAPWIDTH, 420);

	if(sliderPaperThreshold2->value()<(int)paperProbability)
	{
	// PRINT THE INTREPRETATED IMAGE AS CIRCLES
	for(int x = 0; x < LIGHTS_WIDTH;x++)
	{
		for(int y =0; y <LIGHTS_HEIGHT;y++)
		{
			if(getColor(x,y,interpretatedImgTmp1.getCvImage()) ==0)
			{
				ofSetColor(255,255,255);
			}
			else
			{
				ofSetColor(50,50,50);
			}
			ofCircle(CAPWIDTH+20+ 12*x,140+ 12*y,4);
		}
	}
	}
	
	ofSetColor(255, 255, 255);

	
//	grabbedImg.resize(CAPWIDTH, CAPHEIGHT);
	
}


void GrabTheDrawing::crop( const ofPoint& A, const ofPoint& B,
					 const ofPoint& C, const ofPoint& D )
{
	

	// compute matrix for perspectival warping (homography)
	CvPoint2D32f cvsrc[4];
	CvPoint2D32f cvdst[4];
	CvMat* translate = cvCreateMat( 3,3, CV_32FC1 );
	cvSetZero( translate );
	
	
	cvdst[0].x = 0;
	cvdst[0].y = 0;
	cvdst[1].x = rawImg.width;
	cvdst[1].y =  0;
	cvdst[2].x =rawImg.width;
	cvdst[2].y = rawImg.height;
	cvdst[3].x =  0;
	cvdst[3].y =rawImg.height;
	
	cvsrc[0].x = A.x;
	cvsrc[0].y = A.y;
	cvsrc[1].x = B.x;
	cvsrc[1].y = B.y;
	cvsrc[2].x = C.x;
	cvsrc[2].y = C.y;
	cvsrc[3].x = D.x;
	cvsrc[3].y = D.y;
	
	
	cvWarpPerspectiveQMatrix( cvsrc, cvdst, translate );  // calculate homography
	cvWarpPerspective( rawImg.getCvImage(), grabbedImg.getCvImage(), translate );
	
	cvReleaseMat( &translate );
	
	
}

int GrabTheDrawing::getColor(int x,int y, IplImage* img)
{
	uchar* ptr = (uchar*) (img->imageData + y * img->widthStep); 
	return ptr[x];
}
