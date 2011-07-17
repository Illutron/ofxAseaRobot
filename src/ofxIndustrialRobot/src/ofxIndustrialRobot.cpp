

#include "ofxIndustrialRobot.h"

ofxIndustrialRobot::ofxIndustrialRobot(ofBaseApp * _app){
	app = _app;
	thread.plotter = new ofxIndustrialRobotPlotter();
	thread.timeline = new ofxIndustrialRobotTimeline();
	thread.helper = new ofxIndustrialRobotArmHelper(&thread.coreData);
	thread.controller = new ofxIndustrialRobotController(&thread.coreData, thread.helper, thread.timeline, thread.plotter);
	thread.controller->converter = thread.converter;
	visualizer = new ofxIndustrialRobotVisualizer(thread.helper, thread.controller, _app, thread.timeline, thread.plotter);
	thread.serial = new ofxIndustrialRobotSerial();
	//thread.converter = thread.converter;
	//thread.converter->armdata = &thread.coreData;
	thread.start();
	
	startReady = true;
	startPhase2 = false;
	
}

void ofxIndustrialRobot::gotoStartPosition(){
	
	/*if(rotations[0] < -90){
	 rotations[0] = -110;
	 } else {
	 rotations[0] = -50;	
	 }*/
	if(thread.lock()){
		if(!getLocked() && !getEmergencyStop()){
			float rotations[5], rotationsTmp[5];
			for(int i=0;i<5;i++){
				rotations[i] = rotationsTmp[i] = thread.coreData.arms[i].rotation;							
			}
			if(startPhase2)
				rotations[0] += (0-rotations[0])*0.01;
			rotations[1] = -13.5	;//thread.converter->getMinRotation(1, rotations)+5;
			//							rotations[2] -= rotations[1] - rotationsTmp[1];
			rotations[2] = 119.0;
//			rotations[3] = 0;
//			rotations[4] = 0;
			//		rotations[3] -= rotations[2] - rotationsTmp[2];
			
			//						rotations[3] += 180;
			//						rotations[4] -= 180;
			for(int i=0;i<5;i++){
				thread.coreData.arms[i].rotation = rotations[i];							
			}						
			
			if(!setGravityTarget(thread.helper->getEndPosition(5), thread.helper->getTargetDir(), 0.05,0.5, ofxIndustrialRobotDefines::Up, false)){
				cout<<"Could not add start rotations point"<<endl;
			} else {
				cout<<"Goto start position"<<endl;
			}
			
			if(isRobotReady(10, false)){
				if(!startPhase2){
					startPhase2 = true;
				} else {
					startReady = true;
				}
			}
			
			for(int i=0;i<5;i++){
				thread.coreData.arms[i].rotation = rotationsTmp[i];							
			}
		}
		thread.unlock();
	}
	
}

bool ofxIndustrialRobot::isLegalTarget(ofxVec3f target, ofxVec3f dir, ofxIndustrialRobotDefines::HandVariant variant, bool lock){
	bool ok = false;
	if(!lock || thread.lock()){
		if(variant == ofxIndustrialRobotDefines::Auto){			
			if(thread.controller->legalPosition(target, dir, 0.0) || thread.controller->legalPosition(target, dir, 1.0)){
				ok = true;
			}
		} else if(variant == ofxIndustrialRobotDefines::Up){
			if(thread.controller->legalPosition(target, dir, 1.0)){
				ok = true;
			}
		} else if(variant == ofxIndustrialRobotDefines::Down){
			if(thread.controller->legalPosition(target, dir, 0.0)){
				ok = true;
			}
		}
		
		if(lock) thread.unlock();
		
	}
	return ok;
}

bool ofxIndustrialRobot::setGravityTarget(ofxVec3f target, ofxVec3f dir, float force, float maxSpeed, ofxIndustrialRobotDefines::HandVariant variant, bool lock){
	bool ret;
	if(!lock || thread.lock()){
		bool ok = false;
		if(variant == ofxIndustrialRobotDefines::Auto){
			if(thread.controller->legalPosition(target, dir, 0.0) || thread.controller->legalPosition(target, dir, 1.0)){
				thread.controller->variant = ofxIndustrialRobotDefines::Auto;
				ok = true;
			}
		} else if(variant == ofxIndustrialRobotDefines::Up){
			if(thread.controller->legalPosition(target, dir, 1.0)){
				thread.controller->variant = ofxIndustrialRobotDefines::Up;
				ok = true;
			}
		} else if(variant == ofxIndustrialRobotDefines::Down){
			if(thread.controller->legalPosition(target, dir, 0.0)){
				thread.controller->variant = ofxIndustrialRobotDefines::Down;
				ok = true;
			}
		}
		if(ok){
			thread.controller->gravityTarget = target;			
			thread.controller->gravityTargetDir = dir;			
			thread.controller->gravityForce = force;			
			thread.controller->gravityMaxSpeed = maxSpeed;			
			ret = true;
		} else {
			ret = false;
		} 
		if(lock) thread.unlock();
	}
	return ret;
}
ofxVec3f ofxIndustrialRobot::getCurrentTarget(bool lock){
	ofxVec3f ret;
	if(!lock || thread.lock()){
		ret = thread.controller->targetPosition;
		if(lock) thread.unlock();
	}
	return ret;
}
ofxVec3f ofxIndustrialRobot::getCurrentDir(bool lock){
	ofxVec3f ret;
	if(!lock || thread.lock()){
		ret = thread.controller->targetDir.normalized();
		if(lock) thread.unlock();
	}
	return ret;
}

ofxVec3f ofxIndustrialRobot::getCurrentGravityTarget(bool lock){
	ofxVec3f ret;
	if(!lock || thread.lock()){
		ret = thread.controller->gravityTarget;
		if(lock) thread.unlock();
	}
	return ret;
}
ofxVec3f ofxIndustrialRobot::getCurrentGravityDir(bool lock){
	ofxVec3f ret;
	if(!lock || thread.lock()){
		ret = thread.controller->gravityTargetDir;
		if(lock) thread.unlock();
	}
	return ret;
}



bool ofxIndustrialRobot::addPositionCue(float speed, ofxVec3f target, ofxVec3f dir, bool cubicSpline, bool endPause, bool endInZeroSpeed,bool lock){
	bool ret;
	if(!lock || thread.lock()){
		ret = true;
		if(!thread.controller->addCue(speed,  target,dir,cubicSpline,endPause,endInZeroSpeed)){
			ret = false;
			cout<<"Could not add positioncue"<<endl;
		}
		if(lock) thread.unlock();
	}
	return ret;
}

int ofxIndustrialRobot::timelineCuesLeft(bool lock){
	int ret;
	if(!lock || thread.lock()){
		ret = thread.timeline->numberPositionCues() - thread.timeline->currentCue;
		if(lock) thread.unlock();
	}
	return ret;
}
//---

void ofxIndustrialRobot::drawVisualizer(int x, int y, int w, int h){
	/*	if(thread.lock()){
	 ofDrawBitmapString("robot thread fps: "+ofToString(thread.framerate, 0), 15, 45);
	 
	 
	 thread.unlock();
	 }*/
}


float ofxIndustrialRobot::getHandLength(bool lock){
	return thread.coreData.tool->l;
}

float ofxIndustrialRobot::getHandHeight(bool lock){
	return thread.coreData.tool->h/2.0 + thread.coreData.arms[3].length +  thread.coreData.arms[4].length;
}

//---


ofxVec3f ofxIndustrialRobot::getDistanceToPointv(ofxVec3f p,bool lock){
	if(!lock || thread.lock()){
		ofxVec3f v = thread.controller->targetPosition; 
		if(lock) thread.unlock();
		return p - v;	
	} else {
		return NULL;	
	}
	
}
float ofxIndustrialRobot::getDistanceToPointf(ofxVec3f p,bool lock){
	return getDistanceToPointv(p,lock).length();
}

ofxVec3f ofxIndustrialRobot::getDistanceToGravityTargetv(bool lock){
	if(!lock || thread.lock()){
		ofxVec3f v = thread.controller->gravityTarget; 
		if(lock) thread.unlock();
		
		return getDistanceToPointv(v);
	}
}
float ofxIndustrialRobot::getDistanceToGravityTargetf(bool lock){
	return getDistanceToGravityTargetv(lock).length();
}

float ofxIndustrialRobot::getVariant(bool lock){
	if(!lock || thread.lock()){
		float v = thread.helper->data->reverseHeadPercent; 
		if(lock) thread.unlock();
		
		return v;
	}
}

ofxIndustrialRobotDefines::HandVariant ofxIndustrialRobot::getVariantGoal(bool lock){
	if(!lock || thread.lock()){
		float v = thread.helper->data->reverseHeadPercentGoal; 
		if(lock) thread.unlock();
		if(v == 1.0){
			return  ofxIndustrialRobotDefines::Up;	
		} else if( v== 0.0){
			return  ofxIndustrialRobotDefines::Down;	
		} else {
			return  ofxIndustrialRobotDefines::Auto;	
		}
		
	}
}

bool ofxIndustrialRobot::getLocked(){
	bool ret = false;
	if(thread.serial->lock()){
		if(thread.serial->connected){
			ret = thread.serial->lockFlag();	
		}
		thread.serial->unlock();
	}
	return ret;
}

bool ofxIndustrialRobot::getEmergencyStop(){
	bool ret = false;
	if(thread.serial->lock()){
		if(thread.serial->connected){
			ret = thread.serial->emergencyFlag();
		}
		thread.serial->unlock();
	}
	return ret;
}

bool ofxIndustrialRobot::isRobotReady(float radius,bool lock){
	//	cout<<isRobotDirReady(lock) <<"  "<< isRobotVariantReady(lock) <<"  "<< isRobotPositionReady(radius,lock) <<"  "<< isRobotFlagsReady()<<endl;
	if(isRobotDirReady(lock) && isRobotVariantReady(lock) && isRobotPositionReady(radius,lock) && isRobotFlagsReady()){
		return true;
	} else
		return false;
}

bool ofxIndustrialRobot::isRobotFlagsReady(){
	if(thread.serial->lock()){
		bool ret = false;
		if(thread.serial->connected){
			for(int i=0;i<5;i++){				
				if(thread.serial->motorFlag(i)){
					
				} else {
					ret = true;
				}	
			}
		}
		thread.serial->unlock();
		if(ret)
			return false;
	}
	return true;
}

bool ofxIndustrialRobot::isRobotDirReady(double margin, bool lock){
	bool ret = false;
	if(!lock || thread.lock()){
		ofxVec3f v = thread.controller->gravityTargetDir;
		ofxVec3f v2 = thread.controller->targetDir;
		v.normalize();
		v2.normalize();
		if(lock) thread.unlock();
		if((v-v2).length() < margin){
			ret = true;	
		}
	}	
	//	cout<<"Dir ready: "<<ret<<endl;
	return ret;
}

bool ofxIndustrialRobot::isRobotDirReady(bool lock){
	return isRobotDirReady(0.03, lock);
}

bool ofxIndustrialRobot::isRobotVariantReady(float margin, bool lock){
	bool ret = false;
	if(!lock || thread.lock()){
		float v = thread.coreData.reverseHeadPercent - thread.coreData.reverseHeadPercentGoal;
		float v2 = thread.controller->axis4variant -thread.controller->axis4variantgoal;
		
		if(lock) thread.unlock();
		if(v < margin && v > -margin && v2 < margin && v2 > -margin){
			ret = true;
		}
	}
	//		cout<<"Variant ready: "<<ret<<endl;
	return ret;	
}

bool ofxIndustrialRobot::isRobotVariantReady(bool lock){
	return isRobotVariantReady(0.01,lock);	
}
bool ofxIndustrialRobot::isRobotPositionReady(float radius,bool lock){
	bool ret = false;
	if(!lock || thread.lock()){
		ofxVec3f v = thread.controller->gravityTarget;
		ofxVec3f v2 = thread.controller->targetPosition;
		if(lock) thread.unlock();
		if((v-v2).length() < radius){
			ret = true;	
		}
	}	
	//		cout<<"Position ready: "<<ret<<endl;
	return ret;
}


void ofxIndustrialRobot::lockVariant(bool lock,bool threadlock){
	if(!threadlock || thread.lock()){
		thread.controller->variantLocked = lock;
		if(threadlock) thread.unlock();
	}	
}

bool ofxIndustrialRobot::willChangeVariant(bool lock){
	bool ret = true;
	if(!lock || thread.lock()){
		float v = thread.helper->data->reverseHeadPercentGoal - thread.helper->data->reverseHeadPercent; 
		float v2 = thread.controller->axis4variantgoal - thread.controller->axis4variant;
		cout<<v2<<"  "<<thread.controller->axis4variant<<"   "<<thread.controller->axis4variantgoal<<endl;
		if(lock) thread.unlock();
		if(v < 0.01 && v > -0.01 && v2 < 0.01 && v2 > -0.01){
			ret = false;
		}
	}	
	cout<<"ret "<<ret<<endl;
	return ret;
}


void ofxIndustrialRobot::panic(string msg){
	thread.panicMessage = msg;
	thread.panic = true;
}
//---
