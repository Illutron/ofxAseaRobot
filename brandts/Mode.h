#pragma once
#include "ofConstants.h"
#include "Robot.h"
#include "Data.h"
#include "RobotControllerItem.h"
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QTGui>
#include <QCheckBox>

class Mode {
public:
	RobotControllerItem * controllerItem;
	Mode();
	string name;
	myData * robotData;
	Robot * robot;
	
	QWidget * widget;
	
	float rate;
	
	virtual void init(){};
	virtual void activate(){};
	virtual void step(){};
	virtual bool deactivate(){};
	virtual bool isActive(){return controllerItem->isActive();};	
	virtual void securityCheck(){};

};