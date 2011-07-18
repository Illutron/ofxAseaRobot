#ifndef SLIDER_H_INCLUDED
#define SLIDER_H_INCLUDED

#include "ofMain.h"

#include "UiObject.h"

class   UiSlider: public UiObject
{
    //Handles overall control of the User interface.
    protected:
        bool      isFloat;  //eihter "TRIGGER" or "SWITCH"
        float     myMaxValue;
        bool      freeDrag;
        int       textOffset;
        float *   myPointValue;
        int *     myPointValueINT;
	
		void updatePointValue();


    public:
        UiSlider();                           //constructor
        ~UiSlider();                          //destructor
        void             initialize(string name,int x,int y,int w, int h, bool hasDecimal, float startVal, float MaxVal, int idNum,float * pointValue);
        void             initialize(string name,int x,int y,int w, int h,  bool hasDecimal, float startVal, float MaxVal, int idNum, int * pointValueINT);

        //METHODS:--------------------------------------------------------


        void            draw();
        void            drag(int x, int y);
        bool            checkBoundaries(int x, int y);
        void            press();
        void            release();
        void            setFont( string fontString, int fontSize);
		void			setValue(int value);
		void			setValue(float value);

       // float           getValue();

};

/*
class   UiIntSlider: public UiSlider
{
    //Handles overall control of the User interface.
    protected:
        int *     myPointValue;


    public:
        UiIntSlider();                           //constructor
        ~UiIntSlider();                          //destructor
       // void             initialize(string name,int x,int y,int w, int h, bool hasDecimal, float startVal, float MaxVal, int idNum,int * pointValue);

        //METHODS:--------------------------------------------------------


       // void    draw();
};

*/

#endif // SLIDER_H_INCLUDED
