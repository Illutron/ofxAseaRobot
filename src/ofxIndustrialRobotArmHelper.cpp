#include "ofxIndustrialRobotArmHelper.h"

ofxIndustrialRobotArmHelper::ofxIndustrialRobotArmHelper(ofxIndustrialRobotCoreData * _data){
	data = _data;
}

float ofxIndustrialRobotArmHelper::getLength(int arm){
	return data->arms[arm].length;
}

float ofxIndustrialRobotArmHelper::getRotationAngle(int arm){
	return data->arms[arm].rotation;
}

ofVec3f ofxIndustrialRobotArmHelper::getAxis(int arm){
	return data->arms[arm].axis;
}

ofVec3f ofxIndustrialRobotArmHelper::getOffset(int arm){
	return data->arms[arm].offset;
}

ofVec3f ofxIndustrialRobotArmHelper::getDir(int arm){
	float dat[5];
	for(int i=0;i<5;i++){
		dat[i] = data->arms[i].rotation;
	}
	return  getDir(arm, dat);
}
ofVec3f ofxIndustrialRobotArmHelper::getDir(int arm, float * dat){
	ofVec3f dir= ofVec3f(0.0,1.0,0.0);
	if(arm>0){
		int m=4;
		if(arm <4)
			m= arm;
		for(int i=1;i<m;i++){
			//		if(data->arms[i].axis == ofVec3f(0.0,0.0,1.0)){
			dir.rotate(-dat[i], data->arms[i].axis);
			//		} else if(data->arms[i].axis == ofVec3f(0.0,1.0,0.0)){
			//			dir.rotate(-data->arms[i].rotation, dir);
			//		}
		}
	}
	ofVec3f dir2 = dir;
	if(arm > 4)
		dir.rotate(-90.0, ofVec3f(0.0,0.0,1.0));
	if(arm > 3){
		dir.rotate(dat[4], dir2);
	}
	dir.rotate(dat[0], data->arms[0].axis);
	return dir;
}


ofVec3f ofxIndustrialRobotArmHelper::getTargetDir(){
	ofVec3f dir= ofVec3f(0.0,1.0,0.0);
	for(int i=1;i<4;i++){
		dir.rotate(-data->arms[i].rotation, data->arms[i].axis);
	}
	ofVec3f dir2 = dir;
	dir.rotate(-90.0, ofVec3f(0.0,0.0,1.0));
	dir.rotate(data->arms[4].rotation, dir2);
	dir.rotate(data->arms[0].rotation, data->arms[0].axis);
	return dir;
	
}

ofVec3f ofxIndustrialRobotArmHelper::getEndPosition(int arm){
	float dat[5];
	for(int i=0;i<5;i++){
		dat[i] = data->arms[i].rotation;
	}
	ofVec3f v = getEndPosition(arm, dat);
	//scout<<v.x<<"  "<<v.y<<"  "<<v.z<<endl;
	return  v;
}
ofVec3f ofxIndustrialRobotArmHelper::getEndPosition(int arm, float * rotations){
	if(arm > 0){
		ofVec3f v , dir2;
		ofVec3f dir = ofVec3f(0.0,1.0,0.0);
		
		bool andTool = false;
		bool doArm4 = false;
		if(arm == 5){
			arm = 4;
			andTool = true;
		}
		for(int i=1;i<arm+1;i++){
			if(i < 4){
				dir.rotate(-rotations[i], data->arms[i].axis);
				v += dir* data->arms[i].length;// - data->arms[i].offset.rotated(90, ofVec3f(0,0,1)) ;
			} else if(i == 4){
				dir2 = dir;
				doArm4 = true;
			}
			
		}
		
		if(doArm4){
			dir.rotate(rotations[4], dir2);	
			v += dir* data->arms[4].length;// - data->arms[i].offset.rotated(90, ofVec3f(0,0,1)) ;
		}
		
		if(andTool){
			v+= dir* data->tool->h/2.0;  
			dir.rotate(-90.0, ofVec3f(0.0,0.0,1.0));
			dir.rotate(rotations[4], dir2);	
			v += dir* data->tool->l;
		} 
		dir.rotate(rotations[0], data->arms[0].axis);
//		v += ofVec3f(0.0,1.0,0.0)* data->arms[0].length - data->arms[0].offset.rotated(90, ofVec3f(0,0,1)) ;
//		v.rotate(rotations[0], data->arms[0].axis);
		v += ofVec3f(data->arms[0].offset.length() ,data->arms[0].length,0);
		v.rotate(rotations[0], data->arms[0].axis);
		return v;
		
	} else {
		ofVec3f ret = ofVec3f(data->arms[0].offset.length() ,data->arms[0].length,0).rotated(rotations[0], data->arms[0].axis);
		return ret;
	}
	
	
}
ofVec3f ofxIndustrialRobotArmHelper::getStartPosition(int arm){
	ofVec3f v , dir2;
	ofVec3f dir = ofVec3f(0.0,1.0,0.0);
	bool andTool = false;
	bool doArm4 = false;
	if(arm == 5){
		arm = 4;
		andTool = true;
	}
	for(int i=1;i<arm+1;i++){
		if(i < 4){
			dir.rotate(-data->arms[i].rotation, data->arms[i].axis);
			v += dir* data->arms[i].length;// - data->arms[i].offset.rotated(90, ofVec3f(0,0,1)) ;
		} else if(i == 4){
			dir2 = dir;
			doArm4 = true;
		}
		
	}
	
	if(doArm4){
		dir.rotate(data->arms[4].rotation, dir2);	
		v += dir* data->arms[4].length;// - data->arms[i].offset.rotated(90, ofVec3f(0,0,1)) ;
	}
	
	/*if(andTool){
	 v+= dir* data->tool->h/2.0;  
	 dir.rotate(-90.0, ofVec3f(0.0,0.0,1.0));
	 dir.rotate(data->arms[4].rotation, dir2);	
	 v += dir* data->tool->l;
	 }*/
	dir.rotate(data->arms[0].rotation, data->arms[0].axis);
	v += ofVec3f(0.0,1.0,0.0)* data->arms[0].length - data->arms[0].offset.rotated(90, ofVec3f(0,0,1)) ;
	v.rotate(data->arms[0].rotation, data->arms[0].axis);
	
	return v;
	
}

ofxIndustrialRobotTool * ofxIndustrialRobotArmHelper::getTool(){
	return data->tool;
}

ofxIndustrialRobotCoreData* ofxIndustrialRobotArmHelper::getCoreData(){
	return data;
	
}