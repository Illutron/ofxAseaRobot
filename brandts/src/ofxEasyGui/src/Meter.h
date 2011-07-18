/*
 *  Meter.h
 *  openFrameworks
 *
 *  Created by Pat Long on 27/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef METER_H_INCLUDED
#define METER_H_INCLUDED

#include "ofMain.h"

#include "Slider.h"

class   UiMeter: public UiSlider
{
	
public:
	void setValue(float value);
	void setMaxValue(float maxValue);
	
	// we are a slider with mouse inputs disabled
	void drag(int x, int y){}
	void press(){}
	void release(){}
};

#endif
