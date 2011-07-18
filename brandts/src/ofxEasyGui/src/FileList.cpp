#include "FileList.h"


UiFileList::UiFileList()//constructor
{


}

UiFileList::~UiFileList()//destructor
{
}

void    UiFileList::initialize(string name,int x,int y,int w, int h, string directory,int idNum, string * pointValue)
{
    loadDefaults( name, x, y, w,  h, idNum);

    myType = "FILELIST";
    isDraggable = false;

	 DIR.setVerbose(false);
     myLength = DIR.listDir("../"+ directory);
     mySelection = 0;

     if (  myLength >0 )
     {
        myText = DIR.getName(mySelection);
     }
     else
     {
        myText = "EMPTY";
     }
     scrollHandlePos = 25;
     ySpacing = 20;
     NumOnScreenAtOnce = myHeight/ySpacing;
     startfromPoint = 0;
     myLastX = 0;
     myLastY = 0;

    myPointValue = pointValue;
    (*myPointValue) = myText;
}


void    UiFileList::draw()
{
        ofNoFill();
        ofSetColor(255,255,255);
        //ofDrawBitmapString(myName, myX-5,myY - 5);
        ofRect(myX, myY, myWidth, myHeight);
        //------------------------------------

        for(int i = 0; i < myLength; i++)
        {
            if (i >= startfromPoint && i < startfromPoint + NumOnScreenAtOnce)
            {
                int temp = i - startfromPoint;
                string fileInfo = DIR.getName(i);
                 ofSetColor(255,255,255);
                 myFont.drawString(fileInfo, myX,myY + (temp *ySpacing) +13 );
                //ofDrawBitmapString(fileInfo, myX,myY + (temp *ySpacing) +13 );

                if (i == mySelection)
                    {
                            ofEnableAlphaBlending();
                            ofFill();
                            ofSetColor(255,255,255,100);
                            ofRect(myX, myY + (temp *ySpacing), myWidth-20, ySpacing);
                            ofDisableAlphaBlending();
                            ofNoFill();
                    }
            }
        }
        //draw scrollbar
                glColor4f( 0.0f, 0.0f, 0.0f,0.7f );
                ofFill();
                ofRect(myX + myWidth - 20, myY , 15, myHeight);
                ofSetColor(255,255,255);
                ofNoFill();
                ofRect(myX + myWidth - 20, myY , 15, myHeight);

                glColor4f( colorBase[0], colorBase[1], colorBase[2],colorBase[3] );
                ofFill();
                ofRect(myX + myWidth - 20, myY + scrollHandlePos , 14, 25);
                ofSetColor(255,255,255);
                ofNoFill();
                ofRect(myX + myWidth - 20, myY + scrollHandlePos , 14, 25);

}

bool    UiFileList::checkBoundaries(int x, int y) //check to see if mouse is within boundaries of object.
{

    myLastX = x;
    myLastY = y;
    bool withinArea = false;
    if ( freeDrag == false)
    {
        if ( x > myX && x < myX + myWidth && y > myY && y < myY + myHeight)
        {
            withinArea = true;
        }
    }
    else //true
    {
        if ( y > myY - 10 && y < myY + myHeight +10 )
        {
            withinArea = true;
        }
    }
    return withinArea;
}

void    UiFileList::drag(int x, int y)
{
    if  (x > myX + myWidth - 20 && x < myX + myWidth && y > myY && y < myY + myHeight)
        {freeDrag = true;    }
    if ( freeDrag == true)
    {
        float temp =  y-myY;
        scrollHandlePos = temp;
        float temp2 = temp/(myHeight-25);
        if (temp2 < 0)
            {
                temp2 = 0;
                scrollHandlePos = 0;
            }
        startfromPoint = int (temp2 * (myLength-NumOnScreenAtOnce) );
        if (temp2 > 1)
            {
                startfromPoint = (myLength-NumOnScreenAtOnce);
                scrollHandlePos = myHeight-25;
            }
    }
}



void    UiFileList::press()
{
    if ( myLastX > myX && myLastX < myX + myWidth-20 && myLastY > myY && myLastY < myY + myHeight)
    {
     float temp = myLastY-myY;
     float temp2 = temp/myHeight;
     int   temp3 = int(NumOnScreenAtOnce*temp2);
     mySelection = startfromPoint +temp3;


     if (  myLength >0 )
     {
        myText = DIR.getName(mySelection);
     }
     else
     {
        myText = "EMPTY";
     }
     (*myPointValue) = myText;
    }
}

void    UiFileList::release()
{
    freeDrag = false;
}

void    UiFileList::keyPressed(int key)
{
    if (key == myKey)
    {
        mySelection++ ;
        if ( mySelection >= myLength)
        {
            mySelection = myLength-1;
            scrollHandlePos = myHeight-25;
        }

        else if ( mySelection > NumOnScreenAtOnce+ startfromPoint-1)
        {
            startfromPoint++;
            float temp = mySelection / (float)myLength;
            float temp2 = temp* (myHeight-25);
            scrollHandlePos = int (temp2);
        }

         if (  myLength >0 )
         {
            myText = DIR.getName(mySelection);
         }
         else
         {
            myText = "EMPTY";
         }
        (*myPointValue) = myText;

    }
    else if (key == myKey2)
    {
        mySelection-- ;
        if ( mySelection < 0)
        {
            mySelection = 0;
        }
        else if ( mySelection < startfromPoint)
        {
            startfromPoint--;
            float temp = mySelection / (float)myLength;
            float temp2 = temp* (myHeight-25);
            scrollHandlePos = int (temp2);
        }

        if (  myLength >0 )
         {
            myText = DIR.getName(mySelection);
         }
         else
         {
            myText = "EMPTY";
         }
        (*myPointValue) = myText;
    }
}
