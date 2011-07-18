/*
 *  RobotControllerItem.h
 *  openframeworksQT
 *
 *  Created by mads hobye on 07/07/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#pragma once
class RobotControllerItem {
	
public:
	bool locked;

	RobotControllerItem(){
		locked = false;
	}
	QRadioButton * radioButton;
	int mode;
	bool isActive()
	{ 
		return radioButton->isChecked();
	}
	
};

