/*
 *  defines.h
 *  openFrameworks
 *
 *  Created by mads hobye on 26/05/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#define CAPWIDTH 320
#define CAPHEIGHT 240
#define FACEHEIGHT 240

#define FACEWIDTH FACEHEIGHT*1.33

#define CONTROL_MODE_TIMELINE 0
#define CONTROL_MODE_MOTOR 1
#define CONTROL_MODE_GRAVITY 2
#define CONTROL_MODE_POS 3
#define MATRIX_WIDTH 100
#define MATRIX_HEIGHT 100
#define USE_SOUND_INPUT false
#define USEPOINTGREY true
#define LIGHTS_WIDTH 12
#define	LIGHTS_HEIGHT 8

#pragma once
template<typename T>
std::string toString(T t)
{
	std::stringstream s;
	s << t;

		return s.str();
}