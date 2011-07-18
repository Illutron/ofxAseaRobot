#ifndef _INDUSTRIAL_ROBOT_POSITION_CUE
#define _INDUSTRIAL_ROBOT_POSITION_CUE

#include "ofxIndustrialRobotCue.h"
#include "ofxVectorMath.h"

class ofxIndustrialRobotPositionCue : public ofxIndustrialRobotCue {
public:
	ofxVec3f position, hand;
	ofxIndustrialRobotPositionCue(int speed, ofxVec3f _position, ofxVec3f _hand=ofxVec3f(), bool cubicSpline = true, bool endPause=false, bool _endInZeroSpeed=false);	
	bool endPause;
	bool isCubicSpline;
	bool endInZeroSpeed;
};

#endif