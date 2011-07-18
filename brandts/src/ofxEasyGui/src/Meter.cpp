/*
 *  Meter.cpp
 *  openFrameworks
 *
 *  Created by Pat Long on 27/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Meter.h"

void UiMeter::setValue(float value){
	this->myValue = value;
}

void UiMeter::setMaxValue(float maxValue){
	this->myMaxValue = maxValue;
}
