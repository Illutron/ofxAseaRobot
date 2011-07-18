/*
 *  LightButtonMode.cpp
 *  openframeworksQT
 *
 *  Created by Jonas Jongejan on 27/07/09.
 *  Copyright 2009 HalfdanJ. All rights reserved.
 *
 */

#include "DrawingMode.h"


DrawingMode::DrawingMode(Robot * _robot, myData * _data){
	name = "DrawingMode";
	robot = _robot;
	robotData = _data;
	gotPicture = false;
	timer = 0;
	foundMasterSafePos = false;
	
	init();
	
	QVBoxLayout * tabLayout = new QVBoxLayout();
	widget->setLayout(tabLayout);
	
	
	QWidget * gridButton = new QWidget();
	QGridLayout * gridButtonLayout = new QGridLayout();
	gridButtonLayout->setAlignment(Qt::AlignTop);
	gridButtonLayout->setAlignment(Qt::AlignCenter);
	gridButton->setLayout(gridButtonLayout);
	gridButton->setFixedHeight(50);
	gridButton->setContentsMargins(0,0,0,0);
	
	tabLayout->addWidget(gridButton);
	
	lastCaptureTime = 0;
	storePosition[0] = new QPushButton("grab_pos");
	
	gridButtonLayout->addWidget(new QLabel("Store"), 0, 0);
	for(int i=0;i<1;i++){
		gridButtonLayout->addWidget(storePosition[i], 0, i+1);
	}
	for(int n=0;n<3*3*2;n++){
		positionInformation[n] = new float(0);
	}
	
	
}



void DrawingMode::init(){
	controllerItem = robot->addRobotController("drawingMode", CONTROL_MODE_GRAVITY);
}

void DrawingMode::activate(){
	robot->setRobotController(controllerItem);
	rate = rate + 30;
}
bool DrawingMode::deactivate(){
	if(controllerItem->locked){
		return false;
	}
	return true;
}

void DrawingMode::step(){
	rate +=0.03;
	
	
	for(int n=0;n<1;n++){
		if(storePosition[n]->isDown()){
			QMessageBox msgBox;
			msgBox.setText("You are about to change the coordinates of the buttons");
			msgBox.setInformativeText("Are you sure you want to do it?");
			msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			int reply = msgBox.exec();
			if (reply == QMessageBox::Yes){
				cout<<"Set"<<endl;
				
				*positionInformation[n*6] = robot->industrialRobot->getCurrentTarget().x;
				*positionInformation[n*6+1] = robot->industrialRobot->getCurrentTarget().y;
				*positionInformation[n*6+2] = robot->industrialRobot->getCurrentTarget().z;
				*positionInformation[n*6+3] = robot->industrialRobot->getCurrentDir().x;
				*positionInformation[n*6+4] = robot->industrialRobot->getCurrentDir().y;
				*positionInformation[n*6+5] = robot->industrialRobot->getCurrentDir().z;
				storePosition[n]->setDown(false);
			} else {
				storePosition[n]->setDown(false);
			}
		}
	}
	
	if(isActive()){	
	
		if (!gotPicture)
		{
			
			if((robot->industrialRobot->getCurrentTarget()-getPos(0)).length() > 500){ 	
				if(!robot->industrialRobot->setGravityTarget(getPos(0), getPos(0).normalized()*ofxVec3f(1.0,0.0,1.0)-ofxVec3f(0.4,0.1,0), 0.1, 2.0, ofxIndustrialRobotDefines::Up)){
					//cout<<(getPos(0).normalized()*ofxVec3f(1.0,0.0,1.0)).x<<"  "<<(getPos(0).normalized()*ofxVec3f(1.0,0.0,1.0)).y<<"  "<<(getPos(0).normalized()*ofxVec3f(1.0,0.0,1.0)).z<<endl;
					//cout<<"Could not add drawing mode pre target"<<endl;	
				}
			} else {
				if(!robot->industrialRobot->setGravityTarget(getPos(0), getDir(0), 0.1, 1.0, ofxIndustrialRobotDefines::Up)){
					cout<<"Could not add drawing mode target"<<endl;
				} else {
					if(robot->industrialRobot->isRobotReady(0.0001)){
						timer ++;
						if(timer > 30){
							cout<<"drawing mode ready for taking photo"<<endl;
							if(!gotPicture && grabTheDrawing->doGrab(false))
							{
								cout<<"Found picture"<<endl;
								buttonMatrixMode->pictureReady(grabTheDrawing->convertToBool());
								if(lastCaptureTime + 60000 < ofGetElapsedTimeMillis() || ofGetElapsedTimeMillis()+100 < lastCaptureTime){
									ofImage img;
									img.setImageType(GL_RGB);
									img.setFromPixels(grabTheDrawing->interpretatedImg.getPixels(), grabTheDrawing->interpretatedImg.width, grabTheDrawing->interpretatedImg.height, false);
									img.saveImage("/Brandts/picture small "+ofToString(ofGetDay(),0)+"."+ofToString(ofGetMonth(),0)+"."+ofToString(ofGetYear(),0)+"  -  "+ofToString(ofGetMinutes(),0)+"-"+ofToString(ofGetMinutes(),0)+"-"+ofToString(ofGetSeconds(),0)+".jpg");

									ofImage img2;
									img2.setImageType(GL_RGB);
									img2.setFromPixels(grabTheDrawing->tmpGrayImg2.getPixels(), grabTheDrawing->tmpGrayImg2.width, grabTheDrawing->tmpGrayImg2.height, false);
									img2.saveImage("/Brandts/picture "+ofToString(ofGetDay(),0)+"."+ofToString(ofGetMonth(),0)+"."+ofToString(ofGetYear(),0)+"  -  "+ofToString(ofGetMinutes(),0)+"-"+ofToString(ofGetMinutes(),0)+"-"+ofToString(ofGetSeconds(),0)+".jpg");
									
									
									
									lastCaptureTime = ofGetElapsedTimeMillis();
								}
								gotPicture = true;
							} else {
								cout<<"No picture found"<<endl;
								gotPicture = false;
							}
							rate = 0;
							timer = 0;
						}
						
					}	
				}
			}
			
			
		}
	}
	else
	{
		if(motionDetector->isAreaActive(0, 5, 4,5, 0.35))
		{
			if(rate< 50 && rate > 30)
			{
				rate = 50;
			}
			rate +=0.08;

		}
		gotPicture = false;
	}
	if (rate > 70)
	{
		rate = 40;
	}
		
}

ofxVec3f DrawingMode::getPos(int n){
	return ofxVec3f(*positionInformation[n*6], *positionInformation[n*6+1], *positionInformation[n*6+2] );
}
ofxVec3f DrawingMode::getDir(int n){
	//	cout<<*positionInformation[n*6]+3<<endl;
	//return ofxVec3f(-0,-0.3,1);
	return ofxVec3f(*positionInformation[n*6+3], *positionInformation[n*6+4], *positionInformation[n*6+5] );	
}

