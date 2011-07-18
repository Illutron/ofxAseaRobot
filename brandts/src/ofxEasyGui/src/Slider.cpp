#include "Slider.h"

UiSlider::UiSlider()//constructor
{


}

UiSlider::~UiSlider()//destructor
{
}


void    UiSlider::initialize(string name,int x,int y,int w, int h,  bool hasDecimal, float startVal, float MaxVal, int idNum, float * pointValue)
{
    loadDefaults( name, x, y, w,  h, idNum);

    myValue = startVal;

    myPointValue = pointValue;
       (*myPointValue) = myValue;

    myMaxValue = MaxVal;
    myType = "SLIDER";

    freeDrag = false;
    isFloat = hasDecimal;
    textOffset = 5;
}

void    UiSlider::initialize(string name,int x,int y,int w, int h,  bool hasDecimal, float startVal, float MaxVal, int idNum, int * pointValueINT)
{
    loadDefaults( name, x, y, w,  h, idNum);

    myValue = startVal;

    myPointValueINT = pointValueINT;
       (*myPointValueINT) = int(myValue);

    myMaxValue = MaxVal;
    myType = "SLIDER";

    freeDrag = false;
    isFloat = hasDecimal;
    textOffset = 5;
}


void UiSlider::draw()
{

        float percent = (myValue / myMaxValue);
        int slideWidth =int (percent*myWidth);
       // cout <<myValue;
        //cout << "  \n";
       // cout <<myMaxValue;
        //cout << "  \n";

        ofFill();


        glColor4f( colorHighlight2[0],colorHighlight2[1],colorHighlight2[2],colorHighlight2[3]);
        ofRect(myX, myY+textOffset, slideWidth, myHeight);

        ofNoFill();

        string temp ="";
        if ( isFloat == true)
        {   temp = ofToString( myValue,2); }
        else
        {   temp = ofToString( int(myValue) ); }
        string temp2 =myName + ": " +temp;

        if( HoverOver == true)
        {
            ofRect(myX-5, myY-5+textOffset, myWidth+10, myHeight+10);
            glColor4f( 0.0f, 0.0f, 0.0f,0.8f );
            ofFill();
            int textWidth = int (myFont.stringWidth(temp2) );
            ofRect(myX+4, myY-10+textOffset, textWidth, 10);
            ofNoFill();
        }


        ofSetColor(255,255,255);
        ofRect(myX, myY+textOffset, myWidth, myHeight);


        myFont.drawString(temp2, myX+4, myY);
        //ofDrawBitmapString(temp2, myX+4, myY -7);

}


bool    UiSlider::checkBoundaries(int x, int y) //check to see if mouse is within boundaries of object.
{

    bool withinArea = false;
    if ( freeDrag == false)
    {
        if ( x > myX && x < myX + myWidth && y > myY && y < myY + myHeight+textOffset)
        {
            withinArea = true;
        }
    }
    else //true
    {
        if ( x > myX - 10 && x < myX + myWidth +10 )
        {
            withinArea = true;
        }
    }
    return withinArea;
}

void    UiSlider::press()
{
       freeDrag = true;
}
void    UiSlider::drag(int x, int y)
{
 if (freeDrag == true)
 {
    float temp =  x-myX;
    float  temp2 = temp/myWidth;
    if (temp2 < 0)
        {temp2 = 0; }
    myValue = temp2 * myMaxValue;
    if (temp2 > 1)
        {myValue = myMaxValue; }
	this->updatePointValue();
 }
}


void    UiSlider::release()
{
    freeDrag = false;
}

void    UiSlider::setFont( string fontString, int fontSize)
{
    myFont.loadFont(fontString,fontSize-2, true,false);
}

void UiSlider::setValue(int value){
	this->myValue = (float)value;
	this->updatePointValue();
}

void UiSlider::setValue(float value){
	this->myValue = value;
	this->updatePointValue();
}

void UiSlider::updatePointValue(){
    if ( isFloat == true)
    { (*myPointValue) = myValue; }
    else
    {   (*myPointValueINT) = int(myValue); }
}


//------------------------------------------------------------------
//------------------------------------------------------------------
//------------------------------------------------------------------
/*
UiSlider::UiSlider()//constructor
{


}

UiSlider::~UiSlider()//destructor
{
}
*/
