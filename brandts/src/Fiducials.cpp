/*
 *  Fiducials.cpp
 *  openframeworksQT
 *
 *  Created by mads hobye on 27/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Fiducials.h"



void Fiducials::init(myData * _data, QVBoxLayout * _boxLayout)
{
	
	addBaseGui(_boxLayout);
	
	data = _data;
	nameid = "Fiducials";
	imgFiducial.allocate(CAPWIDTH, CAPHEIGHT);
	
	QGroupBox * sliderArea = new QGroupBox("Threshold");
	QGridLayout *sliderAreaLayout = new QGridLayout;
	sliderArea->setLayout(sliderAreaLayout);
	boxLayout->addWidget(sliderArea);	
	
	sliderThreshold = AddQSlider("Threshold",10,1,300,sliderAreaLayout,1,true);
	sliderTop = AddQSlider("Top",0,1,500,sliderAreaLayout,2,true);
	sliderBottom = AddQSlider("Bottom",0,1,500,sliderAreaLayout,3,true);
	sliderLeft= AddQSlider("Left",0,1,500,sliderAreaLayout,4,true);
	sliderRight = AddQSlider("Right",0,1,500,sliderAreaLayout,5,true);
	imgFiducialCrop.allocate(CAPWIDTH, CAPHEIGHT);
	
	
	
}


void Fiducials::update()
{
	imgFiducial = data->srcImg;
	imgFiducial.adaptiveThreshold(sliderThreshold->value());
	fidfinder.findFiducials( imgFiducial );

}





void Fiducials::draw()
{
	imgFiducial.draw(0,0);
	for (list<ofxFiducial>::iterator fiducial = fidfinder.fiducialsList.begin(); fiducial != fidfinder.fiducialsList.end(); fiducial++) {
		if (fiducial->getId() ==  5)
		{
			fiducial->draw( 0, 0 );//draw fiducial
			//fiducial->drawCorners( 0, 0 );//draw corners
	
			
			//***************** CORNERS
			fiducial->computeCorners();
		
			ofSetColor(0, 255, 0);
			ofNoFill();
			glPushMatrix();
			glTranslatef(0, 0, 0);
			
			ofxVec2f a = ofxVec2f(fiducial->cornerPoints[0].x,fiducial->cornerPoints[0].y);
			ofxVec2f b = ofxVec2f(fiducial->cornerPoints[1].x,fiducial->cornerPoints[1].y);
			ofxVec2f c = ofxVec2f(fiducial->cornerPoints[2].x,fiducial->cornerPoints[2].y);
			ofxVec2f d = ofxVec2f(fiducial->cornerPoints[3].x,fiducial->cornerPoints[3].y);
			
			
			
			
			ofxVec2f A =a -(a-b)*sliderLeft->value()/100 -  (a-d)*sliderTop->value()/100;
			ofxVec2f B =b +(b-a)*sliderRight->value()/100 -  (b-c)*sliderTop->value()/100;
			ofxVec2f C =c +(c-d)*sliderRight->value()/100 -  (c-b)*sliderBottom->value()/100;
			ofxVec2f D =d -(d-c)*sliderLeft->value()/100 -  (d-a)*sliderBottom->value()/100;
			ofNoFill();
			ofCircle(A.x,A.y,4);
			
			ofCircle(B.x,B.y,4);
			ofCircle(C.x,C.y,4);
			ofFill();
			ofCircle(D.x,D.y,4);
			if (fiducial->cornerPoints.size() > 0) {
				for(int i = 3; i <4 ;i++) {
					ofCircle(fiducial->cornerPoints[i].x, fiducial->cornerPoints[i].y, 4);
					////printf("corner 0.x: %f corner 0.y %f\n", cornerPoints[i].x, cornerPoints[i].y);
				}
			}
			glPopMatrix();
			ofSetColor(255,255,255);
			
			if (fiducial->cornerPoints.size() > 0) {
				imgFiducialCrop = imgFiducial;
				crop(
					 ofPoint(A.x,A.y),
					  ofPoint(B.x,B.y),
					  ofPoint(C.x,C.y),
					  ofPoint(D.x,D.y)
					 );
				/*crop(
					 ofPoint(fiducial->cornerPoints[0].x,fiducial->cornerPoints[0].y),
					 ofPoint(fiducial->cornerPoints[1].x,fiducial->cornerPoints[1].y),
					 ofPoint(fiducial->cornerPoints[2].x,fiducial->cornerPoints[2].y),
					 ofPoint(fiducial->cornerPoints[3].x,fiducial->cornerPoints[3].y)
					 );*/
				//crop(ofPoint(0,0),ofPoint(100,0),ofPoint(100,100), ofPoint(0,100));
							
			}
	
			
			
			
			
		}
		
		
	}
	
	imgFiducialCrop.resize(10, 10);
	for(int y =0; y <10;y++)
	{
		float* ptr = (float*) (imgFiducialCrop.getCvImage()->imageData + y * imgFiducialCrop.getCvImage()->widthStep); 
		for(int x = 0; x < 10;x++)
		{
			if(ptr[x] < 200)
			{
				ofSetColor(255,255,255);
			}
			else
			{
				ofSetColor(50,50,50);
			}
			ofCircle(20+ 10*x,300+ 10*y,5);
		}
	}
	ofSetColor(255,255,255);
	imgFiducialCrop.draw(300,300,100,100);
	imgFiducialCrop.resize(CAPWIDTH, CAPHEIGHT);
	
	
	
}

float Fiducials::degrees (float radians) {
	return radians*180.0/PI;
}


void Fiducials::crop( const ofPoint& A, const ofPoint& B,
					  const ofPoint& C, const ofPoint& D )
{
	
	
	
	
	// compute matrix for perspectival warping (homography)
	CvPoint2D32f cvsrc[4];
	CvPoint2D32f cvdst[4];
	CvMat* translate = cvCreateMat( 3,3, CV_32FC1 );
	cvSetZero( translate );
	
	
	cvdst[0].x = 0;
	cvdst[0].y = 0;
	cvdst[1].x = imgFiducialCrop.width;
	cvdst[1].y =  0;
	cvdst[2].x =imgFiducialCrop.width;
	cvdst[2].y = imgFiducialCrop.height;
	cvdst[3].x =  0;
	cvdst[3].y =imgFiducialCrop.height;
	
	cvsrc[0].x = A.x;
	cvsrc[0].y = A.y;
	cvsrc[1].x = B.x;
	cvsrc[1].y = B.y;
	cvsrc[2].x = C.x;
	cvsrc[2].y = C.y;
	cvsrc[3].x = D.x;
	cvsrc[3].y = D.y;
	
	
	cvWarpPerspectiveQMatrix( cvsrc, cvdst, translate );  // calculate homography
	cvWarpPerspective( imgFiducial.getCvImage(), imgFiducialCrop.getCvImage(), translate );

	cvReleaseMat( &translate );

		
}
/*
void Fiducials::draw_box(IplImage *image, CvBox2D box, CvScalar color=CV_RGB(0,255,128)) 
{
	CvPoint2D32f boxPoints[4];
	
	/* CamShift seems to get this backwards 
	box.angle = -box.angle;
	
	cvBoxPoints(box, boxPoints);
	cvLine(image,
		   cvPoint((int)boxPoints[0].x, (int)boxPoints[0].y),
		   cvPoint((int)boxPoints[1].x, (int)boxPoints[1].y),
		   color);
	cvLine(image,
		   cvPoint((int)boxPoints[1].x, (int)boxPoints[1].y),
		   cvPoint((int)boxPoints[2].x, (int)boxPoints[2].y),
		   color);
	cvLine(image,
		   cvPoint((int)boxPoints[2].x, (int)boxPoints[2].y),
		   cvPoint((int)boxPoints[3].x, (int)boxPoints[3].y),
		   color);
	cvLine(image,
		   cvPoint((int)boxPoints[3].x, (int)boxPoints[3].y),
		   cvPoint((int)boxPoints[0].x, (int)boxPoints[0].y),
		   color);
}
*/