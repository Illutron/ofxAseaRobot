/*
 *  FaceTracker.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "PaperTracker.h"


void PaperTracker::init(myData * _data,  QVBoxLayout * boxLayout)
{
	
	data = _data;
	nameid = "PaperTracker";
	
	
	
	// GUI
	
	addBaseGui(boxLayout);
	
	QGroupBox * sliderArea = new QGroupBox("Motors");
	QGridLayout *sliderAreaLayout = new QGridLayout;
	boxLayout->addWidget(sliderArea,0,0);	
	
	sliderHughParam1 =  AddQSlider("Param 1", 10, 0, 100, sliderAreaLayout, 1, true);
	sliderHughParam2 =  AddQSlider("Param 2", 10, 0, 100, sliderAreaLayout, 2, true);
	sliderHoughthreshold = AddQSlider("Threshold", 10,1,100,sliderAreaLayout,3,true);
	sliderHoughTheta = AddQSlider("HoughTheta", 10,1,300,sliderAreaLayout,4,true);
	sliderHoughRho = AddQSlider("HoughRo",10,1,300,sliderAreaLayout,5,true);
	sliderLowerCanny = AddQSlider("Lower Canny",10,1,300,sliderAreaLayout,6,true);
	sliderHigherCanny = AddQSlider("Higher Canny",10,1,300,sliderAreaLayout,7,true);

	sliderAreaLayout->setAlignment(Qt::AlignTop);
	sliderArea->setLayout(sliderAreaLayout);	
	sliderArea->setMinimumWidth(460);
	
	
	
	
	
	// **********************
	
	

	grayImage.allocate(CAPWIDTH,CAPHEIGHT);
	grayImageScaled.allocate(CAPWIDTH,CAPHEIGHT);
	
	// EDGE DETECTER
	
	doCanny = true;
	lowerCanny = 10;
	higherCanny = 50;
	
	doHough = true;
	linesBeingUpdate = false;
	houghMethod = CV_HOUGH_PROBABILISTIC;
	houghRho= 1;
	houghTheta = 180;
	
	drawMonitor = false;
	
	doRectTracking = true;
	parAMargin = 1;
	
	rectAMargin = 1;
	
	anyLines = false;
	
	
	captureCounter = 0;
	capturedRect = new oRect();
	avgRect = new oRect();

}




void PaperTracker::update()
{
		grayImage = data->srcImgGray;
		grayImageScaled.scaleIntoMe(grayImage);
	
		
	
		canny(&grayImageScaled,sliderLowerCanny->value(),sliderHigherCanny->value());
		
		linesBeingUpdate = false;
		
		if(doHough){
			linesBeingUpdate = true;
			lines = HoughLines(&grayImageScaled,houghMethod, sliderHoughRho->value(),  CV_PI/sliderHoughTheta->value(), sliderHoughthreshold->value(), sliderHughParam1->value(),  sliderHughParam2->value());
			anyLines = true;
			if(doRectTracking){
				rectPoints.clear();
				
				pLines = findParalLines();
	
					if(pLines.size() > 1){
						
						//There are at least two parrallel groups (that have different angels)
						rectPoints =findRect(pLines);
						//					cout<<rectPoints.size()<<end
						if(rectPoints.size() == 4){
							//Yes, we found a rectangle!
							
							
							if(captureCounter == 0){
							
																
								int minN = -1;
								float minY = -1;
								for(int i=0;i<4;i++){
									if(minY == -1 || minY > rectPoints[i].y){
										minN = i;
										minY = rectPoints[i].y;
									}
								}							
								if(rectPoints[minN].x < rectPoints[(minN+1)%4].x) 
									avgRect = new oRect(rectPoints[minN],rectPoints[(minN+1)%4],rectPoints[(minN+2)%4],rectPoints[(minN+3)%4]); 
								else 
									avgRect = new oRect(rectPoints[minN],rectPoints[(minN+3)%4],rectPoints[(minN+2)%4],rectPoints[(minN+1)%4]); 
								
							} else {
								oRect r;
								for(int i=0;i<4;i++){
									float s = 0;
									int best = -1;
									for(int u=0;u<4;u++){
										//									cout<<"p "<<u<<" "<<rectPoints[u].x<<endl;
										if(best == -1 || avgRect->p[i].squareDistance(rectPoints[u]) < s){
											s = avgRect->p[i].squareDistance(rectPoints[u]);
											best = u;
										}
									}
									r.p[i] = rectPoints[best];
									
								}
								//							avgRect->add(r);
								if(fabs(avgRect->w() - r.w()) > 10 || fabs(avgRect->h() - r.h()) > 10 ){
									captureCounter =- 10*speed;
									cout<<"Abort"<<endl;
																
								}
							}
							captureCounter += 5.9*speed;
							
							
							
							
							
						} else {
							
						}
					}
				}
				else {
					//	cout<<"v"<<ofGetElapsedTimeMillis() - millis<<endl;
				}
			}
}

float PaperTracker::getA(CvPoint* line){
	return (float)(line[1].y - line[0].y)/(line[1].x - line[0].x);
}
float PaperTracker::getB(CvPoint* line){
	float a = getA(line);
	return line[0].y- a*line[0].x;
}

float PaperTracker::dist(int x1, int y1, int x2, int y2){
	return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

vector<vector<CvPoint*> > PaperTracker::findParalLines(){
	vector<vector<CvPoint*> > ret;
	bool * lineAdded = new bool[lines->total];
	for(int i=0;i<lines->total;i++){
		lineAdded[i] = false;
	}
	
	for(int i=0;i<lines->total;i++){
		if(lineAdded[i] == false){
			bool lineFound = false;
			CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
			float a1 = getA(line);
			ofxPoint2f p1a = ofxPoint2f(line[0].x, line[0].y);
			ofxPoint2f p1b = ofxPoint2f(line[1].x, line[1].y);
			ofxVec2f v1 = ofxVec2f((p1b-p1a).x, (p1b-p1a).y);
			ofxPoint2f p1bRot = p1b.rotated(atan(a1)/PI*180, p1a);
			
			
			for(int u=i+1;u<lines->total;u++){
				CvPoint* line2 = (CvPoint*)cvGetSeqElem(lines,u);			
				ofxPoint2f p2a = ofxPoint2f(line2[0].x, line2[0].y);
				ofxPoint2f p2b = ofxPoint2f(line2[1].x, line2[1].y);
				ofxVec2f v2 = ofxVec2f((p2b-p2a).x, (p2b-p2a).y);
				
				float a2 = getA(line2);			
				float angle1 = fabs(v1.angle(v2));
				float angle2 = fabs(v1.angle(-v2));
				float angle;
				if(angle1 < angle2)
					angle = angle1;
				else 
					angle = angle2;
				
				//cout<<angle<<endl;
				if(fabs(angle) < parAMargin  ){
					
					//					cout<<"a1: "<<a1<<" "<<atan(a1)/PI*180<<endl;
					ofxPoint2f p2bRot = p2b.rotated(atan(a1)/PI*180, p1a);
					//					cout<<"y: "<<fabs(p2bRot.y-p1bRot.y)<<endl;
					
					//if(fabs(p2bRot.y-p1bRot.y) > parMinDist && fabs(p2bRot.y-p1bRot.y) < parMaxDist ){
					if(!lineFound){
						lineFound = true;
						vector<CvPoint*> n;
						n.push_back(line);
						ret.push_back(n);
						lineAdded[i] = true;
					}
					ret[ret.size()-1].push_back(line2);
					lineAdded[u] = true;
					//	}
				}
			}
		}
	}
	return ret;
}

vector<ofxPoint2f> PaperTracker::findRect(vector<vector<CvPoint*> > l){
	vector<ofxPoint2f> r;
	r.clear();
	vector<vector<CvPoint*> > tmp;
	
	float bestAngle = -1;
	int bestN1;
	int bestN2;	
	for(int i=0;i<l.size();i++){
		float a1;
		for(int u=0;u<l[i].size();u++){
			a1 += atan(getA(l[i][u]))/PI*180;
		}
		a1 /= l[i].size();
		
		
		for(int u=i+1;u<l.size();u++){
			float a2;
			for(int p=0;p<l[u].size();p++){
				a2 += atan(getA(l[u][p]))/PI*180;
			}
			a2 /= l[u].size();
			//cout<<fabs(fabs(a2-a1)-90) <<endl;
			if(fabs(fabs(a2-a1)-90) < rectAMargin){
				if(fabs(fabs(a2-a1)-90) < bestAngle || bestAngle == -1){
					bestAngle = fabs(fabs(a2-a1)-90);
					tmp.clear();
					bestN1 = i;
					bestN2 = u;
					tmp.push_back(l[i]);
					tmp.push_back(l[u]);
				}				
			}
		}
		
	}
	//	cout<<bestN1<<" - "<<bestN2<<endl;
	//	cout<<"Number of candidate p groups: "<<tmp.size()<<endl;
	if(tmp.size() >= 2){
		
		double * la1 = new double[tmp[0].size()];
		double *lb1 = new double[tmp[0].size()];
		double *la2 = new double[tmp[1].size()];
		double *lb2 = new double[tmp[1].size()];
		for(int i=0; i<tmp[0].size(); i++){
			la1[i] = getA(tmp[0][i]);
			lb1[i] = getB(tmp[0][i]);
		}
		for(int i=0; i<tmp[1].size(); i++){
			la2[i] = getA(tmp[1][i]);
			lb2[i] = getB(tmp[1][i]);
		}
		vector<vector<ofxPoint2f> > tmp2; //All intersection points
		
		for(int i=0; i<tmp[0].size(); i++){
			vector<ofxPoint2f> tmp22 ;
			tmp2.push_back(tmp22);
			int n = tmp2.size()-1;
			for(int u=0; u<tmp[1].size(); u++){
				float x = (-(lb1[i]-lb2[u]))/(la1[i]-la2[u]);
				float y = la2[u]*x+lb2[u];
				tmp2[n].push_back(ofxPoint2f(x,y));	
			}
		}
		vector<oRect> rects;
		
		for(int i=0;i<tmp2.size()-1;i++){
			//All rows except last row - i
			for(int u=0;u<tmp2[i].size()-1;u++){
				//All cols except last - u
				//Go through all the next rows a cols
				for(int i2=i+1;i2< tmp2.size();i2++){
					for(int u2=u+1;u2< tmp2[i].size();u2++){
						rects.push_back(oRect(tmp2[i][u], tmp2[i][u2], tmp2[i2][u2], tmp2[i2][u]));
					}
				}
				
			}
		}
		
		//		cout<<"-------"<<endl;
		int bestRect = -1;
		float marginW = 8;
		float smallestW = 0;
		float minSize = 60;
		for(int i=0;i<rects.size();i++){
			//			cout<<rects[i].w()<<" x "<<rects[i].h()<<endl;
			if(fabs(rects[i].w()-rects[i].h()) < marginW){
				if(rects[i].w() > minSize){
					if(rects[i].w()<smallestW || bestRect == -1){
						bestRect = i; 
						smallestW = rects[i].w();
					}
				}
			}
		}
		//		cout<<"-------"<<endl;		
		
		if(bestRect != -1){
			for(int i=0;i<4;i++){
				r.push_back(ofxPoint2f(rects[bestRect].p[i].x,rects[bestRect].p[i].y));
			}
		}
		
	}
	
	return r;
}


void PaperTracker::threadedFunction(){

}

void PaperTracker::draw()
{
	//data->srcImgGray.draw(0,0);
		grayImageScaled.draw(0,0);
	glPushMatrix();
	
		ofEnableAlphaBlending();
		ofNoFill();
		ofSetColor(255, 255, 255);
	
		if(doHough && linesBeingUpdate){
			ofSetColor(255, 255, 255);	
			for(int i=0;i<lines->total;i++){
				CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
				ofLine(line[0].x, line[0].y, line[1].x, line[1].y);
				ofEllipse(line[0].x, line[0].y, 5,5);
			}
			ofSetColor(0, 255, 0);
			for(int i=0;i<pLines.size();i++){
				if(i ==0)
					glColor3d(0, 255, 0);
				else if(i == 1)
					glColor3d(0, 0, 255);
				else if(i == 2)
					glColor3d(255, 0, 255);
				else
					glColor3d(255, 255, 0);
				for(int u=0;u<pLines[i].size();u++){
					ofLine(pLines[i][u][0].x, pLines[i][u][0].y, pLines[i][u][1].x, pLines[i][u][1].y);
				}
			}
		}
		ofDisableAlphaBlending();
		if(rectPoints.size() > 0){
			for(int i=0;i<rectPoints.size();i++){
				ofSetColor(255, 0, 255);
				ofFill();
				ofEllipse(rectPoints[i].x, rectPoints[i].y, 20, 20);
			}
		}
		ofEnableAlphaBlending();
		glColor4d(0, 255, 255, 0.2);
		glBegin(GL_POLYGON);
		for(int u=0;u<4;u++){			
			glVertex2f(avgRect->p[u].x, avgRect->p[u].y);
		}
		glEnd();
		
		glColor4d(255, 255, 255, 0.2);
		glBegin(GL_POLYGON);
		for(int u=0;u<4;u++){			
			glVertex2f(capturedRect->p[u].x, capturedRect->p[u].y);
		}
		glEnd();

		
		glTranslated(400, 0, 0);
		ofSetColor(255, 255, 255);
	
	
		
	
	glPopMatrix();
	
	

}




void PaperTracker::canny(ofxCvGrayscaleImage * img, int lower, int higher){
	cvCanny(img->getCvImage(), img->getCvImage(), lower, higher);	

	
}


CvSeq* PaperTracker::HoughLines(ofxCvGrayscaleImage * img,int    method, double rho,  double theta, int    threshold, int param1, int param2){
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	
	
//	lines = cvHoughLines2(img->getCvImage(), storage,   CV_HOUGH_PROBABILISTIC, 1, 3*CV_PI/180, 35, 40, 10 );
	
	lines = cvHoughLines2(img->getCvImage(), storage, method, rho, theta, threshold, param1, param2);
	return lines;
	
};

