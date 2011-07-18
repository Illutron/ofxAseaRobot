#include "ButtonMatrixMode.h"


ButtonMatrixMode::ButtonMatrixMode(Robot * _robot, myData * _data){
	name = "ButtonMatrix";
	robot = _robot;
	robotData = _data;
	rate = 0;
	
	pressedTimer = 0;
	curButton = -1;
	foundMasterSafePos = false;
	init();
	
	QVBoxLayout * tabLayout = new QVBoxLayout();
	widget->setLayout(tabLayout);
	
	QWidget * grid = new QWidget();
	gridLayout = new QGridLayout();
	gridLayout->setContentsMargins(0,0,0,0);
	gridLayout->setAlignment(Qt::AlignCenter);
	
	grid->setFixedHeight(320*8.0/12-80);
	grid->setLayout(gridLayout);
	gridLayout->setAlignment(Qt::AlignTop);
	
	tabLayout->addWidget(grid);
	tabLayout->setAlignment(Qt::AlignTop);	
	
	QWidget * gridButton = new QWidget();
	QGridLayout * gridButtonLayout = new QGridLayout();
	gridButtonLayout->setAlignment(Qt::AlignTop);
	gridButtonLayout->setAlignment(Qt::AlignCenter);
	gridButton->setLayout(gridButtonLayout);
	gridButton->setFixedHeight(50);
	gridButton->setContentsMargins(0,0,0,0);
	
	tabLayout->addWidget(gridButton);
	
	
	QWidget * gridSliders = new QWidget();
	QGridLayout * gridSlidersLayout = new QGridLayout();
	gridSlidersLayout->setAlignment(Qt::AlignTop);
	gridSlidersLayout->setAlignment(Qt::AlignCenter);
	gridSliders->setLayout(gridSlidersLayout);
	//	gridSliders->setFixedHeight(50);
	gridSliders->setContentsMargins(0,0,0,0);
	
	tabLayout->addWidget(gridSliders);
	
	int row = 0, column = 0;
	
	for(int i=0;i<96;i++){
		checkboxes[i] = new QCheckBox();
		checkboxes[i]->setTristate(true);
		gridLayout->addWidget(checkboxes[i], row, column); 
		column ++;
		if(column == 12){
			row++;
			column = 0;
		}		
		
		lightTurnedOn[i] = false;
	}	
	lastButton = 0;
	isLightOnImg.allocate(CAPWIDTH, CAPHEIGHT);
	storePosition[0] = new QPushButton("top-left");
	storePosition[1] = new QPushButton("top-right");
	storePosition[2] = new QPushButton("bottom-left");
	
	allOn = new QPushButton("All on");
	allOff= new QPushButton("All off");;
	resetFlags = new QPushButton("Reset flags");
	
	//	gridButtonLayout->addWidget(new QLabel("Store"), 0, 0);
	for(int i=0;i<3;i++){
		gridButtonLayout->addWidget(storePosition[i], 0, i+1);
	}
	gridButtonLayout->addWidget(allOn, 1, 1);
	gridButtonLayout->addWidget(allOff, 1, 2);
	gridButtonLayout->addWidget(resetFlags, 1, 3);
	
	for(int n=0;n<3*3*2;n++){
		positionInformation[n] = new float(0);
	}
	
	row=0;
	int col=0;
	for(int i=0;i<9;i++){
		parSliders[i] = new QSlider(Qt::Horizontal);
		parSliders[i]->setMaximum(1000);
		gridSlidersLayout->addWidget(parSliders[i], row, col);
		col ++;
		if(col > 2){
			col = 0;
			row ++;
		}
	}
	
	buttonRepeat = 0;
}



void ButtonMatrixMode::init(){
	controllerItem = robot->addRobotController("ButtonMatrix", CONTROL_MODE_GRAVITY);
}

void ButtonMatrixMode::activate(){
	robot->setRobotController(controllerItem);
	
}
bool ButtonMatrixMode::deactivate(){
	if(controllerItem->locked){
		return false;
	}
	return true;
}

void ButtonMatrixMode::securityCheck(){
	if(isPointInBoundingbox(robot->industrialRobot->getCurrentTarget(),90)){
		//	cout<<"In safety area"<<endl;
		//	v = robot->industrialRobot->getVariantGoal();
		robot->industrialRobot->lockVariant(true);
//		controllerItem->locked = true;
		if(!robot->industrialRobot->isRobotFlagsReady() && controllerItem->isActive()){
			if(isPointInBoundingbox(robot->industrialRobot->getCurrentTarget(),70)){
				
				robot->industrialRobot->panic("OK Flag removed while in button matrix bounding box");
				robot->industrialRobot->setGravityTarget(robot->industrialRobot->getCurrentTarget()-ofxVec3f(0,0,300), robot->industrialRobot->getCurrentDir(), 0.1, 1.0, ofxIndustrialRobotDefines::Auto);
			}
		}
		//	cout<<v<<endl;
	} else {
		robot->industrialRobot->lockVariant(false);	
	//	controllerItem->locked = false;
		
	}
	
	if(isPointInBoundingbox(robot->industrialRobot->getCurrentTarget(),-20)){
		robot->industrialRobot->panic("To close to panels.");
		robot->industrialRobot->setGravityTarget(robot->industrialRobot->getCurrentTarget()-ofxVec3f(0,0,300), robot->industrialRobot->getCurrentDir(), 0.1, 1.0, ofxIndustrialRobotDefines::Auto);
		
	}	
	
}

void ButtonMatrixMode::step(){

//		cout<<"Light on "<<isLightOn()<<endl;
	int closestButton = -1;
	float closestButtonDistance = -1;
	
	int numChanges = 0;
	for(int i=0;i<96;i++){
		if(lightTurnedOn[i]){
			if(checkboxes[i]->checkState() != Qt::Checked){
				checkboxes[i]->setCheckState(Qt::PartiallyChecked);
				
			}	
		} else {
			if(checkboxes[i]->checkState() != Qt::Unchecked){
				if(checkboxes[i]->checkState() != Qt::PartiallyChecked){
					checkboxes[i]->setCheckState(Qt::Unchecked);					
				} else {
					checkboxes[i]->setCheckState(Qt::PartiallyChecked);
				}
			}				
		}
		if(checkboxes[i]->checkState()  == Qt::PartiallyChecked){
			numChanges++;
			ofxVec3f buttonPos = buttonPosition(i);
			ofxVec3f curPosition = robot->industrialRobot->getCurrentTarget();
			
			int rowC = closestButton / 12;
			// 			int colC = closestButton-rowC*12;	
			int row = i / 12;
			//			int col = i-row*12;	
			
			int border = 3;
			
			bool doit = false;
			if(closestButton == -1){
				doit = true;
			} else if((v == ofxIndustrialRobotDefines::Up && rowC > border && row <= border)){
				doit = true;				
			} else if((v == ofxIndustrialRobotDefines::Down && rowC <= border && row > border)){
				doit = true;				
			} else if((buttonPos - curPosition).length() < closestButtonDistance){
				if((v == ofxIndustrialRobotDefines::Up && row <= border) || (v == ofxIndustrialRobotDefines::Up && row > border && rowC > border)){
					doit = true;				
				} else if((v == ofxIndustrialRobotDefines::Down && row > border) || (v == ofxIndustrialRobotDefines::Down && row <= border && rowC <= border)){
					doit = true;				
				} 
			}
			
			if(doit){
				closestButton = i;
				closestButtonDistance = (buttonPos - curPosition).length();	
				
			}
			/*			if(closestButton == -1 || (buttonPos - curPosition).length() < closestButtonDistance){
			 if(closestButton != -1){
			 
			 if(v == ofxIndustrialRobotDefines::Up){
			 if(rowC
			 }
			 if(rowC > 3 && row <= 3){
			 
			 } else {
			 closestButton = i;
			 closestButtonDistance = (buttonPos - curPosition).length();	
			 }
			 } else {
			 closestButton = i;
			 closestButtonDistance = (buttonPos - curPosition).length();
			 
			 }
			 
			 }
			 }*/
			
			/*		if(checkboxes[i]->checkState()  != Qt::PartiallyChecked){
			 if(ofRandom(0, 100) < 0.1){
			 checkboxes[i]->setCheckState(Qt::PartiallyChecked);
			 
			 }
			 }	*/	
		}
	}
	
	if(numChanges > 0){
		rate += 0.2;
		if(rate > 80){
			rate = 80;
		}
	}
	
	if(curButton  == -1 && closestButton != -1){
		curButton = closestButton;
	}
	
	
	
	if(resetFlags->isDown()){
		for(int i=0;i<96;i++){
			checkboxes[i]->setCheckState(Qt::Unchecked);
			lightTurnedOn[i] = false;
			
		}		
		resetFlags->setDown(false);
	}
	
	if(allOn->isDown()){
		for(int i=0;i<96;i++){
			if(!lightTurnedOn[i]){
				checkboxes[i]->setCheckState(Qt::PartiallyChecked);
			} else {
				checkboxes[i]->setCheckState(Qt::Checked);						
			}
		}		
		allOn->setDown(false);
	}
	if(allOff->isDown()){
		for(int i=0;i<96;i++){
			if(lightTurnedOn[i]){
				checkboxes[i]->setCheckState(Qt::PartiallyChecked);
			} else {
				checkboxes[i]->setCheckState(Qt::Unchecked);						
			}
		}		
		allOff->setDown(false);
	}
	
	
	for(int n=0;n<3;n++){
		if(storePosition[n]->isDown()){
			QMessageBox msgBox;
			msgBox.setText("You are about to change the coordinates of the buttons");
			msgBox.setInformativeText("Are you sure you want to do it?");
			msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			int reply = msgBox.exec();
			if (reply == QMessageBox::Yes){
				cout<<"Set "<<n<<endl;
				
				*positionInformation[n*6] = robot->industrialRobot->getCurrentTarget().x;
				*positionInformation[n*6+1] = robot->industrialRobot->getCurrentTarget().y;
				*positionInformation[n*6+2] = robot->industrialRobot->getCurrentTarget().z;
				*positionInformation[n*6+3] = robot->industrialRobot->getCurrentDir().x;
				*positionInformation[n*6+4] = robot->industrialRobot->getCurrentDir().y;
				*positionInformation[n*6+5] = robot->industrialRobot->getCurrentDir().z;
				storePosition[n]->setDown(false);
			} else {
				storePosition[n]->setDown(false);
			}
		}
	}
	if(!isActive()){
		foundMasterSafePos = false;
		vidGrabber->setExposure(62);
	}
	if(isActive()){	
		if(numChanges > 0){
			controllerItem->locked = true;
		}
//		controllerItem->locked = t;

		vidGrabber->setExposure(8);
		if(curButton == -1){
			//No changes to the matrix
			cout<<"Set rate to zero"<<endl;
			rate = 0;
			v = ofxIndustrialRobotDefines::Up;
			ofxVec3f safeDist = buttonPosition(3*12+1) - cornerDir(0)*300.0;
			robot->industrialRobot->setGravityTarget(safeDist, cornerDir(3), 0.05, 1.0, v);			
			if(robot->industrialRobot->isRobotReady(100)){
				controllerItem->locked = false;	
			}
			
		}
		
		
		int row = curButton / 12;
		int col = curButton-row*12;
		if(!foundMasterSafePos){
			v = ofxIndustrialRobotDefines::Up;
			ofxVec3f safeDist = buttonPosition(3*12+1) - cornerDir(0)*300.0;
			cout<<robot->industrialRobot->setGravityTarget(safeDist, cornerDir(3), 0.05, 1.0, v)<<endl;			
			if(robot->industrialRobot->isRobotReady(30.0)){
				foundMasterSafePos = true;
			}
			
		} else {
			ofxIndustrialRobotDefines::HandVariant vOpp;
			if(v == ofxIndustrialRobotDefines::Up){
				vOpp = ofxIndustrialRobotDefines::Down;
			}
			else {
				vOpp = ofxIndustrialRobotDefines::Up;									
			}
			
			if(curButton > -1){
				ofxVec3f  xdir = ((cornerPos(1)+ofxVec3f(0,10,0)) - cornerPos(0))/11.0;
				ofxVec3f ydir = ((cornerPos(2)) - cornerPos(0))/7.0;
				ofxVec3f buttonPos  =  buttonPosition(curButton);
				
				ofxVec3f buttonDir = cornerDir(curButton);
				ofxVec3f safeDist = buttonPos - cornerDir(curButton)*60.0;
				
				ofxVec3f camOffset = ofxVec3f(0,-100,0);
				if(v == ofxIndustrialRobotDefines::Down){
					camOffset *= -1;
				}
				
				//if(robot->industrialRobot->willChangeVariant()){
				//				cout<<robot->industrialRobot->getVariantGoal() <<"  "<< v<<endl;
				if(robot->industrialRobot->getVariantGoal() != v){
					cout<<"Will change variant"<<endl;
					
					if(robot->industrialRobot->thread.lock()){
						cout<<"Will change variant"<<endl;
						
						float rotations[5], rotationsTmp[5];
						for(int i=0;i<5;i++){
							rotations[i] = rotationsTmp[i] = robot->industrialRobot->thread.coreData.arms[i].rotation;							
						}
						if(v == ofxIndustrialRobotDefines::Up){
							if(rotations[0] < -90){
								rotations[0] = -110;
							} else {
								rotations[0] = -50;	
							}
							rotations[1] = -13.5;//robot->industrialRobot->thread.converter->getMinRotation(1, rotations)+5;
//							rotations[2] -= rotations[1] - rotationsTmp[1];
							rotations[2] = 110.0;
							rotations[3] -= rotations[1] - rotationsTmp[1];
							rotations[3] -= rotations[2] - rotationsTmp[2];
						} else {							
							if(rotations[0] < -90){
								rotations[0] = -110;
							} else {
								rotations[0] = -50;	
							}
							rotations[1] = -13.5	;//robot->industrialRobot->thread.converter->getMinRotation(1, rotations)+5;
//							rotations[2] -= rotations[1] - rotationsTmp[1];
							rotations[2] = 119.0;
							rotations[3] -= rotations[1] - rotationsTmp[1];
							rotations[3] -= rotations[2] - rotationsTmp[2];
						}
						//						rotations[3] += 180;
						//						rotations[4] -= 180;
						for(int i=0;i<5;i++){
							robot->industrialRobot->thread.coreData.arms[i].rotation = rotations[i];							
						}						
						if(!robot->industrialRobot->setGravityTarget(robot->industrialRobot->thread.helper->getEndPosition(5), robot->industrialRobot->thread.helper->getTargetDir(), 0.05,2, vOpp, false)){
							cout<<"Could not add safe rotations point"<<endl;
						} else {
							cout<<"Goto variant change position"<<endl;
						}
						for(int i=0;i<5;i++){
							robot->industrialRobot->thread.coreData.arms[i].rotation = rotationsTmp[i];							
						}			
						
						
						if(robot->industrialRobot->isRobotReady(5.0,false)){
							if(v == ofxIndustrialRobotDefines::Up){
								robot->industrialRobot->thread.controller->helper->data->reverseHeadPercentGoal = 1.0;
							}
							
							if(v == ofxIndustrialRobotDefines::Down){
								robot->industrialRobot->thread.controller->helper->data->reverseHeadPercentGoal = 0.0;
							}
							cout<<"Change to "<<robot->industrialRobot->thread.controller->helper->data->reverseHeadPercentGoal<<"  "<<robot->industrialRobot->thread.controller->helper->data->reverseHeadPercent<<endl;
						}
						
						robot->industrialRobot->thread.unlock();
					}
					
				} else {
					
					if(pressedTimer == 0){	
						if(!foundToSafePos){
							//	controllerItem->locked = false;
							if(robot->industrialRobot->isLegalTarget(safeDist, buttonDir, vOpp) && ((row > 3 && v ==  ofxIndustrialRobotDefines::Up) || (row <= 3 && v ==  ofxIndustrialRobotDefines::Down))){
								if(row > 3){
									v =  ofxIndustrialRobotDefines::Down;
									cout<<"Down"<<endl;
								} else {
									v =  ofxIndustrialRobotDefines::Up;	
									cout<<"Up"<<endl;
								}
							}
							else if(robot->industrialRobot->setGravityTarget(safeDist, buttonDir, parSliders[0]->value()/1000.0, 5.0*parSliders[1]->value()/1000.0, v)){
								//		cout<<"goto safe pos"<<endl;
								
								if(robot->industrialRobot->isRobotReady(30.0*parSliders[2]->value()/1000.0)){
									foundToSafePos = true;
									//			cout<<"Found safe pos"<<endl;
								} else {
									//			cout<<"Not ready"<<endl;
								}
							} /*else if(robot->industrialRobot->isLegalTarget(safeDist, buttonDir, vOpp)){
							 v = vOpp;
							 }*/ else {
								 cout<<"Could not add safe pos point 1"<<endl;
							 }
						} else {
							//	controllerItem->locked = true;
							if(robot->industrialRobot->setGravityTarget(buttonPos, buttonDir,parSliders[3]->value()/1000.0, 3.0*parSliders[4]->value()/1000.0, v)){
								if(robot->industrialRobot->isRobotReady(30.0*parSliders[5]->value()/1000.0)){
									confirmedPress = false;
									pressedTimer = 1;
									cout<<"Found button pos"<<endl;
									
									
									if(robot->industrialRobot->setGravityTarget(safeDist, buttonDir, parSliders[6]->value()/1000.0, 3.0*parSliders[7]->value()/1000.0, v)){
										
									} else {
										cout<<"Could not add safe pos point 2"<<endl;
									}
								}
							} else {
								cout<<"Could not add button pos point"<<endl;
							}
						}
					} else {
						foundToSafePos = false;
						//controllerItem->locked = false;
						if(!robot->industrialRobot->isRobotReady(20.0*parSliders[8]->value()/1000.0)){
							//	controllerItem->locked = true;
							//	robot->industrialRobot->setGravityTarget(safeDist, buttonDir, parSliders[6]->value()/1000.0, 3.0*parSliders[7]->value()/1000.0, v);
							//		cout<<"Go to safe back"<<endl;
						} else {
							if(!confirmedPress){
								confirmedPress = true;
								
								ofxVec3f camDir;
								if(v == ofxIndustrialRobotDefines::Up){
									camDir = buttonDir+ofxVec3f(0,-0.1,0);
								}
								
								if(v == ofxIndustrialRobotDefines::Down){
									camDir = buttonDir+ofxVec3f(0,0.1,0);
								}								 
								
								if(robot->industrialRobot->setGravityTarget(safeDist+camOffset,camDir, parSliders[6]->value()/1000.0, 3.0*parSliders[7]->value()/1000.0, v)){
									
								} else {
									cout<<"Could not add confirmPos"<<endl;
								}
							} else {								
//								cout<<"Check light: "<<isLightOn()<<"   "<<lightTurnedOn[curButton]<<endl;
								if(isLightOn() != lightTurnedOn[curButton] || buttonRepeat > 8 ){
									lightTurnedOn[curButton] = !lightTurnedOn[curButton];
									if(lightTurnedOn[curButton ])
										checkboxes[curButton]->setCheckState(Qt::Checked);
									else 
										checkboxes[curButton]->setCheckState(Qt::Unchecked);

									buttonRepeat = 0;
								} else {
									buttonRepeat ++;
									/*		if(lightTurnedOn[curButton])
									 checkboxes[curButton]->setCheckState(Qt::Unchecked);
									 else 
									 checkboxes[curButton]->setCheckState(Qt::Checked);
									 */
								}
								pressedTimer = 0;
								//						curButton ++;
								lastButton = curButton;
								curButton = -1;
								//							curButton = ofRandom(0, 95);
								//		curButton ++;
								/*		if(curButton > s.length()-1){
								 curButton = 0;
								 rate = 0;
								 }*/
								//	curButton = int( ofRandom(0, 10));
								//	curRow = int(ofRandom(0, 3));
								cout<<"Done"<<endl;
							}
						}
					}
				}
			} 
			else {
				//No button
				/*	v = ofxIndustrialRobotDefines::Up;
				 ofxVec3f safeDist = cornerPos(lastButton) - cornerDir(lastButton)*300.0;
				 robot->industrialRobot->setGravityTarget(safeDist, cornerDir(0), 0.1, 1.0, v);
				 */
			}
		}
		
	}
	
	isLightOnImg = robotData->srcImgGray;
	isLightOnImg.threshold(sliderLightOnThreshold1->value());
	//isLightOn();
}

ofxVec3f ButtonMatrixMode::cornerPos(int n){
	return ofxVec3f(*positionInformation[n*6], *positionInformation[n*6+1], *positionInformation[n*6+2] );
}
ofxVec3f ButtonMatrixMode::cornerDir(int n){
	//	cout<<*positionInformation[n*6]+3<<endl;
	int row = n / 12;
	int col = n-row*12;
	if(col < 2){
		return ofxVec3f(0.2,-0.09,0.95).normalized();		
	} else {
		return ofxVec3f(0.0,-0.09,0.95).normalized();	
	}
	
	//	return ofxVec3f(*positionInformation[n*6+3], *positionInformation[n*6+4], *positionInformation[n*6+5] )+ofxVec3f(0.1,0,0);	
}

ofxVec3f ButtonMatrixMode::buttonPosition(int n){
	int row = n / 12;
	int col = n-row*12;
	
	//	ofxVec3f  xdir = ((cornerPos(1)+ofxVec3f(0,20,0)) - cornerPos(0))/11.0;
	//	ofxVec3f ydir = ((cornerPos(2)) - cornerPos(0))/7.0;
	
	ofxVec3f pos = cornerPos(0) + xdir()*col + ydir()*row;
	if(col < 6){
		if(col%2 == 1){
			pos -= ofxVec3f(0,10,0);
		}
	} else {
		pos -= ofxVec3f(0,10,0);
		if(col%2 == 1){
			pos -= ofxVec3f(0,10,0);
		}
	}
	return pos;
	
}


ofxVec3f ButtonMatrixMode::xdir(){
	return ((cornerPos(1)+ofxVec3f(0,20,0)) - cornerPos(0))/11.0;
}
ofxVec3f ButtonMatrixMode::ydir (){ 
	return ((cornerPos(2)) - cornerPos(0))/7.0;
}


bool ButtonMatrixMode::isPointInBoundingbox(ofxVec3f p, float depth){
	
	ofxVec3f p0 = cornerPos(0);
	ofxVec3f p1 = cornerPos(1);
	ofxVec3f p2 = cornerPos(2)+xdir()*11;
	ofxVec3f p3 = cornerPos(2);
	//	cout<<p0.z<<"  "<<p1.z<<"  "<<p2.z<<"   "<<p3.z<<endl;
	
	float xPercent = (p.x-p0.x)/(p1.x-p0.x);
	float yPercent = (p.y-p0.y)/(p0.y-p3.y);
	
	float dz = (p1.z-p0.z);
	float dyz = (p3.z-p0.z);
	
	float bz = p0.z + dyz*yPercent +dz*xPercent-depth;// - depth+robot->industrialRobot->getHandLength() + (robot->industrialRobot->getCurrentDir()*robot->industrialRobot->getHandLength()).z; 
	if(robot->industrialRobot->getCurrentDir().z < 0){
		bz += (robot->industrialRobot->getCurrentDir()*robot->industrialRobot->getHandLength()).z;
	}
	if(p.z > bz){
		return true;
	}
	return false;
	
	//	cout<<bz<<endl;
	
	//	if(p.
}


void ButtonMatrixMode::pictureReady(bool boolPoints[])
{
	
	int numChanges=0;
	for(int x=0;x < LIGHTS_WIDTH*LIGHTS_HEIGHT;x++){
		if(boolPoints[x] !=  lightTurnedOn[x]){
			numChanges++;
		}
	}
	
	if(numChanges > 4){
		rate = 90;

		int n = 0;
		
		for(int x=0;x < LIGHTS_WIDTH*LIGHTS_HEIGHT;x++)
			
		{
			
			if(boolPoints[n])
			{
				//cout << "true";	
			}		
			if(boolPoints[n] !=  lightTurnedOn[n])
			{
				checkboxes[n]->setCheckState(Qt::PartiallyChecked);
			}
			if(boolPoints[n] ==  lightTurnedOn[n])
			{
				if(boolPoints[n]){
					checkboxes[n]->setCheckState(Qt::Checked);
				} else {
					checkboxes[n]->setCheckState(Qt::Unchecked);			
				}
				
			}
			
			n++;
			
		}
	}
	
	
	
}

bool ButtonMatrixMode::isLightOn()
{
	
	cout << "lights ont:" << sliderLightOnThreshold2->value() <<"   "<< cvCountNonZero(isLightOnImg.getCvImage()) << "\n";
	
	return sliderLightOnThreshold2->value() < cvCountNonZero(isLightOnImg.getCvImage());
	
}
