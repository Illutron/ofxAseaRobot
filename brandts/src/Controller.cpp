/*
 *  Controller.cpp
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Controller.h"

Controller::Controller(myData * _data,QMainWindow * tab,QComboBox * viewCombo){
	data = _data;
	InteractionDesign * tmpID = new InteractionDesign();
	FaceTracker *  tmpFaceTracker = new FaceTracker();

	DrawTestPattern * tmpPattern = new DrawTestPattern();
	VideoGrabber * tmpVideoGrabber = new VideoGrabber();
	//PaperTracker * tmpPaperTracker = new PaperTracker();
	GrabTheDrawing * tmpGrab = new GrabTheDrawing();
	MotionDetector * tmpMotionDetector = new MotionDetector();
	

	
	tmpRobot = new Robot();
	SoundInput * tmpSound = new SoundInput();

	Joystick * tmpJoystick = new Joystick();

	//Fiducials * tmpFiducials = new Fiducials();
	
	tmpID->motionDetector = tmpMotionDetector;
	tmpID->grabTheDrawing = tmpGrab;
	tmpPattern->robot = tmpRobot;
	tmpID->robot = tmpRobot;
	tmpFaceTracker->robot = tmpRobot;
	tmpVideoGrabber->faceTracker = tmpFaceTracker;
	tmpJoystick->robot = tmpRobot;
	tmpID->vidGrabber = tmpVideoGrabber;
	
	
	

	//tmpRobot->interactionDesign = tmpId;
	
	//addPlugin(tmpPaperTracker,tab,viewCombo);
	addPlugin(tmpRobot,tab, viewCombo);
	addPlugin(tmpFaceTracker,tab, viewCombo);
	addPlugin(tmpID,tab, viewCombo);
	addPlugin(tmpPattern, tab,viewCombo);
	addPlugin(tmpVideoGrabber,tab,viewCombo);
	addPlugin(tmpMotionDetector, tab, viewCombo);

	if(USE_SOUND_INPUT)
		addPlugin(tmpSound, tab, viewCombo);

	addPlugin(tmpJoystick,tab,viewCombo);
	addPlugin(tmpGrab,tab,viewCombo);

	tmpID->faceMode->faceTracker = tmpFaceTracker;
	//addPlugin(tmpFiducials, tab, viewCombo);
	
}

void Controller::saveSettings(QSettings * settings)
{
	
	for(vector<BasePlugin*>::iterator it=plugins.begin(); it!=plugins.end(); it++){
		(*it)->saveSettings(settings);
		
	}
}

void Controller::loadSettings(QSettings * settings)
{
	
	for(vector<BasePlugin*>::iterator it=plugins.begin(); it!=plugins.end(); it++){
		(*it)->loadSettings(settings);
		
	}
}

void Controller::update()
{	
	for(vector<BasePlugin*>::iterator it=plugins.begin(); it!=plugins.end(); it++){
		
		if((*it)->chkEnabled->isChecked() )
		{
			(*it)->update();
		}
	}	
}


void Controller::draw(int drawPlugin)
{	
	int i  = 0;
	for(vector<BasePlugin*>::iterator it=plugins.begin(); it!=plugins.end(); it++){
		if (i== drawPlugin)
		{
			if((*it)->chkEnabled->isChecked() )
			{
				(*it)->draw();
			}
			
		}
		i++;
	}	
}

void Controller::addPlugin(BasePlugin* plugin,QMainWindow * tab,QComboBox * viewCombo){
	plugins.push_back(plugin);
	
	
	QWidget *groupBox = new QWidget();
	QVBoxLayout *boxLayout = new QVBoxLayout(groupBox);
	groupBox->setLayout(boxLayout);
	boxLayout->setAlignment(Qt::AlignTop);
	
	plugin->init(data, boxLayout);
	//	tab->addTab(groupBox, plugin->nameid.c_str());
	viewCombo->addItem( plugin->nameid.c_str());
	
	
	
	QDockWidget *dockWidget = new QDockWidget(QString(plugin->nameid.c_str()), tab);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea |
								Qt::RightDockWidgetArea);
	dockWidget->setWidget(groupBox);
	dockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
	dockWidget->setObjectName(plugin->nameid.c_str());
	tab->addDockWidget(Qt::RightDockWidgetArea, dockWidget,Qt::Vertical);
	docks.push_back(dockWidget);
	if(docks.size() > 1){
		tab->tabifyDockWidget(docks[docks.size()-2], dockWidget);
	}



}

