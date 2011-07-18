/*---------------------------------------------------
Open Frameworks 0.05
graphical user interface - Dennis Rosenfeld-2008
*///--------------------------------


#include "UiObject.h"


UiObject::UiObject()//constructor
{
}

UiObject::~UiObject()//destructor
{
}

void    UiObject::initialize(string name,int x,int y,int w, int h,int idNum)
{
    loadDefaults( name, x, y, w,  h, idNum);
}
void    UiObject::loadDefaults(string name,int x,int y,int w, int h,int idNum)
{
    myX = x;
    myY = y;
    myWidth = w;
    myHeight = h;
    myName = name;

    myState = false;
    myValue = 0;
    myText = "--";

    myID = idNum;
    myType = "NOTHING";

    isDraggable = false;
    myKey  = 999;
    myKey2 = 999;


    //glColor4f( 0.5f, 0.3f, 0.0f,0.9f );

     colorBase[0] = 0.2f;
     colorBase[1] = 0.2f;
     colorBase[2] = 0.2f;
     colorBase[3] = 0.9f;

    //glColor4f( 0.8f, 0.6f, 0.2f,1.0f );
     colorHighlight1[0] = colorBase[0] + 0.3f;
     colorHighlight1[1] = colorBase[1] + 0.3f;
     colorHighlight1[2] = colorBase[2] + 0.2f;
     colorHighlight1[3] = 1.0f;

    //glColor4f( 0.9f, 0.7f, 0.2f,1.0f );
     colorHighlight2[0] = colorBase[0] + 0.4f;
     colorHighlight2[1] = colorBase[1] + 0.4f;
     colorHighlight2[2] = colorBase[2] + 0.2f;
     colorHighlight2[3] = 1.0f;

    //glColor4f( 0.6f, 0.4f, 0.1f,0.9f );
     colorOver[0] = colorBase[0] + 0.1f;
     colorOver[1] = colorBase[1] + 0.1f;
     colorOver[2] = colorBase[2] + 0.1f;
     colorOver[3] = colorBase[3];
     //float  colorHighlight2[4];

    //myFont.loadFont("fonts/Helvetica Neue.ttf",12, true, false);
}

void    UiObject::update()
{


}


string    UiObject::getValueString()
{
    return myText;
}

bool    UiObject::getValueBool()
{
    return myState;
}


float    UiObject::getValueFloat()
{
    return myValue;
}

int    UiObject::getValueInt()
{
    return int(myValue);
}


int      UiObject::getID()
{
        return myID;
}

int      UiObject::getHeight()
{
        return myHeight;
}

int      UiObject::getWidth()
{
        return myWidth;
}


void    UiObject::draw()
{
    /*
        if (myState == true)
        {
         ofFill();
         ofSetColor(25,25,180);
         ofRect(myX, myY, myWidth, myHeight);
        }

        if( HoverOver == true)
        {
            ofRect(myX-10, myY-10, myWidth+20, myHeight+20);
        }

        ofNoFill();
        ofSetColor(255,255,255);
        ofRect(myX, myY, myWidth, myHeight);
        ofDrawBitmapString(myName, myX+4, myY +13);
        */
}

bool    UiObject::checkBoundaries(int x, int y) //check to see if mouse is within boundaries of object.
{
    bool withinArea = false;
    if ( x > myX && x < myX + myWidth && y > myY && y < myY + myHeight)
    {
        withinArea = true;
    }
    return withinArea;
}

string UiObject::checkType()
{
    return myType;
}
bool    UiObject::keyPressOK()
{
    return true;
}

void     UiObject::setLine (bool OnOff)
{
    if (OnOff == true)
        {HoverOver = true; }
    else
        {HoverOver = false; }

}

void  UiObject::press()
{
 //placeholder function;
}

void  UiObject::dePress()
{
 //placeholder function;
}



void     UiObject::release()
{
//placeholder function;
}

void     UiObject::drag(int x, int y)
{
//placeholder function;
}

void    UiObject::keyPressed(int key)
{
    //if (key == myKey)
    //{
        //do something
    //}

    //if (key == myKey2)
    //{
        //do something else
    //}

    //cout << myKey;
}

void    UiObject::keyReleased(int key)
{
    //if (key == myKey)
    //{
        //do something;
    //}
}

void    UiObject::addItem(int idNum, string name)
{
       //placeholder function;
}

void       UiObject::bindKey(int key)
{
    myKey = key;
    //cout << "binding key:";
    //cout << myKey;
    //cout << " \n";
}

void       UiObject::bindKey2(int key)//adds a second key for cycling through menus - up and down, etc.
{
    myKey2 = key;
    //cout << "binding key2:";
    //cout << myKey;
    //cout << " \n";
}

void            UiObject::setFont( string fontString, int fontSize)
{
    myFont.loadFont(fontString,fontSize, true,false);
}

bool    UiObject::listenForTrigger()
{
       //placeholder function;
}
