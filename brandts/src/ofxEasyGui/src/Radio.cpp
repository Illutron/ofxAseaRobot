#include "Radio.h"

UiRadio::UiRadio()//constructor
{


}

UiRadio::~UiRadio()//destructor
{
}


//void    UiRadio::initialize(string name,int x,int y,int w, int h, int numButtons, int defaultChoice,int idNum)
void      UiRadio::initialize(string name, int x,int y,int h, int nColumns, int idNum, int * pointValue)
{
    loadDefaults( name, x, y, 999,  h, idNum);
    myWidth = nColumns * 80;

    myType = "RADIO";
    //-------------specific
    myLength = 0;
    mySelection = 0;
    numColumns = nColumns;

    buttonHeight = 25;
    spacer = 5;
    numRows =  0;
    remainder =0;
   // cout << numRows;
    columnSpace = 100;

    hoverOver = false;
    hoverNum  = 0;

    myPointValue = pointValue;
    (*myPointValue) = mySelection;
}

void    UiRadio::draw()
{
//cout << numRows;

        int currentColumn = 0;
        int currentRow = 0;
        //ofSetCircleResolution(88);
        for (int i = 0; i < myLength; i++)
        {

            ofEnableSmoothing();
            ofRect(myX + ( currentColumn * columnSpace), myY + ((buttonHeight+spacer)*currentRow), buttonHeight, buttonHeight);
            //ofCircle(myX + ( currentColumn * columnSpace) + (buttonHeight/2), myY + ((buttonHeight+spacer)*currentRow)+10 + (buttonHeight/2), 10);


            myFont.drawString(names[i], myX + ( currentColumn * columnSpace)+ buttonHeight+5, myY + ((buttonHeight+spacer)*currentRow)+13);
            //ofDrawBitmapString(names[i], myX + ( currentColumn * columnSpace)+ buttonHeight+5, myY + ((buttonHeight+spacer)*currentRow)+30);
            if (hoverOver == true &&hoverNum == i)
            {
                glColor4f( colorHighlight2[0],colorHighlight2[1],colorHighlight2[2],colorHighlight2[3]);
                ofRect(myX + ( currentColumn * columnSpace)-3, myY + ((buttonHeight+spacer)*currentRow)-3, buttonHeight+6, buttonHeight+6);
                ofSetColor(255,255,255);
            }

            if (i == mySelection)
            {
                ofFill();
                glColor4f( colorBase[0], colorBase[1], colorBase[2],colorBase[3] );
                //ofSetColor(25,45,100);
                ofRect(myX + ( currentColumn * columnSpace)+3, myY + ((buttonHeight+spacer)*currentRow)+3, buttonHeight-7, buttonHeight-7);
                //ofCircle(myX + ( currentColumn * columnSpace) + (buttonHeight/2), myY + ((buttonHeight+spacer)*currentRow)+10 + (buttonHeight/2), 6);

                ofNoFill();
                ofRect(myX + ( currentColumn * columnSpace)+3, myY + ((buttonHeight+spacer)*currentRow)+3, buttonHeight-7, buttonHeight-7);
                //ofCircle(myX + ( currentColumn * columnSpace) + (buttonHeight/2), myY + ((buttonHeight+spacer)*currentRow)+10 + (buttonHeight/2), 6);
                ofSetColor(255,255,255);

            }

           int temp = numRows-1;
           if (remainder != 0 && currentColumn < remainder)
           {    temp++; }
           if (currentRow < temp)
            {
              currentRow++;
            }
            else
            {
                currentColumn++;
                currentRow = 0;
            }


        }


        //if( HoverOver == true)
        //{
            //glColor4f( 0.9f, 0.7f, 0.2f,1.0f );
         //   ofRect(myX-5, myY-5, myWidth+10, myHeight+10);
        //}
        ofNoFill();
        ofSetColor(255,255,255);
        //ofRect(myX, myY, myWidth, myHeight);


         myFont.drawString(myName, myX+4, myY-5);
         ofDisableSmoothing();
       // ofDrawBitmapString(myName, myX+4, myY +13);

        //cout << myLength;

}

void    UiRadio::addItem(int idNum, string name)
{
    myLength++;
    numRows = myLength /numColumns ;
    remainder = myLength % numColumns;
    //cout << "adding \n";
    names.push_back(name);

}

bool    UiRadio::checkBoundaries(int x, int y) //check to see if mouse is within boundaries of object.
{
    bool withinArea = false;
    hoverOver = false;

    //cout << y;
    //cout << "\n";
    if ( x > myX && x < myX + myWidth && y > myY && y < myY + myHeight)
    {
        withinArea = true;
        int currentColumn = 0;
        int currentRow = 0;

        for (int i = 0; i < myLength; i++)
        {

            //ofRect(myX + ( currentColumn * columnSpace)+3, myY + ((buttonHeight+spacer)*currentRow)+3, buttonHeight-7, buttonHeight-7);
            //ofRect(myX + ( currentColumn * columnSpace), myY + ((buttonHeight+spacer)*currentRow), buttonHeight, buttonHeight);
            //if ( x > myX  && x < myX + buttonHeight && y < myY + 10 +buttonHeight && y > myY + +10)

            if ( x > myX + ( currentColumn * columnSpace) && x < myX + ( currentColumn * columnSpace) + buttonHeight && y < myY + ((buttonHeight+spacer)*currentRow) +buttonHeight && y > myY + ((buttonHeight+spacer)*currentRow))
            {
                hoverOver = true;
                hoverNum = i;
            }
           int temp = numRows-1;
           if (remainder != 0 && currentColumn < remainder)
           {    temp++; }
           if (currentRow < temp)
            {
              currentRow++;
            }
            else
            {
                currentColumn++;
                currentRow = 0;
            }
        }
    }
    return withinArea;
}

void    UiRadio::press()
{
    if (hoverOver == true)
    {
        mySelection = hoverNum;
        (*myPointValue) = mySelection;
    }
}

int    UiRadio::getValueInt()
{
    return mySelection;
}

void    UiRadio::keyPressed(int key)
{
    if (key == myKey)
    {
        mySelection++ ;
        if ( mySelection >= myLength)
        {
            mySelection = myLength-1;
        }
        (*myPointValue) = mySelection;
    }
    else if (key == myKey2)
    {
        mySelection-- ;
        if ( mySelection < 0)
        {
            mySelection = 0;
        }
        (*myPointValue) = mySelection;
    }
}

void UiRadio::select(int value){
	if(value >= 0 && value < myLength){
		mySelection = value;
		(*myPointValue) = mySelection;
	}
}
