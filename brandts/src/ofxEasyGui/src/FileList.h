#ifndef FILELIST_H_INCLUDED
#define FILELIST_H_INCLUDED


#include "ofMain.h"

#include "ofxDirList.h"

#include "UiObject.h"


class   UiFileList: public UiObject
{
    //Select and load files from directory
    private:
        ofxDirList  DIR;
        int         mySelection;
        int         myLength;
        int         scrollHandlePos;
        int         NumOnScreenAtOnce;
        int         ySpacing;
        int         startfromPoint;
        bool        freeDrag;
        int         myLastX;
        int         myLastY;

        string *      myPointValue;

    public:
        UiFileList();                           //constructor
        ~UiFileList();                          //destructor
        void              initialize(string name,int x,int y,int w, int h, string directory,int idNum, string * pointValue);

        //METHODS:--------------------------------------------------------
        void            draw();
        void            drag(int x, int y);
        bool            checkBoundaries(int x, int y);
        void            release();
        void            press();
        void            keyPressed(int key);

};

#endif // FILELIST_H_INCLUDED
