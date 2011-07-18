/*
 *  Robot.h
 *  openFrameworks
 *
 *  Created by mads hobye on 09/06/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "BasePlugin.h"
#include "Data.h"
#include "ofxIndustrialRobot.h"
#include <QTableWidget>
#include <QLabel>
#include <QTGui>
#include <QDialog>

#include "RobotControllerItem.h"

class Robot : public BasePlugin {
public:
	virtual void init(myData * _data, QVBoxLayout * boxLayout);
	virtual void update();
	virtual void draw();


	virtual void threadedFunction();
	void setPosition(int speed, ofxVec3f vecArm, ofxVec3f vecHand);
	bool tryLock();
	bool isLocked(pthread_mutex_t * _mutex);
	ofxIndustrialRobot * industrialRobot;
	
	myData * data;
	
	QLabel *motorLabel[5];
	QLabel *motorLabel2[5];
	QSlider *motorSlider[5]; 
	QPushButton *setValue[5];
	QSlider *variantSlider; 



	QCheckBox *reverseHead;
	QCheckBox *pause;
	
	QLabel *motorStatusLabel[5];
	QLabel *panicStatus;
	QPushButton *resetMotors;
	QPushButton *unlockMotors;
	
	QDoubleSpinBox *manualPosition[6];
	
	int resetting;

	
	QPushButton *loadXmlButton;
	QCheckBox * byteone[8];
	QCheckBox * bytetwo[8];
	QCheckBox * bytestatus[8];
	
	bool lastReverseHead;
	
	QRadioButton *inputRadio[6];

	//int inputSet;
	int controlMode;
	void setControlMode(int mode);
	int  getControlMode();
	
	bool initFlag;
	
	QVBoxLayout * robotControllerLayout;
	QWidget * robotControllerTab;
	
	
	
	list<RobotControllerItem*> robotControllers;
	RobotControllerItem * addRobotController(char*  name, int mode);
	bool setRobotController(RobotControllerItem * controllerItem);
	RobotControllerItem * currentControllerItem;
	void tjeckCurrentRobotController();
	RobotControllerItem * manualControllerItem;
	
	QCheckBox * controllerItemLocked;
	
	bool warningShown;

	
};