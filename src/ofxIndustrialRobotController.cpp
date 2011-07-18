
#include "ofxIndustrialRobotController.h"

ofxIndustrialRobotController::ofxIndustrialRobotController(ofxIndustrialRobotCoreData* _data, ofxIndustrialRobotArmHelper * _helper, ofxIndustrialRobotTimeline * _timeline, ofxIndustrialRobotPlotter * _plotter){
	data = _data;
	helper = _helper;
	timeline = _timeline;
	plotter = _plotter;
	
	input = Slider;
	targetPosition = ofxVec3f();
	targetDir = ofxVec3f(0.0, -1.0, 0.0);
	lastTargetPosition = ofxVec3f();
	helper->data->reverseHeadPercent = 1.0;
	helper->data->reverseHeadPercentGoal = 1.0;
	helper->data->reverseHeadPercentTmp = 1.0;
	gravityTarget = ofxVec3f();
	gravityV = ofxVec3f();
	gravityVDir = ofxVec3f();
	gravityTargetDir = ofxVec3f();
	gravityMaxSpeed = 0.0;
	gravityForce = 1.0;
	mode = 0;
	variantLocked = false;
	
	axis4variant = 0.0;
	axis4variantgoal = 0.0;
	axis4varianttmp = 0.0;
	
	changingVariant = false;
	changingVariant2 = false;
	
	inZoneSafetyMode = false;
	
}

void ofxIndustrialRobotController::setInput(int _input){
	input = _input;
	if(input == Gravity){
		gravityV = ofxVec3f(0.0,0.0,0.0);
		gravityVDir = ofxVec3f(0.0,0.0,0.0);
		gravityTarget = targetPosition;
		gravityTargetDir = targetDir;
		
	} if(input == Timeline){
		timeline->clear();
		addCue(100, targetPosition, targetDir);
	}
	
	if(!legalPosition(targetPosition,targetDir, helper->data->reverseHeadPercentGoal)){
		float ot = 0.0;
		if( helper->data->reverseHeadPercentGoal == 0.0)
			ot = 1.0;
		if(legalPosition(targetPosition, targetDir, ot)){
			helper->data->reverseHeadPercentGoal = ot;
			helper->data->reverseHeadPercent = ot;
			helper->data->reverseHeadPercentTmp = ot;
		}
	}	
}

void ofxIndustrialRobotController::gotoResetPosition(){
	for(int i=0;i<5;i++){
		data->arms[i].rotation = converter->convertO(i, 0);
	}
	targetPosition = helper->getEndPosition(5);
	targetDir = helper->getTargetDir();
	
	gravityV = ofxVec3f(0.0,0.0,0.0);
	gravityVDir = ofxVec3f(0.0,0.0,0.0);
	gravityTarget = targetPosition;
	gravityTargetDir = targetDir;
	
	timeline->clear();
	addCue(100, targetPosition, targetDir);
	
	if(!legalPosition(targetPosition,targetDir, helper->data->reverseHeadPercentGoal)){
		float ot = 0.0;
		if( helper->data->reverseHeadPercentGoal == 0.0)
			ot = 1.0;
		if(legalPosition(targetPosition, targetDir, ot)){
			helper->data->reverseHeadPercentGoal = ot;
			helper->data->reverseHeadPercent = ot;
			helper->data->reverseHeadPercentTmp = ot;
		}
	}		
	
}

void ofxIndustrialRobotController::step(float framerate){
	if(framerate > 0){	

		if(!variantLocked){
			//Check if we can change the variant 2
			if(!changingVariant){
				/*if(axis4variant != axis4variantgoal){
					//If the axis4variant (variant2) should be changed
					if(axis4variantgoal == 1.0 && axis4varianttmp < 1.0){
						axis4varianttmp += 0.08*(1.0/framerate);
					}
					if(axis4variantgoal == 0.0 && axis4varianttmp > 0.0){
						axis4varianttmp -= 0.08*(1.0/framerate);
					}
					axis4variant = (sin(axis4varianttmp*PI-HALF_PI)+1.0)/2.0;		
					
					if(axis4variant > 0.999){
						axis4variant = 1.0;
					}
					if(axis4variant < 0.001){
						axis4variant = 0.0;
					}
					
					changingVariant2 = true;	
					setHandToPosition(targetPosition,  targetDir,helper->data->reverseHeadPercent, axis4variant);			
				} else if(changingVariant2){
					//We just finished changing variant2, do the reset of it, and continue the code
					gravityV = ofxVec3f();
					gravityVDir = ofxVec3f();
					changingVariant2 = false;
				}
				 */
			}
			
			//Check if we can change the variant1
			if(!changingVariant2){
				if(helper->data->reverseHeadPercentGoal == 1.0 && helper->data->reverseHeadPercentTmp < 1.0){
					helper->data->reverseHeadPercentTmp += 0.09*(1.0/framerate);
				}
				if(helper->data->reverseHeadPercentGoal == 0.0 && helper->data->reverseHeadPercentTmp > 0.0){
					helper->data->reverseHeadPercentTmp -= 0.09*(1.0/framerate);
				}
				if(helper->data->reverseHeadPercent > 0.999){
					helper->data->reverseHeadPercent = 1.0;
				}
				if(helper->data->reverseHeadPercent < 0.001){
					helper->data->reverseHeadPercent = 0.0;
				}
				
				float variant1[5], variant2[5], variantgoal[5];
				if(!getArmRotations(targetPosition, targetDir, helper->data->reverseHeadPercentGoal, variant2, true, axis4variant) && helper->data->reverseHeadPercent != helper->data->reverseHeadPercentGoal){
					cout<<"Cannot change variant here because of space limits"<<endl;
				}
				if(helper->data->reverseHeadPercent != helper->data->reverseHeadPercentGoal && getArmRotations(targetPosition, targetDir, helper->data->reverseHeadPercentGoal, variant2, true, axis4variant) ){
					changingVariant = true;
					
					for(int i=0;i<5;i++){
						variant1[i] = converter->convertRotation(i);
					}
					for(int i=0;i<5;i++){
						variantgoal[i] = converter->convertRotation(i,variant2[i],variant2);
					}
					
					bool ok = true;
					for(int i=3;i<5;i++){
						if(variant1[i] < variantgoal[i]){
							variant1[i] += 0.7;
							if(variant1[i] > variantgoal[i]){
								variant1[i] = variantgoal[i];
							} else {
								ok = false;
							}
						} else if(variant1[i] > variantgoal[i]){
							variant1[i] -= 0.7;
							if(variant1[i] < variantgoal[i]){
								variant1[i] = variantgoal[i];
							} else {
								ok = false;
							}
						} 
					}
					
					float a[5];
					for(int i=0;i<5;i++){
						a[i] = converter->convertO(i,variant1[i],a);
						data->arms[i].rotation = a[i];
					}
					
					if(ok){
						changingVariant = false;
						helper->data->reverseHeadPercent = helper->data->reverseHeadPercentGoal;
						targetPosition = helper->getEndPosition(5);
						targetDir = helper->getTargetDir();
						gravityV = ofxVec3f();
						gravityVDir = ofxVec3f();
					}
					
				}
				
			} else
				changingVariant = false;
		}		
	}
	
	float variant1[5], variant2[5];
	getArmRotations(targetPosition, targetDir, 0.0, variant1, true);
	getArmRotations(targetPosition, targetDir, 1.0, variant2, true);
	lastTargetPosition = targetPosition;		
	
	
	if(!changingVariant && !changingVariant2){		
		//*******************
		//Timeline position
		//*******************
		if(input == Timeline && !changingVariant){
			timeline->step(vTarget, framerate);
			targetPosition = timeline->getPosition();
			targetDir = timeline->getHandPosition();
			
			if(!legalPosition(timeline->getNextCuePosition(), timeline->getNextCueHandPosition(), helper->data->reverseHeadPercentGoal)){
				float ot = 0.0;
				if( helper->data->reverseHeadPercentGoal == 0.0)
					ot = 1.0;
				if(legalPosition(timeline->getNextCuePosition(), timeline->getNextCueHandPosition(), ot)){
					helper->data->reverseHeadPercentGoal = ot;
				}
			}
			setHandToPosition(targetPosition, targetDir,helper->data->reverseHeadPercent);
		}	
		
		
		//*******************
		//Manual position
		//*******************
		if(input == ManualPosition && !changingVariant){	
			variant = ofxIndustrialRobotDefines::Auto;
			gravityMaxSpeed = 1.0;
			gravityForce = 0.03;
		}
		
		//******************
		//Gravity
		//******************
		if(input == Gravity || input == ManualPosition){
			//Zone security
			float rotations[5];
			getArmRotations(gravityTarget, gravityTargetDir, helper->data->reverseHeadPercentGoal, rotations, false);
			int numZones = (rotations[0]-helper->data->arms[0].rotation)/(360.0/NUMBER_ZONES);
			//	cout<<numZones<<"  curRot: "<<(helper->data->arms[0].rotation) <<" wanted rot: "<< rotations[0]<<endl;
			ofxVec3f gTarget, gDir;
			if(abs(numZones)>6 || (inZoneSafetyMode && abs(numZones)>1 )){

				//Safe zone mode
				inZoneSafetyMode = true;
				cout<<"In zone mode"<<endl;
				
				float rotations[5], rotationsTmp[5];
				for(int i=0;i<5;i++){
					rotations[i] = rotationsTmp[i] = helper->data->arms[i].rotation;							
				}
				//	cout<<"Goal: "<<helper->data->reverseHeadPercentGoal<<"  current: "<<helper->data->reverseHeadPercent<<endl;
				if(helper->data->reverseHeadPercentGoal == 1.0){
					rotations[1] = -10.0;//robot->industrialRobot->thread.converter->getMinRotation(1, rotations)+5;
					rotations[2] = 119.0;
					if(helper->data->reverseHeadPercent < 0.5){
						rotations[3] = 90;
						rotations[4] = -180;
					} else {
						rotations[3] = -70;
						rotations[4] = 0;
					}
				} else {							
					rotations[1] = -10.0;//robot->industrialRobot->thread.converter->getMinRotation(1, rotations)+5;
					rotations[2] = 119.0;
					if(helper->data->reverseHeadPercent < 0.5){
						rotations[3] = 90;
						rotations[4] = -180;
					} else {
						rotations[4] = 0;
						rotations[3] = -70;
					}
				}
				
				for(int i=0;i<5;i++){
					helper->data->arms[i].rotation = rotations[i];							
					//	cout<<i<<" : "<<rotations[i]<<endl;
				}						
				gTarget = helper->getEndPosition(5);
				gDir = helper->getTargetDir();
				cout<<"Dir: "<<gDir.x<<"  "<<gDir.y<<"  "<<gDir.z<<endl;
				cout<<"Target: "<<gTarget.x<<"  "<<gTarget.y<<"  "<<gTarget.z<<endl;
				variantLocked = true;
				gravityVariant = ofxIndustrialRobotDefines::Auto;
				
				gravityMaxSpeed = 1.0;
				gravityForce = 0.1;
				
				
				if(fabs(rotations[1] - rotationsTmp[1]) < 5.0 && fabs(rotations[2] - rotationsTmp[2]) < 5.0){
					
					variantLocked = false;
					safeRotationA = 0.01;
					if(safeRotationV <= 3.0){
						safeRotationV += safeRotationA;
					}
					float rot = safeRotationV;
					
					gravityMaxSpeed = 6.0;
					gravityForce = 0.3;
					
					if(numZones < 0){
						helper->data->arms[0].rotation -= rot;
					} else {
						helper->data->arms[0].rotation += rot;
					}
					//					cout<<rotations[0]<<endl;
					gTarget = helper->getEndPosition(5);
					gDir = helper->getDir(5);
					
					if(helper->data->reverseHeadPercent < 0.5){
						gravityVariant = ofxIndustrialRobotDefines::Down;						
					} else {
						gravityVariant = ofxIndustrialRobotDefines::Up;
					}
					
					//		gTarget = targetPosition;					
				}
				
				
				for(int i=0;i<5;i++){
					helper->data->arms[i].rotation = rotationsTmp[i];							
				}		
				
				
				
			} else if(inZoneSafetyMode){
				variantLocked = false;
				inZoneSafetyMode = false;	
				gravityMaxSpeed = 1.0;
				gravityForce  = 0.5;
				gTarget = gravityTarget;
				gDir = gravityTargetDir;	
				gravityVariant = variant;
			} else {
				gTarget = gravityTarget;
				gDir = gravityTargetDir;
				gravityVariant = variant;
			}
			//Usual gravity movement if its not to far away (to many zones)
			
			//Constants
			double a  = gravityForce*0.01;
			double maxSpeed = gravityMaxSpeed;
			
			//Variables recalculated every frame
			double goalDist = (gTarget - targetPosition).length();
			double curSpeed = gravityV.length();
			double curAcc = gravityA.length();
			if( (gravityA*-1.0 + (gTarget - targetPosition)).length() >= (gravityA + (gTarget - targetPosition)).length()){
				curAcc *= -1.0;
			}
			
			double tmt = (sqrt(curAcc*curAcc/a + 2.0* curSpeed)/(sqrt(2.0) *sqrt(a)));
			double amt = -(sqrt(a) * sqrt((curAcc*curAcc +  a * 2.0 * curSpeed)/a))/sqrt(2);
			
			double distanceAtBrake = (curAcc*curAcc*curAcc)/(3.0 *a*a) + (curAcc*curAcc * tmt)/a + (amt*tmt*tmt)/2.0 - (a * tmt*tmt*tmt)/2.0 + (curAcc * curSpeed)/a + 2.0 * tmt * curSpeed;
			double speedIfAccelerate = curAcc * (-curAcc)/a + (a * ((-curAcc)/a)*((-curAcc)/a))/2.0 + curSpeed;
			
			double speedIfDeac = 0.5*curAcc*curAcc/a + curSpeed;
			
			if(!gravityV.normalized().match((gTarget - targetPosition).normalized()) ){
				gravityV += ((gTarget - targetPosition).normalized() - gravityV.normalized())*0.05;
			}
			
			mode = 0;
			if(goalDist > 0)
				mode = 1;
			
			if(speedIfDeac > maxSpeed){
				mode = -1;
				if(curAcc < 0){
					mode = 0;
					gravityA= ofxVec3f();
				}
			}
			if(distanceAtBrake >= goalDist){
				mode = -1;
			}
			if(speedIfAccelerate <= 0  && curAcc < 0){
				mode = 1;
			}
			
			if(goalDist < 0.2){
				targetPosition = gTarget;
				gravityA = ofxVec3f();
				gravityV = ofxVec3f();
			} else {
				gravityA += (gTarget - targetPosition).normalized() * a * (float)mode;
			}
			
			gravityV += gravityA*(125.0/framerate);
			if( (gravityA*-1.0 + gravityV).length() <= (gravityA + gravityV).length()){
				data->armAccHistory.push_back((gravityA.length()+0.2)*6.0);
			} else {
				data->armAccHistory.push_back((-gravityA.length()+0.2)*6.0);	
			}
			
			
			gravityVDir *= 0.9;
			gravityVDir += (gDir - targetDir )*0.007*gravityForce;
			if(gravityVDir.length() > 0.005){
				gravityVDir.limit(0.005);
			}
		//	cout<<gravityVDir.length() <<endl;
			
			targetPosition += gravityV*(125.0/framerate);
			targetDir += gravityVDir*(125.0/framerate);
			targetDir.normalize();
			
			if(gravityVariant == ofxIndustrialRobotDefines::Auto || inZoneSafetyMode){
				if(!legalPosition(gTarget,gDir, helper->data->reverseHeadPercentGoal)){
					
					float ot = 0.0;
					if( helper->data->reverseHeadPercentGoal == 0.0)
						ot = 1.0;
					if(legalPosition(gTarget, gDir, ot)){
						helper->data->reverseHeadPercentGoal = ot;
						if(axis4variantgoal == 1.0 && axis4variant < 0.5){
							if(legalPosition(gTarget, gDir, helper->data->reverseHeadPercentGoal, 0.0)){
								axis4variantgoal = 0.0;
							}
						} else if(axis4variantgoal == 0.0 && axis4variant > 0.5){
							if(legalPosition(gTarget, gDir, helper->data->reverseHeadPercentGoal, 1.0)){
								axis4variantgoal = 1.0;
							}
						}
						
						setHandToPosition(targetPosition,  targetDir,helper->data->reverseHeadPercent, axis4variant);			
					}
				} else {
					setHandToPosition(targetPosition,  targetDir, helper->data->reverseHeadPercent, axis4variant);			
				}
				
			} else {
				if(gravityVariant == ofxIndustrialRobotDefines::Up){
					helper->data->reverseHeadPercentGoal = 1.0;
				}
				if(gravityVariant == ofxIndustrialRobotDefines::Down){
					helper->data->reverseHeadPercentGoal = 0.0;
				}
				if(legalPosition(gTarget,gDir, helper->data->reverseHeadPercentGoal)){
					setHandToPosition(targetPosition,  targetDir, helper->data->reverseHeadPercent, axis4variant);		
				}
			}
			if(!legalPosition(gTarget, gDir, helper->data->reverseHeadPercentGoal,axis4variantgoal)){				
				if(legalPosition(gTarget, gDir, helper->data->reverseHeadPercentGoal, 0.0)){
					axis4variantgoal = 0.0;
				}
				if(legalPosition(gTarget, gDir, helper->data->reverseHeadPercentGoal, 1.0)){
					axis4variantgoal = 1.0;
				}
				
			}
		}
		
		
		//*******************
		//Manual motor / slider
		//*******************
		
		if(input == Slider  && !changingVariant){
			float rotations[5];
			getArmRotations(targetPosition, targetDir, helper->data->reverseHeadPercent, rotations, false);
			targetPosition = helper->getEndPosition(5);
			targetDir = helper->getTargetDir();
			if(!legalPosition(targetPosition, targetDir, helper->data->reverseHeadPercentGoal)){
				float ot = 0.0;
				if( helper->data->reverseHeadPercentGoal == 0.0){
					ot = 1.0;
				}
				if(legalPosition(targetPosition, targetDir, ot)){
					helper->data->reverseHeadPercentGoal = ot;
					helper->data->reverseHeadPercentTmp = ot;
					helper->data->reverseHeadPercent = ot;
				}
			}
		}
	}
	
	vTarget = targetPosition - lastTargetPosition;
	vTarget *= framerate;
	if(lastTargetPosition.length() == 0){
		vTarget = 0;
	}
	data->armSpeedHistory.push_back(vTarget*6);
	
	
}

bool ofxIndustrialRobotController::getArmRotations(ofxVec3f toolEnd, ofxVec3f dir, float variant, float* ret,  bool limitMotors,float variant2){
	ofxVec3f toolStart = toolEnd - dir.normalized()*data->tool->l;
	ofxVec3f dist = toolStart - helper->getEndPosition(0);
	double a = atan2(toolStart.x,toolStart.z)-HALF_PI;
	if(a < -PI)
		a += TWO_PI;
	ofxVec3f toolStartRotated = toolStart.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	ofxVec3f dirRotated = dir.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	
	double a1;
	a1 = atan2(dirRotated.y,dirRotated.x) - HALF_PI + variant*PI;
	ofxVec3f h = -ofxVec3f(cos(a1), sin(a1))*(data->arms[3].length+data->arms[4].length + data->tool->h/2.0);
	
	dirRotated = dirRotated.getRotatedRad(HALF_PI - (a1) + variant*PI, ofxVec3f(0.0,0.0,1.0));
	
	double a2;
	a2 =  (atan2(dirRotated.x,dirRotated.z)-HALF_PI - variant*PI) * (1.0 + variant*-2.0)-TWO_PI*variant;
	ofxVec3f armPosition = toolStart + h.getRotatedRad(a, ofxVec3f(0.0,1.0,0.0));
	
	
	float ret2[5]; 
	bool ok = getArmRotations2(armPosition, 360.0*a1/TWO_PI, 360.0*a2/TWO_PI, ret2, limitMotors, variant2);
	
	for(int i=0;i<5;i++){
		ret[i] = ret2[i];
	}
	return ok;
}

bool ofxIndustrialRobotController::getArmRotations2(ofxVec3f position, double rotation1, double rotation2,  float* ret,  bool limitMotors,float variant2){
	double a = atan2(position.x,position.z)-HALF_PI;
	if(a < 0)
		a += TWO_PI;
	
	if((position-helper->getEndPosition(0)).length()>getMaxToolDistance()){
		cout<<"To far away "<<(position-helper->getEndPosition(0)).length()<<"  "<<getMaxToolDistance()<<endl;
		if(!limitMotors)
			return false;
		position -= (position-helper->getEndPosition(0)).normalized()*((float)(position-helper->getEndPosition(0)).length()-getMaxToolDistance()+0.001);
	}
	
	ofxVec3f rotatedTarget = position.getRotatedRad(-a, ofxVec3f(0.0,1.0,0.0));
	
	ofxVec3f rotatedStart = ofxVec3f(0.0,1.0,0.0)* data->arms[0].length - data->arms[0].offset.rotated(90, ofxVec3f(0,0,1)) ;
	
	float b = sqrt((rotatedTarget.x-rotatedStart.x)*(rotatedTarget.x-rotatedStart.x) + (rotatedTarget.y-rotatedStart.y)*(rotatedTarget.y-rotatedStart.y));
	
	float base = (data->arms[1].length*data->arms[1].length  - data->arms[2].length*data->arms[2].length + b*b)/(2.0*b);
	float h = sqrt(data->arms[1].length*data->arms[1].length-base*base);
	float px=rotatedStart.x+base*(rotatedTarget.x-rotatedStart.x)/b;
	float py=rotatedStart.y+base*(rotatedTarget.y-rotatedStart.y)/b;
	
	float q2x=px-h*(rotatedTarget.y-rotatedStart.y)/b;
	float q2y=py+h*(rotatedTarget.x-rotatedStart.x)/b;
	
	float a1 = atan2(q2y-rotatedStart.y, q2x-rotatedStart.x);
	float a2 = atan2(rotatedTarget.y-q2y, rotatedTarget.x-q2x);
	float r[5];
	if(!isnan((180.0*a/PI))&&!isnan(-(180.0*a1/PI-90)) && !isnan(-(180.0*a2/PI-90)+(180.0*a1/PI-90))&&!isnan(-(180.0*(-a2)/PI)-rotation1)){
		r[0] = fmod(180.0*a/PI, 360);	
		r[1] = fmod(-(180.0*a1/PI-90),360);
		r[2] = fmod(-(180.0*a2/PI-90)+(180.0*a1/PI-90),360);
		r[3] = fmod(-(180.0*(-a2)/PI)-rotation1,360);
		r[4] = rotation2;
		
		if(r[0] > 180){
			r[0] -= 360;
		}
	} else {
		cout<<"NaN"<<endl;
		return false;
	}
	float diff3 = -90 - r[3];
	if(diff3 > 0){
		r[3] += diff3*sin((variant2)*PI);
	}
	r[4] += 360*variant2;
	
	if(limitMotors){
		for(int i=0;i<4;i++){
			if(fmod(r[i],360) < fmod(converter->getMinRotation(i, r),360) || fmod(r[i],360) > fmod(converter->getMaxRotation(i, r),360)){
				if(fmod(r[i],360) + 360 > fmod(converter->getMinRotation(i, r),360) && fmod(r[i],360) + 360 < fmod(converter->getMaxRotation(i, r),360)){			
					r[i] = fmod(r[i],360) + 360;
				}
				if(fmod(r[i],360) - 360 > fmod(converter->getMinRotation(i, r),360) && fmod(r[i],360) - 360 < fmod(converter->getMaxRotation(i, r),360)){			
					r[i] = fmod(r[i],360) - 360;
				}
			}
		}
		
		for(int i=0;i<5;i++){
			if(r[i] <= converter->getMinRotation(i, r)){
				//		cout<<"Less then min "<<i<<endl;
				if(i == 4){
					if((r[i]) <= converter->getMinRotation(i, r)){
						return false;
					}
				} else {
					return false;
				} 
			} else if(r[i] >= converter->getMaxRotation(i, r)){
				//		cout<<"More then max on axis "<<i<<"  "<<r[i] <<" >= "<< converter->getMaxRotation(i, r)<<endl;
				if(i == 4){
					if((r[i]) >= converter->getMaxRotation(i, r)){
						return false;
					}
				} else {
					return false;
				}
			}
		}
	}
	for(int i=0;i<5;i++){
		ret[i] = r[i];	
	}
	return true;
}


bool ofxIndustrialRobotController::legalPosition(ofxVec3f toolEnd, ofxVec3f dir, float variant,float variant2){
	float b[5];
	if(variant2 == -1){
		bool ok= getArmRotations(toolEnd, dir, variant, b, true, 0.0);
		if(ok)
			return true;
		return getArmRotations(toolEnd, dir, variant, b, true, 1.0);
	} else {
		return getArmRotations(toolEnd, dir, variant, b, true, variant2);	
	}
}



void ofxIndustrialRobotController::setHandToPosition(ofxVec3f toolEnd, ofxVec3f dir, float variant,float variant2){	
	//	cout<<variant<<endl;
	float b[5];
	if(variant == -1){
		variant = helper->data->reverseHeadPercent;
	}
	
	getArmRotations(toolEnd, dir, variant, b, false,variant2);
	
	
	for(int i=0;i<5;i++){
		if(b[i] < converter->getMinRotation(i, b)){
			b[i] = converter->getMinRotation(i, b);
		}
		if(b[i] > converter->getMaxRotation(i, b)){
			b[i] = converter->getMaxRotation(i, b);
		}
		
		data->arms[i].rotation = b[i];
	}
	
	
}


float ofxIndustrialRobotController::getMaxToolDistance(){
	float r[5];
	r[0] = 0;
	r[1] = converter->getMaxRotation(1, r);
	ofxVec3f v1 = ofxVec3f(data->arms[1].length,0,0);
	ofxVec3f v2 = ofxVec3f(data->arms[2].length,0,0).rotated(converter->getMinRotation(2, r),ofxVec3f(0,0,1));
	return (v1+v2).length();
}

bool ofxIndustrialRobotController::addCue(float speed, ofxVec3f target, ofxVec3f dir,  bool cubicSpline,  bool endPause,  bool endInZeroSpeed){
	if(legalPosition(target,  dir, 1.0) || legalPosition(target,  dir, 0.0)){
		timeline->cues.push_back(new ofxIndustrialRobotPositionCue(speed, target, dir, cubicSpline, endPause,endInZeroSpeed));
		return true;
	} else return false;
	
}

void ofxIndustrialRobotController::mousePressed(ofxVec3f target){
	if(input == Timeline){
		if(addCue(100, target, targetDir)){
			cout<<"Added "<<target.x<<", "<<target.y<<", "<<target.z<<endl;
		}
	} else if(input == Gravity){
		if(legalPosition(target, targetDir, 0.0) || legalPosition(target, targetDir, 1.0)){
			gravityTarget = target;
			gravityTargetDir = targetDir;
			gravityForce = 0.1;
			gravityMaxSpeed = 1.5;
			variant = ofxIndustrialRobotDefines::Auto;
			
		}
	}
	
}
