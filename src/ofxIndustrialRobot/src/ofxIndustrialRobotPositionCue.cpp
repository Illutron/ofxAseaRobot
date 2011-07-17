
#include "ofxIndustrialRobotPositionCue.h"


ofxIndustrialRobotPositionCue::ofxIndustrialRobotPositionCue(int _speed, ofxVec3f _position, ofxVec3f _hand, bool cubicSpline, bool _endPause, bool _endInZeroSpeed){
	position = _position;
	hand = _hand;
	speed = _speed;
	isPositionCue = true;
	isCubicSpline = cubicSpline;
	endPause = _endPause;
	endInZeroSpeed = _endInZeroSpeed;

}