#ifndef RADIO_H_INCLUDED
#define RADIO_H_INCLUDED

#include "ofMain.h"

#include "UiObject.h"


class   UiRadio: public UiObject
{
    //Linked set of buttons.
    private:
        
        int      myLength;

        vector   <string> names;
        int      numColumns;
        bool     hoverOver;
        int     hoverNum;

        int buttonHeight;
        int spacer;
        int numRows;
        int remainder;
        int columnSpace;

        int *      myPointValue;




    public:
        UiRadio();                           //constructor
        ~UiRadio();                          //destructor
        //void             initialize(string name,int x,int y,int w, int h, int numButtons, int defaultChoice,int idNum);
        void             initialize(string name, int x,int y,int h, int nColumns, int idNum, int * pointValue);

        //METHODS:--------------------------------------------------------


        void            draw();
        void            addItem(int idNum, string name);
        bool            checkBoundaries(int x, int y);
        void            press();
        int             getValueInt();
        void            keyPressed(int key);
		void			select(int value);

};

#endif // RADIO_H_INCLUDED
