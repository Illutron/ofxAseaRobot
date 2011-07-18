#include "TextField.h"

UiTextField::UiTextField()//constructor
{


}

UiTextField::~UiTextField()//destructor
{
}


void    UiTextField::initialize(string name,int x,int y,int w, int h, string defaultText, int idNum, string * pointValue)
{

    loadDefaults( name, x, y, w,  h, idNum);
    myType = "TEXTFIELD";
    myText = defaultText;
    myDefaultText = defaultText;
    inTextEnterMode = false;

    myPointValue = pointValue;
    (*myPointValue) = myText;

}

void    UiTextField::draw()
{
        if(inTextEnterMode == true)
        {
            ofFill();
            glColor4f( 0.9f, 0.7f, 0.2f,1.0f );
            ofRect(myX, myY, myWidth, myHeight);
        }

        ofNoFill();
        ofSetColor(255,255,255);
        ofRect(myX, myY, myWidth, myHeight);
        myFont.drawString(myText, myX+4, myY +13);
       // ofDrawBitmapString(myText, myX+4, myY +13);
}

void    UiTextField::press()
{
    if (inTextEnterMode == false)
    {    replaceAllText = true; }
    inTextEnterMode = true;
}

void    UiTextField::dePress()
{
    inTextEnterMode = false;

    if (myText.size() == 0)
    {
       myText = myDefaultText;
    }
}

void    UiTextField::keyPressed(int key)
{
    if (inTextEnterMode == true)
    {
        if (replaceAllText == true)
        {
            myText = "";
            replaceAllText = false;
        }

        if(key == OF_KEY_DEL || key == OF_KEY_BACKSPACE)
        {
            //textTest.resize(text, padChar);
            if (myText.size() >0)
            {
                myText.erase( myText.size()-1);
            }
            // textTest = textTest - 1;
            }
            else if (key == OF_KEY_RETURN)
            {
                inTextEnterMode = false;
            }
            else
            {
                myText += key;
            }
    }
    (*myPointValue) = myText;
}

bool    UiTextField::keyPressOK()
{
    bool isOK;
    if (inTextEnterMode == true)
    {
        isOK = false;
    }
    else
    {
        isOK = true;
    }
    return isOK;
}
