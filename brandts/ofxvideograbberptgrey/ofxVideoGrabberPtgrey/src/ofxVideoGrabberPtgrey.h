#ifndef _OFX_VIDEO_GRABBER_PTGREY
#define _OFX_VIDEO_GRABBER_PTGREY


#if defined( TARGET_OSX ) || defined( TARGET_LINUX )
#include "ofxVideoGrabberDc1394.h"
#elif defined( TARGET_WIN32 )
#include "ofxVideoGrabberFlyCapture.h"
#endif


#endif

