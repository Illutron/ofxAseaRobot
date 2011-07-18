#include "Button.h"

UiButton::UiButton()//constructor
{


}

UiButton::~UiButton()//destructor
{
}


void    UiButton::initialize(string name,int x,int y,int w, int h, string bmode, bool textOnTop, int idNum, bool * pointValue)
{
    loadDefaults( name, x, y, w,  h, idNum);

    myType = "BUTTON";
    isDraggable = false;
    myMode = bmode;
    textOverButton = textOnTop;
    fireTrigger = false;

    myPointValue = pointValue;
	myState = *myPointValue;
}


void    UiButton::draw()
{

        ofFill();
        ofSetColor(255,255,255);
        ofRect(myX-2, myY-2, myWidth+4, myHeight+4);
        ofSetColor(25,45,100);
        glColor4f( colorBase[0], colorBase[1], colorBase[2],colorBase[3] );
        ofRect(myX, myY, myWidth, myHeight);
        //ofRect(myX, myY, myWidth, myHeight);

        // draw a quad that fills the whole screen, that is red at
        // the top and blue at the bottom
/*
        glBegin(GL_QUADS);
        glColor4f( 0.8f, 0.8f, 0.8f,0.0f );
        glVertex3f( myX, myY, 0.0f );
        glVertex3f( myX + myWidth, myY, 0.0f );

        glColor4f( 1.0f, 1.0f, 1.0f,0.2f );
        glVertex3f(myX + myWidth, myY+(myHeight/2), 0.0f );
        glVertex3f( myX,  myY+(myHeight/2), 0.0f );
        glEnd();

*/
        if (myState == true)
        {
         ofFill();
         glColor4f( colorHighlight1[0],colorHighlight1[1],colorHighlight1[2],colorHighlight1[3]);
         ofRect(myX, myY, myWidth, myHeight);
        }

        if( HoverOver == true)
        {
           // ofNoFill();


            if (myState == true)
            {
                glColor4f( colorHighlight2[0],colorHighlight2[1],colorHighlight2[2],colorHighlight2[3]);
            }
            else
            {
                glColor4f( colorOver[0], colorOver[1], colorOver[2],colorOver[3] );
            }
            ofRect(myX, myY, myWidth, myHeight);

        }


       ofSetColor(255,255,255);
        int textWidth = int (myFont.stringWidth(myName) );
        textWidth = (myWidth-textWidth)/2;
        //int textHeight = myFont.stringHeight(myName);
        //textHeight = (myHeight-textHeight);
        if (textOverButton == true)
        {
            myFont.drawString(myName, myX + textWidth, myY +(myHeight/2)+2 );
        }
        else
        {
            myFont.drawString(myName, myX + myWidth+10, myY +(myHeight/2)+2 );
        }
        //ofDrawBitmapString(myName, myX+4, myY +13);
        ofNoFill();

}

void    UiButton::press()
{
    if (myMode == "SWITCH")
    {
        if (myState == true)
            {myState = false;   }
        else
            {myState = true;    }

    }
    else if (myMode == "TRIGGER")
    {
        myState = true;
        fireTrigger = true;
        //cout << "press \n";
    }

      (*myPointValue) = myState;
}


void    UiButton::release()
{
    if (myMode == "TRIGGER")
    {
        myState = false;
        (*myPointValue) = myState;
    }
}

void    UiButton::keyPressed(int key)
{
    if (key == myKey)
    {

        if (myMode == "SWITCH")
        {
            if (myState == true)
                {myState = false;   }
            else
                {myState = true;    }
        }
        else if (myMode == "TRIGGER")
        {
            myState = true;
            fireTrigger = true;
        }

        (*myPointValue) = myState;
    }
}

void    UiButton::keyReleased(int key)
{
    if (key == myKey)
    {
        if (myMode == "TRIGGER")
        {
            myState = false;
        }
        (*myPointValue) = myState;
    }
}

bool    UiButton::getValueBool()
{
    bool tempState;

    if (myMode == "TRIGGER")
    {
        tempState= fireTrigger;
        fireTrigger = false;
    }
    else
    {
        tempState= myState;
    }

    return tempState;
}

bool  UiButton::listenForTrigger()
{

    bool tempState;

    if (myMode == "TRIGGER")
    {
        tempState= fireTrigger;
        fireTrigger = false;
    }
    else
    {
        tempState= myState;
    }

    return tempState;
}

void UiButton::setState(bool state){
	if (myMode == "SWITCH"){
		this->myState = state;
	}
	else if (myMode == "TRIGGER")
	{
		this->myState = state;
		if(this->myState){
			this->fireTrigger = true;
		}
	}
	
	(*myPointValue) = this->myState;
}
