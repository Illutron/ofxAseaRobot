
#include "ofxIndustrialRobotPositionCue.h"


ofxIndustrialRobotPositionCue::ofxIndustrialRobotPositionCue(int _speed, ofVec3f _position, ofVec3f _hand, bool cubicSpline, bool _endPause, bool _endInZeroSpeed){
	position = _position;
	hand = _hand;
	speed = _speed;
	isPositionCue = true;
	isCubicSpline = cubicSpline;
	endPause = _endPause;
	endInZeroSpeed = _endInZeroSpeed;

}