#include "ofxIndustrialRobotTimeline.h"
#include "ofxVectorMath.h"

ofxIndustrialRobotTimeline::ofxIndustrialRobotTimeline(){
	clear();
	isPaused = false;
	cues.push_back(new ofxIndustrialRobotPositionCue(200, ofxVec3f(1269.0,800.824,0.0), ofxVec3f(0.0,-1.0,0)));
};

void ofxIndustrialRobotTimeline::clear(){
	cues.clear();
	currentCue = 0;
	nCuesInSpline = 0;
	splineStart = 0;
	currentCueMaxSpeed = 0;
	splineTime = 0;
	cueStartTime = ofGetElapsedTimeMillis();
	timeStartFadeDown = 0;
	fadeDownStartSpeed = 0;
	tmp = 0;
	atSplineEnd = false;
}

float fade(float percent, float start, float stop){
	float a = (stop-start);
	float percent2 = (float)(50.0*(sin((PI*percent)-HALF_PI)+1.0))/100.0;
	return a*percent2 + start;
}

float fadeDistance(float start, float stop, float time){
	float t = (time/1000.0);
	float a = (stop-start)/t;
	return 0.5*a*t*t+start*t;
}

float calculateDistanceInSpline(ofxMSASpline3D spline, float start, float end, int resolution){
	float ret = 0;
	float pDist = (float)end-start;
	if(start < 0 || start > 1.0)
		return NULL;
	ofxVec3f p1 = spline.sampleAt(start);				
	for(int i=1 ; i<=resolution ; i++){
		float s = start+(float)pDist*i/resolution;
		if(s < 0 || s > 1.0)
			return NULL;
		ofxVec3f p2 = spline.sampleAt(s);
		ret += (p2-p1).length();
		p1 = p2;
	}	
	return ret;
}

void ofxIndustrialRobotTimeline::play(){
	isPaused = false;
}
void ofxIndustrialRobotTimeline::pause(){
	isPaused = true;
}




void ofxIndustrialRobotTimeline::step(ofxVec3f v, float framerate){
	if(framerate > 0){ //we have to check so we don't get divide by zero
		float dt = 1.0 / framerate;
		if(!isPaused){
			//Vi tjekker først om vi står i et waypoint
			if( (splineTime * (nCuesInSpline-1) >= currentCue-splineStart  && currentCue < getLastPositionCue())){
				float diff = (float)splineTime*(nCuesInSpline-1) - (float)(currentCue-splineStart);
				//Hastigheden som vi har bevæget os med som max hastighed;	
				if(!atSplineEnd){
					currentCueMaxSpeed = v.length(); 
				}
				
				//Da vi går til næste cue, kan vi update pathen
				//Først finder vi starten af den nye kurve
				splineStart = (currentCue > 1)? currentCue - 1 : 0;
				if(numberPositionCues()-splineStart < 4){
					splineStart = numberPositionCues() - 4;
				}
				if(splineStart < 0){
					splineStart = 0;	
				}
				
				//Generer kurven
				if(numberPositionCues()-splineStart < 4){
					mode = OFX_MSA_SPLINE_LINEAR;
				} else {
					if(getPositionCue(currentCue+1)->isCubicSpline)
						mode = OFX_MSA_SPLINE_CUBIC;
					else 
						mode = OFX_MSA_SPLINE_LINEAR;
				}
				generateSpline(splineStart,0); //every time we are in a cue point we can generate a new spline (with the new points eventually added)
				atSplineEnd = false;
				nCuesInSpline = numberPositionCues()-splineStart;
				
				cueStartTime = ofGetElapsedTimeMillis();
				
				currentCue++;
				tmp  ++;
				//Udregn hvor i splinen vi er nu efter vi har genereret ny spline
				splineTime = (float)(currentCue-1-splineStart+diff) / (nCuesInSpline-1); //Calculate where in the spline we are (0.0-1.0)
				
				timeStartFadeDown = 0;
				timeStartFadeDown = 0;
				if(getPositionCue(currentCue-1)->endPause){
					pause();
				}
			} 
			
			///Check if we should step forward in spline
			if(splineTime * (nCuesInSpline-1) < currentCue-splineStart){
				int inTime = 1000; //Tiden den fader ind på
				int outTime = 1000; //Tiden den fader ud på
				int curTime = ofGetElapsedTimeMillis()-cueStartTime; 
				float reql = getPositionCue(currentCue)->speed*dt; //Hvis vi ser bort fra fades, hvor langt burde vi komme med den satte hastighed
				float lengthToNextCue = calculateDistanceInSpline(spline, splineTime, (float)(currentCue-splineStart) / (nCuesInSpline-1), 2000); //Approx. længde til næste cue fra hvor vi er
				//			cout<<"lengthtonextcue   "<<lengthToNextCue<<endl;
				//Udregn hvor hurtig robotten skal være ved udgang af cue
				float dstSpeed; 
				if(currentCue < getLastPositionCue()){
					if(nCuesInSpline + splineStart > currentCue + 1 && !getPositionCue(currentCue+1)->endPause && !getPositionCue(currentCue)->endInZeroSpeed)
						dstSpeed = getPositionCue(currentCue+1)->speed;
					else {
						dstSpeed = 0.0;
					}
				} else {
					dstSpeed = 0.0;
				}			
				
				float reqFadeDistance ;
				if(timeStartFadeDown > 0){					
					reqFadeDistance = fadeDistance(fadeDownStartSpeed, dstSpeed, outTime);
				} else {
					reqFadeDistance = fadeDistance(v.length(), dstSpeed, outTime);				
				}
				
				if(reqFadeDistance > lengthToNextCue && v.length() > 0){
					//				cout<<lengthToNextCue<<"    "<<reqFadeDistance<<endl;
					if(timeStartFadeDown == 0){
						timeStartFadeDown = ofGetElapsedTimeMillis();
						fadeDownStartSpeed = v.length();
					}
					float p = ((float)(ofGetElapsedTimeMillis()-timeStartFadeDown)/outTime);
					//	p = 1.0-lengthToNextCue/reqFadeDistance-0.1 ;
					//				cout<<p<<endl;
					if(p> 1.0)
						p= 1.0;
					reql = fade(p, fadeDownStartSpeed,dstSpeed)*dt;
					//				cout<<p<<"reql "<<reql<<"   "<<fadeDownStartSpeed<<"   "<<dstSpeed<<endl;
				}
				else if(curTime <= inTime){
					reql = fade((float)curTime/inTime, currentCueMaxSpeed, getPositionCue(currentCue)->speed)*dt;
				}	
				
				
				float l = 0;
				float dSt = 0;
				ofxVec3f p1 = spline.sampleAt(splineTime);
				while(l < reql && atSplineEnd == false){
					dSt += 0.000001;
					if(dSt + splineTime <= 1.0 ){
						ofxVec3f p2 = spline.sampleAt(splineTime+dSt);
						l += (p2-p1).length();
						p1 = p2;
					} else {
						atSplineEnd = true;
						if(nCuesInSpline + splineStart < numberPositionCues())
							currentCueMaxSpeed = v.length(); 
						else {
							currentCueMaxSpeed = 0;
						}
					}
				}
				
				
				splineTime += dSt; //Step forward in the splines time
			}
		}
	} 
	
	
	
}

ofxVec3f ofxIndustrialRobotTimeline::getPosition(){
	if(numberPositionCues() == 1){
		return getPositionCue(0)->position;
	} else if(numberPositionCues() == 0){
		return ofxVec3f(1000,700,0);	
	} else {
		return 	spline.sampleAt(splineTime);
	}
}

ofxVec3f ofxIndustrialRobotTimeline::getHandPosition(){
	if(numberPositionCues() == 1){
		return getPositionCue(0)->hand;
	} else if(numberPositionCues() == 0){
		return ofxVec3f(1,0,0);	
	} else {
		return 	handSpline.sampleAt(splineTime);
	}
}

ofxVec3f ofxIndustrialRobotTimeline::getNextCuePosition(){
	return getPositionCue(currentCue)->position;
	
	
}
ofxVec3f ofxIndustrialRobotTimeline::getNextCueHandPosition(){
	return getPositionCue(currentCue)->hand;
}

int ofxIndustrialRobotTimeline::numberCues(){
	return cues.size();
}

int ofxIndustrialRobotTimeline::numberPositionCues(){
	unsigned int n = 0;
	for(int i=0;i<cues.size();i++){
		if(cues[i]->isPositionCue)
			n++;
	}
	return n;
}

int ofxIndustrialRobotTimeline::getLastPositionCue(){
	for(int i=cues.size()-1;i>=0;i--){
		if(cues[i]->isPositionCue)
			return i;			
	}
	return 0;
}

ofxIndustrialRobotPositionCue* ofxIndustrialRobotTimeline::getPositionCue(int n){
	int c = 0;
	for(int i=0;i<numberCues();i++){
		if(cues[i]->isPositionCue){
			if(c == n){
				return (ofxIndustrialRobotPositionCue*)cues[i];
			}
			c++;
		}
	}
	return NULL;
}

void ofxIndustrialRobotTimeline::generateSpline(int first, int last){
	spline.clear();
	handSpline.clear();
	spline.setInterpolation(mode);
	handSpline.setInterpolation(mode);
	//spline.setInterpolation(OFX_MSA_SPLINE_LINEAR);
	//spline.setUseDistance(true);
	int n= numberPositionCues()-last;
	for(int i=first;i<n;i++){
		spline.push_back(getPositionCue(i)->position);
		handSpline.push_back(getPositionCue(i)->hand);
	}	
	nCuesInSpline = n;
}
