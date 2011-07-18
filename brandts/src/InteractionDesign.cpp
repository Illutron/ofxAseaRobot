/*
 *  InteractionDesign.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "InteractionDesign.h"
#include "ButtonMatrixMode.h"



void InteractionDesign::init(myData * _data, QVBoxLayout * _boxLayout)
{
	
	addBaseGui(_boxLayout);
	//	faceTrackingControllerItem = robot->addRobotController("FaceTracking", CONTROL_MODE_GRAVITY);
	controlPanelControllerItem = robot->addRobotController("ControlPanel", CONTROL_MODE_GRAVITY);
	
	data = _data;
	nameid = "Interaction";
	
	ofxVec3f target = ofxVec3f(40,420,290);
	if( !robot->industrialRobot->setGravityTarget(target, ofxVec3f(1.0,0.0,0.0))){
	}
	
	idlePosition = ofxVec3f(1460, 1560, 0);
	
	buttonPosition = ofxVec3f(1560.1, 844.1, 1063.7);
	buttonDir = ofxVec3f(0.73, -0.15, 0.67);
	
	button = new QPushButton("Press button");
	button->setCheckable(false);
	//boxLayout->addWidget(button);
	
	securityCheck = new QCheckBox("Security");
	boxLayout->addWidget(securityCheck);
	hookupWidget(securityCheck, "secuityCheck", TYPE_CHECKBOX);
	
	autoMode = new QCheckBox("Auto mode");
	boxLayout->addWidget(autoMode);
	
	startButton = new QPushButton("Start");
	boxLayout->addWidget(startButton);
	
	
	
	LightButtonMode* lightButtonMode = new LightButtonMode(robot, data);
	IdleMode* idleMode = new IdleMode(robot, data);
	KeyboardMode* keyboardMode = new KeyboardMode(robot, data);
	matrixMode = new ButtonMatrixMode(robot, data);
	DrawingMode* drawingMode = new DrawingMode(robot, data);
	faceMode = new FaceTrackerMode(robot, data);
	
	drawingMode->grabTheDrawing = grabTheDrawing;
	drawingMode->buttonMatrixMode = matrixMode;
	drawingMode->vidGrabber = vidGrabber;
	matrixMode->vidGrabber = vidGrabber;
	faceMode->vidGrabber = vidGrabber;
	faceMode->motionDetector = motionDetector;
	drawingMode->motionDetector = motionDetector;
	idleMode->motionDetector = motionDetector;
	modes.push_back(lightButtonMode);
	modes.push_back(idleMode);
	modes.push_back(keyboardMode);
	modes.push_back(matrixMode);
	modes.push_back(drawingMode);
	modes.push_back(faceMode);
	
	QGroupBox * sliderArea = new QGroupBox("Threshold");
	QGridLayout *sliderAreaLayout = new QGridLayout;
	sliderArea->setLayout(sliderAreaLayout);
	matrixMode->widget->layout()->addWidget(sliderArea);
	matrixMode->sliderLightOnThreshold1 = AddQSlider("Light on threshold 1",1,200,255,sliderAreaLayout,1,true);
	matrixMode->sliderLightOnThreshold2 = AddQSlider("Light on threshold 2",1,0,20000,sliderAreaLayout,2,true);
	
	
	QGroupBox * modeGroup = new QGroupBox("Modes");
	QGridLayout *vbox = new QGridLayout;
	
	QTabWidget * settingsTab = new QTabWidget();
	hookupWidget(settingsTab, "interactionDesignSettingsTab", TYPE_TAB_POSITION);
	
	for(unsigned int i=0;i<modes.size();i++){
		modeActivated.push_back(new QCheckBox(""));
		vbox->addWidget(modeActivated[i],i,0);
		
		vbox->addWidget(new QLabel(modes[i]->name.c_str()),i,1);
		
		modeProgressBars.push_back(new QProgressBar());
		modeProgressBars[i]->setRange(0,100);
		vbox->addWidget(modeProgressBars[i],i,2);
		
		modePushButtons.push_back(new QPushButton("Activate"));
		vbox->addWidget(modePushButtons[i],i,3);
		
		settingsTab->addTab(modes[i]->widget,modes[i]->name.c_str());
	}
	
	for(int i=0;i<3*3*2;i++){
		hookupValue(matrixMode->positionInformation[i], "matrixPosition"+ofToString(i, 0));
	}
	
	for(int i=0;i<3*3*2;i++){
		hookupValue(drawingMode->positionInformation[i], "drawingPosition"+ofToString(i, 0));
	}
	
	
	for(int i=0;i<9;i++){
		hookupWidget(matrixMode->parSliders[i], "parslider"+ofToString(i, 0), TYPE_SLIDER);
	}
	
	for(int i=0;i<96;i++){
		hookupWidget(matrixMode->checkboxes[i], "buttonmatrixCheck"+ofToString(i, 0), TYPE_TRISTATE_CHECKBOX);
		hookupValue(&matrixMode->lightTurnedOn[i], "buttonmatrixBool"+ofToString(i, 0));
	}
	
	modeGroup->setLayout(vbox);
	_boxLayout->addWidget(modeGroup);	
	_boxLayout->addWidget(settingsTab);
	currentMode = -1;
	
	startMode = false;
}


void InteractionDesign::update()
{
/*	if(startButton->isDown()){
		startMode = true;
		robot->industrialRobot->startPhase2 = false;
		robot->industrialRobot->startReady = false;
		startButton->setDown(false);
		robot->setControlMode(CONTROL_MODE_GRAVITY);
		robot->unlockMotors->setChecked(true);
		autoMode->setChecked(true);
	}
	if(startMode){
		if(!robot->industrialRobot->startReady){
			robot->industrialRobot->gotoStartPosition();
		} else {
			startMode = false;
		}
		
	}
	
*/
	
	if(robot->industrialRobot->startReady){
		if(!robot->industrialRobot->getEmergencyStop() &&!robot->industrialRobot->getLocked() ){
			//		cout<<"Step"<<endl;
			for(unsigned int i=0;i<modes.size();i++){
				modes[i]->step();		
			}
		}
		if(securityCheck->isChecked()){
			for(unsigned int i=0;i<modes.size();i++){
				modes[i]->securityCheck();		
			}
		}
		if(autoMode->isChecked()){
			int highestRateId = -1;
			float highestRate = 0;
			currentMode = -1;
			for(unsigned int i=0;i<modes.size();i++){
				modeProgressBars[i]->setValue(modes[i]->rate);
				if(highestRate<modes[i]->rate){
					highestRate = modes[i]->rate;
					highestRateId = i;
				}
				if(modes[i]->isActive())
					currentMode = i;
			}
			if(currentMode != highestRateId){
				if(!robot->initFlag){
					if(currentMode == -1 || modes[currentMode]->deactivate()){
						modes[highestRateId]->activate();
						currentMode = highestRateId;
					}
				}
			}
		}
	}
	/*
	 
	 ofxVec3f safeDist = buttonPosition + ofxVec3f(-30,0,-30);
	 
	 if(button->isDown()){
	 robot->setRobotController(controlPanelControllerItem);
	 } 
	 
	 
	 if(controlPanelControllerItem->isActive()){
	 if(button->isDown()){
	 robot->inputRadio[2]->setChecked(true);
	 
	 if(!foundToSafePos){
	 controlPanelControllerItem->locked = false;
	 robot->industrialRobot->setGravityTarget(safeDist, buttonDir, 0.6, 2.0, ofxIndustrialRobotDefines::Down);
	 if(robot->industrialRobot->isRobotReady(10)){
	 foundToSafePos = true;
	 }
	 } else {
	 controlPanelControllerItem->locked = true;
	 robot->industrialRobot->setGravityTarget(buttonPosition, buttonDir, 0.9, 3.0, ofxIndustrialRobotDefines::Down);
	 
	 }
	 } else {
	 foundToSafePos = false;
	 controlPanelControllerItem->locked = false;
	 if((robot->industrialRobot->getCurrentTarget() - buttonPosition).length() < 20.0){
	 controlPanelControllerItem->locked = true;
	 robot->industrialRobot->setGravityTarget(safeDist, buttonDir, 0.9, 3.0, ofxIndustrialRobotDefines::Down);
	 }
	 }
	 }
	 /*	list<bool*> robotControllersActive
	 for(list<BasePlugin*>::iterator it=plugins.begin(); it!=plugins.end(); it++)
	 {
	 (*it)->saveSettings(settings);
	 
	 }
	 */
	
	/*if(faceTrackingControllerItem->isActive())
	 {
	 updateFacetracking();
	 }
	 */
	if(currentMode > -1){
		if(!modes[currentMode]->controllerItem->isActive()){
			currentMode = -1;
		}
	}
	
	
}




void InteractionDesign::draw()
{
	matrixMode->isLightOnImg.draw(0, 0);
	
	
}