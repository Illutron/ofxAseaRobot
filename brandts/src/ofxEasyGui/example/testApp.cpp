#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(127,127,127);

    gui = new GuiHandler;
    GuiPanel*  Panel1 = gui->addPanel("panel 1",200,70,150,250);
    GuiPanel*  Panel2 = gui->addPanel("panel 2",380,70,250,230);
    GuiPanel*  Panel3 = gui->addPanel("panel 3",640,70,250,250);

        Panel1->addButton("button",100,30,"SWITCH",true,1, &myValue1);
            Panel1->bindKey(1,'a');
        Panel1->addButton("trigger",60,20,"TRIGGER",true,2, &myValue2);
            Panel1->bindKey(2,'b');


        Panel2->addFloatSlider("slider1",140,10, 26.8, 100, 3, &myValue3);
        Panel2->addIntSlider("slider2",140,10, 35, 50, 9, &myValue6);

        Panel2->addTextField("text1",140,20, "fileName", 4, &myString1);
        Panel2->addTextField("text2",140,20, "today's Date", 5,&myString2);

        Panel2->addRadio("radio",150,2, 6, &myValue4);
            Panel2->addRadioElement(6,"num1");
            Panel2->addRadioElement(6,"num2");
            Panel2->addRadioElement(6,"num3");
            Panel2->addRadioElement(6,"num4");
            Panel2->addRadioElement(6,"num5");
            Panel2->addRadioElement(6,"num6");
            Panel2->addRadioElement(6,"num7");
            Panel2->addRadioElement(6,"num8");
            //Panel2->addRadioElement(6,"nine");
            Panel2->bindKey(6,OF_KEY_RIGHT);
            Panel2->bindKey2(6,OF_KEY_LEFT);
        Panel2->addButton("switch",30,30,"SWITCH",false,7, &myValue5);
//
        Panel3->addFileList("fileBrowse",200,200,"files",8, &myString3);
           Panel3->bindKey(8,']');
            Panel3->bindKey2(8,'[');


}

//--------------------------------------------------------------
void testApp::update(){


        //ACTIVATING FUNCTION FROM A BUTTON PRESS:

        //ideally i would like to use an event, but for now:

        //triggers:
        bool fireTrigger = gui->listenForTrigger(2);
        if (fireTrigger == true)
        {
            testFunction(0);
        }

        //Switches:
        toggle1a = gui->listenForTrigger(7);
        if ( toggle1b != toggle1a)
        {
            if (toggle1a == true)
                { testFunction(1); }
            else
                { testFunction(2); }
            toggle1b = toggle1a;
        }

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(0xffffff);
	gui->draw();

	if (myValue1 == true)
	{    ofDrawBitmapString("Effect On", 10,10);	}
	else
	{   ofDrawBitmapString("Effect Off", 10,10);	}

    if (myValue2 == true)
	{   ofDrawBitmapString("Triger Pushed Down", 10,40);	}
	else
	{   ofDrawBitmapString("Trigger relased", 10,40);	}

    string temp = ofToString( myValue3 );
    ofDrawBitmapString(temp, 10,70);
    temp = ofToString( myValue6 );
    ofDrawBitmapString(temp, 10,90);

    ofDrawBitmapString(myString1, 10,110);
    ofDrawBitmapString(myString2, 10,130);
    temp = ofToString( myValue4 );
    ofDrawBitmapString(temp, 10,150);
    ofDrawBitmapString(myString3, 10,170);


}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){


}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(){

}
//--------------------------------------------------------------

void testApp::testFunction(int setting)
{
    cout << "test function  activated \n";
    string temp = "event num " + ofToString(setting) + "\n";
    cout << temp ;
}
