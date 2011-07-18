/*---------------------------------------------------
Open Frameworks 0.05
graphical user interface - Dennis Rosenfeld-2008
*///--------------------------------


#include "GuiPanel.h"


GuiPanel::GuiPanel()//constructor
{
}

GuiPanel::~GuiPanel()//destructor
{
}


void    GuiPanel::initialize(string name,int x,int y,int w, int h)
{
    myX = x;
    myY = y;
    myWidth = w;
    myHeight = h;
    myName = name;
    hidden = false;
    buttonSpacing = 18;
    fontString = "fonts/Helvetica Neue Bold.ttf";
    fontSize = 10;
    myFont.loadFont(fontString,fontSize, true,false);
    HoverOver = false;
}


void    GuiPanel::update()
{


}

//------------Create-Elements-------------------------------------------------
//-------------------------------------------------------------------------------
UiObject*      GuiPanel::addElement(string name,int w, int h, int idNum) //generic UI object. do not call.
{
    UiObject* newElement = new UiObject();
    int tempSize = calcSize();
    newElement->initialize(name,myX+15,myY + tempSize,w,h, idNum);
    newElement->setFont(fontString, fontSize);

    elements.push_back(newElement);
    adjustHeight(tempSize+h);
    return newElement;
}

UiObject*      GuiPanel::addSpacer(int h) //just add some space..
{
    UiObject* newElement = new UiObject();
    int tempSize = calcSize();
    newElement->initialize("spacer",myX+15,myY + tempSize,100,h, 777);
    newElement->setFont(fontString, fontSize);

    elements.push_back(newElement);
    adjustHeight(tempSize+h);
    return newElement;
}


UiButton*      GuiPanel::addButton(string name,int w, int h, string switchMode,bool textOnTop, int idNum, bool* pointValue) //create a buttons - switch or trigger type.
{
    UiButton* newButton = new UiButton();
    int tempSize = calcSize();
    newButton->initialize(name,myX+15,myY + tempSize,w,h,switchMode,textOnTop, idNum, pointValue);
    newButton->setFont(fontString, fontSize);

    elements.push_back(newButton);
    adjustHeight(tempSize+h);
    return newButton;
}


UiSlider*      GuiPanel::addFloatSlider(string name,int w, int h, float startVal, float MaxVal, int idNum, float * pointValue)//create a slider.
{
    UiSlider* newSlider = new UiSlider();
    int tempSize = calcSize();
    newSlider->initialize(name,myX+15,myY + tempSize,w,h, true,startVal,MaxVal, idNum, pointValue);
    newSlider->setFont(fontString, fontSize);

    elements.push_back(newSlider);
    adjustHeight(tempSize+h);
    return newSlider;
}

UiSlider*      GuiPanel::addIntSlider(string name,int w, int h, float startVal, float MaxVal, int idNum, int * pointValue)//create a slider.
{
    UiSlider* newSlider = new UiSlider();
    int tempSize = calcSize();
    newSlider->initialize(name,myX+15,myY + tempSize,w,h, false,startVal,MaxVal, idNum, pointValue);
    newSlider->setFont(fontString, fontSize);

    elements.push_back(newSlider);
    adjustHeight(tempSize+h);
    return newSlider;
}

UiMeter*      GuiPanel::addIntMeter(string name,int w, int h, float startVal, float MaxVal, int idNum, int * pointValue)//create a meter.
{
    UiMeter* newMeter = new UiMeter();
    int tempSize = calcSize();
    newMeter->initialize(name,myX+15,myY + tempSize,w,h, false,startVal,MaxVal, idNum, pointValue);
    newMeter->setFont(fontString, fontSize);
	
    elements.push_back(newMeter);
    adjustHeight(tempSize+h);
    return newMeter;
}


UiTextField*      GuiPanel::addTextField(string name,int w, int h, string  defaultText, int idNum,string * pointValue) //create a text field
{
    UiTextField* newTextField = new UiTextField();
    int tempSize = calcSize();
    newTextField->initialize(name,myX+15,myY + tempSize,w,h, defaultText, idNum, pointValue);
    newTextField->setFont(fontString, fontSize);

    elements.push_back(newTextField);
    adjustHeight(tempSize+h);
    return newTextField;
}


UiRadio*      GuiPanel::addRadio(string name, int h, int nColumns, int idNum, int * pointValue) //create a set of radio buttons
{
    UiRadio* newRadio = new UiRadio();
    int tempSize = calcSize();

    //newRadio->initialize(name, myX+15,myY + tempSize,w,h, numButtons, defaultChoice, idNum);
    newRadio->initialize(name, myX+15,myY + tempSize,h,nColumns,idNum, pointValue);
    newRadio->setFont(fontString, fontSize);

    elements.push_back(newRadio);
    adjustHeight(tempSize+h);
    adjustWidth(newRadio->getWidth() );
    //cout << newRadio->getWidth();
    return newRadio;
}


void      GuiPanel::addRadioElement(int idNum, string name) //set up the individual radio buttons
{
        for(int i = 0; i < elements.size(); i++)
        {
            if (elements[i]->getID() == idNum && elements[i]->checkType() == "RADIO")
            {
                elements[i]->addItem(idNum, name);
            }
        }
}



UiFileList*      GuiPanel::addFileList(string name,int w, int h, string directory,int idNum, string * pointValue)
{
    UiFileList* newFileList = new UiFileList();
    int tempSize = calcSize();
    newFileList->initialize(name,myX+15,myY + tempSize,w,h,directory, idNum, pointValue);
    newFileList->setFont(fontString, fontSize);

    elements.push_back(newFileList);
    adjustHeight(tempSize+h);
    return newFileList;
}


void      GuiPanel::bindKey(int idNum, int key) //associate a key press with a button or ui element.
{

        for(int i = 0; i < elements.size(); i++)
        {
            if (elements[i]->getID() == idNum)
            {
                elements[i]->bindKey(key);

            }
        }
}

void      GuiPanel::bindKey2(int idNum, int key) //associate a key press with a button or ui element.
{
        for(int i = 0; i < elements.size(); i++)
        {
            if (elements[i]->getID() == idNum)
            {
                elements[i]->bindKey2(key);

            }
        }
}
//-------------------------------------------------------------
//-------------------------------------------------------------
int     GuiPanel::calcSize()
{
    int tempSize = 0;
    for(int i = 0; i < elements.size(); i++)
        {
            int temp = elements[i]->getHeight();
            tempSize += temp+ buttonSpacing;
        }
    tempSize+= 30; //margin from top.
    return tempSize;


}

void     GuiPanel::adjustHeight(int amt)
{
    if (myHeight < amt )
    {
        myHeight = amt+20;
    }
}

void     GuiPanel::adjustWidth(int amt)
{
    if (myWidth < amt )
    {
        myWidth = amt+30;
        cout << "adjusting";
    }
}


void    GuiPanel::draw()
{
    if (hidden == false)
    {
        ofEnableAlphaBlending();
        ofSetColor(0,0,0,180);
        ofFill();
        //ofRect(myX, myY, myWidth, myHeight);
         //ofSetColor(255,255,255,240);



// draw a quad that fills the whole screen, that is red at
// the top and blue at the bottom
        glBegin(GL_QUADS);
        glColor4f( 0.0f, 0.0f, 0.0f,0.9f );
        glVertex3f( myX, myY, 0.0f );
        glVertex3f( myX + myWidth, myY, 0.0f );
        glColor4f( 0.0f, 0.0f, 0.0f,0.6f );
        glVertex3f(myX + myWidth, myY+myHeight, 0.0f );
        glVertex3f( myX,  myY+myHeight, 0.0f );
        glEnd();


        //glColor4f( 0.5f, 0.3f, 0.0f,0.9f );
        ofNoFill();
        ofSetColor(255,255,255,200);
        ofRect(myX, myY, myWidth, myHeight);


        myFont.drawString(myName, myX+17, myY +12);
        ofLine(myX,myY+20,myX + myWidth, myY+20);
        //ofDrawBitmapString(myName, myX+17, myY +10);

       // if( HoverOver == true)
       // {
         //   ofRect(myX-10, myY-10, myWidth+20, myHeight+20);
       // }
        for(int i = 0; i < elements.size(); i++)
        {
            elements[i]->draw();
        }
    }
}

bool    GuiPanel::checkBoundaries(int x, int y) //check to see if mouse is within boundaries of object.
{
    bool withinArea = false;
    if ( x > myX && x < myX + myWidth && y > myY && y < myY + myHeight)
    {
        withinArea = true;
    }
    return withinArea;
}

void     GuiPanel::setLine (bool OnOff)
{
    if (OnOff == true)
        {HoverOver = true; }
    else
        {HoverOver = false; }

}

//------------MOUSE-and KEYBOARD-------------------------------------------------
//-------------------------------------------------------------------------------

void    GuiPanel::mouseMoved(int x, int y)
{

    for (int i = 0; i < elements.size(); i++ )
    {
      if ( elements[i]->checkBoundaries(x,y) == true)
      {
        elements[i]->setLine(true);
      }
      else
        {
        elements[i]->setLine(false);
      }
    }
}

void    GuiPanel::mousePressed(int x, int y, int button)
{

    for (int i = 0; i < elements.size(); i++ )
    {
      if ( elements[i]->checkBoundaries(x,y) == true)
      {
        elements[i]->press();
      }
      else
      {
        if ( elements[i]->checkType() == "TEXTFIELD")
        {
            elements[i]->dePress();
        }
      }
    }
}

void    GuiPanel::mouseReleased()
{
    for (int i = 0; i < elements.size(); i++ )
    {
        elements[i]->release();
    }
}



void    GuiPanel::mouseDragged(int x, int y, int button)
{
    for (int i = 0; i < elements.size(); i++ )
    {
      if ( elements[i]->checkBoundaries(x,y) == true)
      {
        elements[i]->drag(x,y);
        //cout <<"should Drag";
      }
    }
}
