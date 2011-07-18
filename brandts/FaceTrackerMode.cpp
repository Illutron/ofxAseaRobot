#include "FaceTrackerMode.h"

FaceTrackerMode::FaceTrackerMode(Robot * _robot, myData * _data){
	name = "FaceTracker";
	robot = _robot;
	robotData = _data;
	rate = 0;
	idlePosition = ofxVec3f(1560, 1460, 0);
	
	maxX = 2100;
	minX = 1390;
	maxY = 1660;
	minY = 1170;
	maxZ = 750;
	
	init();
	noFaceCount = 0;
	lastCaptureTime = 0;
	randomCount = 0;
}

void FaceTrackerMode::init(){
	controllerItem = robot->addRobotController("FaceTracker", CONTROL_MODE_GRAVITY);
}
void FaceTrackerMode::activate(){
	robot->setRobotController(controllerItem);
	
	rate = rate + 10;
}
bool FaceTrackerMode::deactivate(){
	if(controllerItem->locked){
		return false;
	}
	return true;
}
void FaceTrackerMode::step(){
	rate += 0.01;
	
		if(rate > 50){
			rate = 30;
		}
	
	if(isActive()){
		
		/*		if( !robot->industrialRobot->setGravityTarget(idlePosition, idleDirection, 0.1, 1, ofxIndustrialRobotDefines::Up)){
		 cout<<"Could not add facetracker"<<endl;
		 }
		 */		
		updateFacetracking();
		
	}
	else
	{
		if(motionDetector->isAreaActive(5, 0, 5, 3, 0.2))
		{
			if(rate < 30 && rate > 10)
			{
				rate = 30;
			}
			rate +=0.04;

		}
	}
}

void FaceTrackerMode::updateFacetracking()
{
	//	if(robot->industrialRobot->timelineCuesLeft() < 2){
	/*		
	 
	 if(!(vec->x == 0 && vec->y==0 && vec->z==0) && !robot->industrialRobot->addPositionCue(80.0,  ofxVec3f(1169.0+ vec->x,700.824+ vec->y,-400.0), ofxVec3f(0.0,-1.0,0),false)){
	 cout<<"Could not add facetracker"<<endl;
	 }
	 }*/
	
	ofxCvBlob * blob =  faceTracker->getBiggestFace();
	noFaceCount ++;
	
	
	if(blob != NULL){
		noFaceCount -= 5;
		if(noFaceCount<0)
			noFaceCount = 0;
		float w = FACEWIDTH;
		float h = FACEHEIGHT;
		float factor = CAPWIDTH/FACEWIDTH;
		//		cout<< blob->boundingRect.width<<endl;
		//float x = 2200;
		
		/*		
		 float x =  robot->industrialRobot->getCurrentTarget().x - (blob->boundingRect.width-50)*5.0;
		 
		 ofxVec3f target = ofxVec3f(x, robot->industrialRobot->getCurrentTarget().y - (blob->centroid.y-h/2.0)*1.5, robot->industrialRobot->getCurrentTarget().z + (blob->centroid.x-w/2.0)*1.0);
		 if( !robot->industrialRobot->setGravityTarget(target, ofxVec3f(1.0,0.2,-0))){
		 //			cout<<"Could not add facetracker"<<endl;
		 }
		 */
		
		ofxVec3f curDir =  robot->industrialRobot->getCurrentDir();
		ofxVec3f curPos =  robot->industrialRobot->getCurrentTarget();
		float z = curDir.z + (blob->centroid.x-w/2.0)*0.002*factor;
		//cout<<z<<" "<<(blob->centroid.x-w/2.0)<<endl;
		float y = curDir.y - (blob->centroid.y-h/2.0)*0.002*factor;
		//		y = curDir.y
		ofxVec3f targetDir = ofxVec3f(0.9, y, z).normalized();
		
		ofxVec3f armDir = curPos.normalized();
		
		if(targetDir.y > 0.7)
			targetDir.y = 0.7;
		if(targetDir.y < -0.7)
			targetDir.y = -0.7;
		
		if(targetDir.z > 0.4+armDir.z)
			targetDir.z = 0.4+armDir.z;
		if(targetDir.z < -0.4+armDir.z)
			targetDir.z = -0.4+armDir.z;
		
		
		
		float tx =  robot->industrialRobot->getCurrentTarget().x - (blob->boundingRect.width-40)*2.0;		
		//		tx = (blob->boundingRect.width-40)*40 + 1800;
		tx = (maxX-300- robot->industrialRobot->getCurrentTarget().x)*0.1 + robot->industrialRobot->getCurrentTarget().x;
		
		ofxVec3f targetPosition = curPos;
		
		ofxVec3f target = targetPosition; // + targetDir.normalized()*robot->industrialRobot->getHandLength();
		target.x = tx;
		if(target.x > maxX){
			target.x = maxX;
		}
		if(target.x < minX){
			target.x = minX;
		}
		
		if(noFaceCount < 40){
			if(robot->industrialRobot->thread.lock()){
				if(robot->industrialRobot->thread.coreData.reverseHeadPercent < 0.2 || robot->industrialRobot->thread.coreData.reverseHeadPercent > 0.8){
					if( !robot->industrialRobot->setGravityTarget(target, targetDir, 0.1, 2, ofxIndustrialRobotDefines::Up,false)){
						cout<<"Could not add facetracker scale "<<target.x<<", "<<target.y<<",  "<<target.z<<"  -  "<<targetDir.x<<", "<<targetDir.y<<",  "<<targetDir.z<<endl;
					}
				}
				robot->industrialRobot->thread.unlock();
			}
		}
		//cout<<"face"<<endl;
		
	}
	else {
		
	}
	
	if(noFaceCount > 40){
		if(rate > 60){
			rate = 30;
		}	
		rate -=0.1;
		
		robot->industrialRobot->lockVariant(false);
		
		if(noFaceCount > 100){
			noFaceCount = 100;
		}
		//cout<<"No face"<<endl;
		
		/*
		 It is a nice try but quickly becoms jaggy - smoothness needed - mads
		 if(randomCount > 50)
		 {
		 idlePositionRandomized = idlePosition;
		 idlePositionRandomized.x = idlePositionRandomized.x + ofRandom(-30,0)*2;
		 randomCount =0;
		 }
		 randomCount++;*/ 
		
		
		//	idleDirection = ofxVec3f(2.0,-0.2,sin(ofGetElapsedTimeMillis()/5000.0)).normalized();
		if(robot->industrialRobot->isRobotDirReady(0.1)){
			idleDirection = ofxVec3f(2.0,ofRandom(-0.4, -0.05), ofRandom(-1.0, 1.0)).normalized();
			ofxVec3f offsetPos = ofxVec3f(ofRandom(-200,200), ofRandom(-200,400), ofRandom(-200,200));
			if( !robot->industrialRobot->setGravityTarget(idlePosition+offsetPos, idleDirection, 0.05, 0.4, ofxIndustrialRobotDefines::Up)){
				cout<<"Could not add facetracker"<<endl;
			}
		}
		
	} else {
		if(lastCaptureTime + 60000 < ofGetElapsedTimeMillis() || ofGetElapsedTimeMillis()+100 < lastCaptureTime){
			ofImage img;
			img.setImageType(GL_RGB);
			img.setFromPixels(faceTracker->imgBiggestFace.getPixels(), faceTracker->imgBiggestFace.width, faceTracker->imgBiggestFace.height, false);
			img.saveImage("/Brandts/facepositive"+ofToString(ofGetDay(),0)+"."+ofToString(ofGetMonth(),0)+"."+ofToString(ofGetYear(),0)+"  -  "+ofToString(ofGetMinutes(),0)+"-"+ofToString(ofGetMinutes(),0)+"-"+ofToString(ofGetSeconds(),0)+".jpg");
			
			lastCaptureTime = ofGetElapsedTimeMillis();
		}
		
		rate += 0.1;
		//cout << "motion tracker counting";
		
		
		if(rate > 70){
			rate = 40;
		}	
		
		robot->industrialRobot->lockVariant(true);
		
		//	cout<<"face1"<<endl;
		//	if(robot->industrialRobot->thread.lock()){
		ofxVec3f curTarget =  robot->industrialRobot->getCurrentTarget();
		ofxVec3f curDir =  robot->industrialRobot->getCurrentDir();
		
		float tz = curTarget.z  - ( curTarget.normalized().z-curDir.z)*90.0;;
		
		if(tz > maxZ){
			tz = maxZ;
		} 
		if(tz < -maxZ){
			tz = -maxZ;
		}
		
		float ty = curTarget.y  - ( -0.15-curDir.y)*200.0;;
		
		if(ty > maxY){
			ty = maxY;
		} 
		if(ty < minY){
			ty = minY;
		}
		
		ofxVec3f target = ofxVec3f(robot->industrialRobot->getCurrentGravityTarget().x,ty,tz);
		if( !robot->industrialRobot->setGravityTarget(target, robot->industrialRobot->getCurrentGravityDir(), 0.1, 1, ofxIndustrialRobotDefines::Up)){
			cout<<"Could not add facetracker  pos "<<robot->industrialRobot->getCurrentGravityTarget().x<<"   "<<ty<<"   "<<tz<<endl;
		}
		//	robot->industrialRobot->thread.unlock();
	}
	
	//}
	
	
}	