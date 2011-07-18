/*
 *  ofxIndustrialRobotMotorConverter.cpp
 *  openframeworksQT
 *
 *  Created by Jonas Jongejan on 03/07/09.
 *  Copyright 2009 HalfdanJ. All rights reserved.
 *
 */

#include "ofxIndustrialRobotMotorConverter.h"

void ofxIndustrialRobotMotorConverter::loadXml(){
	XML = new ofxXmlSettings();
	if( XML->loadFile("irb60.xml") ){
		cout<<"irb60.xml loaded!"<<endl;
		for(int i=0;i<5;i++){
			XML->pushTag("AXIS", i);
			multFactor[i] = XML->getValue("MULTFACTOR", 0.0, 0);
			addFactor[i] = XML->getValue("ADDFACTOR", 0.0, 0);
			lengtha[i] = XML->getValue("LENGTHA", 0.0, 0);
			lengthb[i] = XML->getValue("LENGTHB", 0.0, 0);
			angleAdd[i] = XML->getValue("ANGLEADD", 0.0, 0);
			XML->popTag();
		}
		
	} else{
		cout<<"unable to load irb60.xml check data/ folder"<<endl;
		multFactor[0] = 1.0;
		addFactor[0] = 0.0;
		multFactor[1] = 2.0;
		addFactor[1] = 0.0;
		multFactor[2] = 1.0;
		addFactor[2] = 0.0;
		multFactor[3] = 1.0;
		addFactor[3] = 0.0;
		multFactor[4] = 1.0;
		addFactor[4] = 0.0;
	}	
	
}

ofxIndustrialRobotMotorConverter::ofxIndustrialRobotMotorConverter(){
	loadXml();	

}

float ofxIndustrialRobotMotorConverter::convertO(int n, float val, float values[5]){
	float ret = 0 ;
	if(n == 0)
		ret = 180.0*((val) / multFactor[n])/PI;
	else if(n == 1){
		float a = lengtha[n];
		float b = lengthb[n];
		float l = val;
		l /= (8.0*PI/32.0);
		l += addFactor[n];
		float r = acos((-l*l+b*b+a*a)/(2.0*a*b))*360.0/TWO_PI - angleAdd[n]-25.8794632531;
		ret = r;
	} else if(n == 2){
		float a = lengtha[n];
		float b = lengthb[n];
		float l = val;
		l /= (8.0*PI/32.0);
		l += addFactor[n];
		float r = acos((-l*l+b*b+a*a)/(2.0*a*b))*360.0/TWO_PI - values[1] -angleAdd[n]-25.8794632531;
		ret = r;
	} else if(n == 3){
		val += addFactor[n];
		val /= multFactor[n];
		ret = 180.0*val/PI-values[1] - values[2]+90.0;
	} else if (n== 4){
	//	val -= 4.7;
	//	val -= 4.7;
		val -= addFactor[n];
		val += (values[3] +values[1] + values[2]-90.0) * multFactor[3]*(PI/180.0);
		val /= multFactor[4];
		ret = 180.0*val/PI;	
	}
	
	return ret;	
}

float ofxIndustrialRobotMotorConverter::convertO(int n,float val){
	float values[5];
	for(int i=0;i<5;i++){
		values[i] = armdata->arms[i].rotation;
	}
	return convertO(n,val,values);
}

float ofxIndustrialRobotMotorConverter::convertRotation(int n, float val, float values[5]){
	float ret = 0 ;
	if(n == 0)
		ret = multFactor[n]*PI*val/180.0;
	else if(n == 1){
		float a = lengtha[n];
		float b = lengthb[n];
		float l = sqrt(a*a + b*b - 2.0*a*b*cos(((val+angleAdd[n]+25.8794632531)/360.0)*TWO_PI));
		l -= addFactor[n];
		l *= 8.0*PI/32.0;
		ret = l;
	} else if(n == 2){
		float a = lengtha[n];
		float b = lengthb[n];
		float l = sqrt(a*a + b*b - 2.0*a*b*cos(((val+angleAdd[n]+25.8794632531+values[1])/360.0)*TWO_PI));
		l -= addFactor[n];
		l *= 8.0*PI/32.0;
		ret = l;
	} else if(n == 3){
		ret = PI*(val + values[1] + values[2] -90.0)/180.0 * multFactor[n] - addFactor[n];
	
	} else if (n== 4){
		ret = multFactor[4] * PI*(val)/180.0 - multFactor[3]*PI*(values[1] + values[2] + values[3] - 90.0) / 180.0 + addFactor[n];
	}
	
	return ret;	
	
}

float ofxIndustrialRobotMotorConverter::convertRotation(int n,float val){
	float values[5];
	for(int i=0;i<5;i++){
		values[i] = armdata->arms[i].rotation;
	}
	return convertRotation(n,val,values);
}

float ofxIndustrialRobotMotorConverter::convertRotation(int n){
	return convertRotation(n, armdata->arms[n].rotation);
}

float ofxIndustrialRobotMotorConverter::getMinRotation(int n, float v[5]){
	float ret;
	switch (n) {
		case 0:
			return armdata->arms[n].minRotation;
			break;
		case 1:
			return armdata->arms[n].minRotation;
			break;
		case 2:
			return armdata->arms[2].minRotation- fmod(v[1],360);
			break;
		case 3:
			ret = armdata->arms[3].minRotation - (v[1] + v[2] -90);
			if(ret < -117.0)
				ret = -117.0;
			return ret;
			break;
		case 4:
			return ((armdata->arms[4].omin- addFactor[n])+(v[1] + v[2] + v[3] -90)*multFactor[3]*(PI/180))/(multFactor[4]*(PI/180));
			break;
		default:
			return 0;
			break;
	}
}
float ofxIndustrialRobotMotorConverter::getMaxRotation(int n, float v[5]){
	float retu;
	float ret;
	switch (n) {
		case 0:
			return armdata->arms[n].maxRotation;
			break;
		case 1:
			return armdata->arms[n].maxRotation;
			break;
		case 2:
			retu = armdata->arms[2].maxRotation- fmod(v[1],360);
			if(retu > 138)
				retu = 138;
			return retu;
			break;
		case 3:
			return armdata->arms[3].maxRotation - (v[1] + v[2] -90);
			break;
		case 4:
			ret = ((armdata->arms[4].omax - addFactor[n])+(v[1] + v[2] + v[3] -90)*multFactor[3]*(PI/180))/(multFactor[4]*(PI/180));
			return ret;

			break;
		default:
			return 0;
			break;
	}}
