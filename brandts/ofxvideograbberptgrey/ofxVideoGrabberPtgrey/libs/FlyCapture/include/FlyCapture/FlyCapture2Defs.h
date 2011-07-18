//=============================================================================
// Copyright � 2008 Point Grey Research, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of Point
// Grey Research, Inc. ("Confidential Information").  You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with Point Grey Research, Inc. (PGR).
//
// PGR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. PGR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================

//=============================================================================
// $Id: FlyCapture2Defs.h,v 1.138 2009/06/19 16:59:10 hirokim Exp $
//=============================================================================

#ifndef PGR_FC2_FLYCAPTURE2DEFS_H
#define PGR_FC2_FLYCAPTURE2DEFS_H

#include <memory.h> 

//=============================================================================
// Definitions file for FlyCapture2. 
//
// Holds structures, enumerations and other global definitions that are used 
// across the entire FlyCapture2 API.
//=============================================================================

#ifndef NULL
#define NULL 0
#endif

#ifndef FULL_32BIT_VALUE
#define FULL_32BIT_VALUE 0x7FFFFFFF
#endif 

namespace FlyCapture2
{

    //==========================================================================
    // Global constants
    //==========================================================================

    /** The maximum length that is allocated for a string. */
    static const unsigned int sk_maxStringLength = 512;       

    //==========================================================================
    // Typedefs
    //========================================================================== 

    /** A GUID to the camera.  It is used to uniquely identify a camera. */
    class PGRGuid
    {
    public:
        unsigned int value[4];

        /** Constructor. */
        PGRGuid()
        {
            value[0] = 0;
            value[1] = 0;
            value[2] = 0;
            value[3] = 0;
        }

        /** Copy constructor. */
        PGRGuid( const PGRGuid& other )
        {
            this->value[0] = other.value[0];
            this->value[1] = other.value[1];
            this->value[2] = other.value[2];
            this->value[3] = other.value[3];
        }

        /** Assignment operator. */
        PGRGuid& operator=( const PGRGuid& other )
        {
            if( &other != this )
            {
                this->value[0] = other.value[0];
                this->value[1] = other.value[1];
                this->value[2] = other.value[2];
                this->value[3] = other.value[3];
            }
            
            return *this;
        }

        /** Equality operator. */
        bool operator==( const PGRGuid& guid )
        {
            if ( this->value[0] == guid.value[0] &&
                this->value[1] == guid.value[1] &&
                this->value[2] == guid.value[2] &&
                this->value[3] == guid.value[3] )
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        /** Inequality operator. */
        bool operator!=( const PGRGuid& guid )
        {
            return !(operator==( guid ));
        }
    };

    //==========================================================================
    // Enumerations
    //==========================================================================

    /** The error types returned by functions. */
    enum ErrorType
    {        
        PGRERROR_UNDEFINED = -1, /**< Undefined */
        PGRERROR_OK, /**< Function returned with no errors. */
        PGRERROR_FAILED, /**< General failure. */
        PGRERROR_NOT_IMPLEMENTED, /**< Function has not been implemented. */
        PGRERROR_FAILED_BUS_MASTER_CONNECTION, /**< Could not connect to Bus Master. */
        PGRERROR_NOT_CONNECTED, /**< Camera has not been connected. */
        PGRERROR_INIT_FAILED, /**< Initialization failed. */ 
        PGRERROR_NOT_INTITIALIZED, /**< Camera has not been initialized. */
        PGRERROR_INVALID_PARAMETER, /**< Invalid parameter passed to function. */
        PGRERROR_INVALID_SETTINGS, /**< Setting set to camera is invalid. */         
        PGRERROR_INVALID_BUS_MANAGER, /**< Invalid Bus Manager object. */
        PGRERROR_MEMORY_ALLOCATION_FAILED, /**< Could not allocate memory. */ 
        PGRERROR_LOW_LEVEL_FAILURE, /**< Low level error. */
        PGRERROR_NOT_FOUND, /**< Device not found. */
        PGRERROR_FAILED_GUID, /**< GUID failure. */
        PGRERROR_INVALID_PACKET_SIZE, /**< Packet size set to camera is invalid. */
        PGRERROR_INVALID_MODE, /**< Invalid mode has been passed to function. */
        PGRERROR_NOT_IN_FORMAT7, /**< Error due to not being in Format7. */
        PGRERROR_NOT_SUPPORTED, /**< This feature is unsupported. */
        PGRERROR_TIMEOUT, /**< Timeout error. */
        PGRERROR_BUS_MASTER_FAILED, /**< Bus Master Failure. */
        PGRERROR_INVALID_GENERATION, /**< Generation Count Mismatch. */
        PGRERROR_LUT_FAILED, /**< Look Up Table failure. */
        PGRERROR_IIDC_FAILED, /**< IIDC failure. */
        PGRERROR_STROBE_FAILED, /**< Strobe failure. */
        PGRERROR_TRIGGER_FAILED, /**< Trigger failure. */
        PGRERROR_PROPERTY_FAILED, /**< Property failure. */
        PGRERROR_PROPERTY_NOT_PRESENT, /**< Property is not present. */
        PGRERROR_REGISTER_FAILED, /**< Register access failed. */
        PGRERROR_READ_REGISTER_FAILED, /**< Register read failed. */
        PGRERROR_WRITE_REGISTER_FAILED, /**< Register write failed. */
        PGRERROR_ISOCH_FAILED, /**< Isochronous failure. */
        PGRERROR_ISOCH_ALREADY_STARTED, /**< Isochronous transfer has already been started. */
        PGRERROR_ISOCH_NOT_STARTED, /**< Isochronous transfer has not been started. */
        PGRERROR_ISOCH_START_FAILED, /**< Isochronous start failed. */
        PGRERROR_ISOCH_RETRIEVE_BUFFER_FAILED, /**< Isochronous retrieve buffer failed. */
        PGRERROR_ISOCH_STOP_FAILED, /**< Isochronous stop failed. */
        PGRERROR_ISOCH_SYNC_FAILED, /**< Isochronous image synchronization failed. */
        PGRERROR_ISOCH_BANDWIDTH_EXCEEDED, /**< Isochronous bandwidth exceeded. */
        PGRERROR_IMAGE_CONVERSION_FAILED, /**< Image conversion failed. */
        PGRERROR_IMAGE_LIBRARY_FAILURE, /**< Image library failure. */
        PGRERROR_BUFFER_TOO_SMALL, /**< Buffer is too small. */
        PGRERROR_FORCE_32BITS = FULL_32BIT_VALUE
    };  

    /** The type of bus callback to register a callback function for. */
    enum BusCallbackType
    {
        BUS_RESET, /**< Register for all bus events. */
        ARRIVAL, /**< Register for arrivals only. */
        REMOVAL, /**< Register for removals only. */
        CALLBACK_TYPE_FORCE_32BITS = FULL_32BIT_VALUE
    };    

    /** 
     * The grab strategy employed during image transfer. This type controls
     * how images that stream off the camera accumulate in a user buffer
     * for handling. 
     */
     /* 
       @remark Unlike earlier versions of the FlyCapture SDK, it is no longer 
       * necessary to explicitly start the image grabbing process before
       * specifying an image grabbing mode.
     */
    enum GrabMode
    {
        /**
         * Grabs the newest image in the user buffer each time the 
         * RetrieveBuffer() function is called. Older images are dropped 
         * instead of accumulating in the user buffer. Grabbing blocks if the 
         * camera has not finished transmitting the next available image. If
         * the camera is transmitting images faster than the application can
         * grab them, images may be dropped and only the most recent image 
         * is stored for grabbing. Note that this mode is the equivalent of 
         * flycaptureLockLatest in earlier versions of the FlyCapture SDK.
         */
        DROP_FRAMES,

        /**
         * Images accumulate in the user buffer, and the oldest image is 
         * grabbed for handling before being discarded. This member can be 
         * used to guarantee that each image is seen. However, image processing 
         * time must not exceed transmission time from the camera to the 
         * buffer. Grabbing blocks if the camera has not finished transmitting 
         * the next available image. The buffer size is controlled by the 
         * numBuffers parameter in the FC2Config struct. Note that this mode is 
         * the equivalent of flycaptureLockNext in earlier versions of the 
         * FlyCapture SDK.
         */
        BUFFER_FRAMES, 

        /**
         * Unspecified grab mode.
         */
        UNSPECIFIED_GRAB_MODE,
        GRAB_MODE_FORCE_32BITS = FULL_32BIT_VALUE
    };      
    
    /** Timeout options for grabbing images. */
    enum GrabTimeout
    {        
        TIMEOUT_INFINITE = -1, /**<  Wait indefinitely. */
        TIMEOUT_UNSPECIFIED = -2, /**< Unspecified timeout setting. */
        GRAB_TIMEOUT_FORCE_32BITS = FULL_32BIT_VALUE
    };

    /** Bandwidth allocation options for 1394 devices. */
    enum BandwidthAllocation
    {
        /** Do not allocate bandwidth. */
        BANDWIDTH_ALLOCATION_OFF = 0,
        /** Allocate bandwidth. This is the default setting. */
        BANDWIDTH_ALLOCATION_ON = 1,
        /**
        * Bandwidth allocation is not supported by either the camera or
        * operating system.
        */
        BANDWIDTH_ALLOCATION_UNSUPPORTED = 2,
        /** Not specified. This leaves the current setting unchanged. */
        BANDWIDTH_ALLOCATION_UNSPECIFIED = 3,
        BANDWIDTH_ALLOCATION_FORCE_32BITS = FULL_32BIT_VALUE

    };

    /** Interfaces that a camera may use to communicate with a host. */
    enum InterfaceType
    {                
        INTERFACE_IEEE1394, /**< IEEE-1394 (Includes 1394a and 1394b). */
        INTERFACE_USB2, /**< USB 2.0. */
        INTERFACE_UNKNOWN, /**< Unknown interface. */
        INTERFACE_TYPE_FORCE_32BITS = FULL_32BIT_VALUE

    };

    /**
     * Camera properties. Not all properties may be supported, depending
     * on the camera model.
     */
    enum PropertyType
    {
        BRIGHTNESS, /**< Brightness. */
        AUTO_EXPOSURE, /**< Auto exposure. */
        SHARPNESS, /**< Sharpness */
        WHITE_BALANCE, /**< White balance. */
        HUE, /**< Hue. */
        SATURATION, /**< Saturation. */
        GAMMA, /**< Gamma. */
        IRIS, /**< Iris. */
        FOCUS, /**< Focus. */
        ZOOM, /**< Zoom. */
        PAN, /**< Pan. */
        TILT, /**< Tilt. */
        SHUTTER, /**< Shutter. */
        GAIN, /**< Gain. */
        TRIGGER_MODE, /**< Trigger mode. */
        TRIGGER_DELAY, /**< Trigger delay. */
        FRAME_RATE, /**< Frame rate. */
        TEMPERATURE, /**< Temperature. */
        UNSPECIFIED_PROPERTY_TYPE, /**< Unspecified property type. */
        PROPERTY_TYPE_FORCE_32BITS = FULL_32BIT_VALUE

    };

    /** Frame rates in frames per second. */
    enum FrameRate
    {        
        FRAMERATE_1_875, /**< 1.875 fps. */       
        FRAMERATE_3_75, /**< 3.75 fps. */   
        FRAMERATE_7_5, /**< 7.5 fps. */    
        FRAMERATE_15, /**< 15 fps. */   
        FRAMERATE_30, /**< 30 fps. */     
        FRAMERATE_60, /**< 60 fps. */     
        FRAMERATE_120, /**< 120 fps. */
        FRAMERATE_240, /**< 240 fps. */
        NUM_FRAMERATES, /**< Number of possible camera frame rates. */
        FRAMERATE_FORMAT7, /**< Custom frame rate for Format7 functionality. */
        FRAMERATE_FORCE_32BITS = FULL_32BIT_VALUE

    };

    /** DCAM video modes. */
    enum VideoMode
    {        
        VIDEOMODE_160x120YUV444, /**< 160x120 YUV444. */
        VIDEOMODE_320x240YUV422, /**< 320x240 YUV422. */
        VIDEOMODE_640x480YUV411, /**< 640x480 YUV411. */
        VIDEOMODE_640x480YUV422, /**< 640x480 YUV422. */
        VIDEOMODE_640x480RGB, /**< 640x480 24-bit RGB. */
        VIDEOMODE_640x480Y8, /**< 640x480 8-bit. */
        VIDEOMODE_640x480Y16, /**< 640x480 16-bit. */
        VIDEOMODE_800x600YUV422, /**< 800x600 YUV422. */
        VIDEOMODE_800x600RGB, /**< 800x600 RGB. */
        VIDEOMODE_800x600Y8, /**< 800x600 8-bit. */
        VIDEOMODE_800x600Y16, /**< 800x600 16-bit. */
        VIDEOMODE_1024x768YUV422, /**< 1024x768 YUV422. */
        VIDEOMODE_1024x768RGB, /**< 1024x768 RGB. */
        VIDEOMODE_1024x768Y8, /**< 1024x768 8-bit. */
        VIDEOMODE_1024x768Y16, /**< 1024x768 16-bit. */
        VIDEOMODE_1280x960YUV422, /**< 1280x960 YUV422. */
        VIDEOMODE_1280x960RGB, /**< 1280x960 RGB. */
        VIDEOMODE_1280x960Y8, /**< 1280x960 8-bit. */
        VIDEOMODE_1280x960Y16, /**< 1280x960 16-bit. */
        VIDEOMODE_1600x1200YUV422, /**< 1600x1200 YUV422. */
        VIDEOMODE_1600x1200RGB, /**< 1600x1200 RGB. */
        VIDEOMODE_1600x1200Y8, /**< 1600x1200 8-bit. */
        VIDEOMODE_1600x1200Y16, /**< 1600x1200 16-bit. */
        VIDEOMODE_FORMAT7, /**< Custom video mode for Format7 functionality. */
        NUM_VIDEOMODES, /**< Number of possible video modes. */
        VIDEOMODE_FORCE_32BITS = FULL_32BIT_VALUE
    };

    /** Camera modes for DCAM formats as well as Format7. */
    enum Mode
    {
        MODE_0 = 0,
        MODE_1,
        MODE_2,
        MODE_3,
        MODE_4,
        MODE_5,
        MODE_6,
        MODE_7,
        MODE_8,
        MODE_9,
        MODE_10,
        MODE_11,
        MODE_12,
        MODE_13,
        MODE_14,
        MODE_15,
        MODE_16,
        MODE_17,
        MODE_18,
        MODE_19,
        MODE_20,
        MODE_21,
        MODE_22,
        MODE_23,
        MODE_24,
        MODE_25,
        MODE_26,
        MODE_27,
        MODE_28,
        MODE_29,
        MODE_30,
        MODE_31,
        NUM_MODES, /**< Number of modes */
        MODE_FORCE_32BITS = FULL_32BIT_VALUE
    };  

    /** Pixel formats available for Format7 modes. */
    enum PixelFormat
    {        
        PIXEL_FORMAT_MONO8     = 0x80000000, /**< 8 bits of mono information. */
        PIXEL_FORMAT_411YUV8   = 0x40000000, /**< YUV 4:1:1. */
        PIXEL_FORMAT_422YUV8   = 0x20000000, /**< YUV 4:2:2. */
        PIXEL_FORMAT_444YUV8   = 0x10000000, /**< YUV 4:4:4. */
        PIXEL_FORMAT_RGB8      = 0x08000000, /**< R = G = B = 8 bits. */
        PIXEL_FORMAT_MONO16    = 0x04000000, /**< 16 bits of mono information. */
        PIXEL_FORMAT_RGB16     = 0x02000000, /**< R = G = B = 16 bits. */
        PIXEL_FORMAT_S_MONO16  = 0x01000000, /**< 16 bits of signed mono information. */
        PIXEL_FORMAT_S_RGB16   = 0x00800000, /**< R = G = B = 16 bits signed. */
        PIXEL_FORMAT_RAW8      = 0x00400000, /**< 8 bit raw data output of sensor. */
        PIXEL_FORMAT_RAW16     = 0x00200000, /**< 16 bit raw data output of sensor. */
        PIXEL_FORMAT_MONO12    = 0x00100000, /**< 12 bits of mono information. */
        PIXEL_FORMAT_RAW12     = 0x00080000, /**< 12 bit raw data output of sensor. */
        PIXEL_FORMAT_BGR       = 0x80000008, /**< 24 bit BGR. */
        PIXEL_FORMAT_BGRU      = 0x40000008, /**< 32 bit BGRU. */
        PIXEL_FORMAT_RGB       = PIXEL_FORMAT_RGB8, /**< 24 bit RGB. */
        PIXEL_FORMAT_RGBU      = 0x40000002, /**< 32 bit RGBU. */
        NUM_PIXEL_FORMATS	      =  15, /**< Number of pixel formats. */
        UNSPECIFIED_PIXEL_FORMAT = 0 /**< Unspecified pixel format. */
    };

    /** Bus speeds. */
    enum BusSpeed
    {        
        BUSSPEED_S100, /**< 100Mbits/sec. */
        BUSSPEED_S200, /**< 200Mbits/sec. */
        BUSSPEED_S400, /**< 400Mbits/sec. */
        BUSSPEED_S480, /**< 480Mbits/sec. Only for USB cameras. */
        BUSSPEED_S800, /**< 800Mbits/sec. */
        BUSSPEED_S1600, /**< 1600Mbits/sec. */
        BUSSPEED_S3200, /**< 3200Mbits/sec. */
        BUSSPEED_S_FASTEST, /**< The fastest speed available. */
        BUSSPEED_ANY, /**< Any speed that is available. */
        BUSSPEED_SPEED_UNKNOWN = -1, /**< Unknown bus speed. */
        BUSSPEED_FORCE_32BITS = FULL_32BIT_VALUE
    };

    /**
     * Color processing algorithms. Please refer to our knowledge base at
     * article at http://www.ptgrey.com/support/kb/index.asp?a=4&q=33 for
     * complete details for each algorithm.
     */
    enum ColorProcessingAlgorithm
    {        
        /** Default method. */
        DEFAULT, 
        /** No color processing. */
        NO_COLOR_PROCESSING, 
        /**
         * Fastest but lowest quality. Equivalent to 
         * FLYCAPTURE_NEAREST_NEIGHBOR_FAST in FlyCapture.
         */ 
        NEAREST_NEIGHBOR, 
        /** Weights surrounding pixels based on localized edge orientation. */
        EDGE_SENSING, 
        /** Similar quality to rigorous but much faster. */
        HQ_LINEAR,
        /** Slowest but produces the best results. */
        RIGOROUS,
        COLOR_PROCESSING_ALGORITHM_FORCE_32BITS = FULL_32BIT_VALUE
    };

    /** Bayer tile formats. */
    enum BayerTileFormat
    {        
        NONE, /**< No bayer tile format. */
        RGGB, /**< Red-Green-Green-Blue. */
        GRBG, /**< Green-Red-Blue-Green. */
        GBRG, /**< Green-Blue-Red-Green. */
        BGGR, /**< Blue-Green-Green-Red. */
        BT_FORCE_32BITS = FULL_32BIT_VALUE
    };

    /** File formats to be used for saving images to disk. */
    enum ImageFileFormat
    {        
        FROM_FILE_EXT = -1, /**< Determine file format from file extension. */
        PGM, /**< Portable gray map. */
        PPM, /**< Portable pixmap. */
        BMP, /**< Bitmap. */
        JPEG, /**< JPEG. */
        JPEG2000, /**< JPEG 2000. */
        TIFF, /**< Tagged image file format. */
        PNG, /**< Portable network graphics. */
        RAW, /**< Raw data. */
        IMAGE_FILE_FORMAT_FORCE_32BITS = FULL_32BIT_VALUE
    };
    
    //==========================================================================
    // Structures
    //==========================================================================

    /** The current version of the library. */
    struct FC2Version
    {
        unsigned int major; /**< Major version number. */
        unsigned int minor; /**< Minor version number. */
        unsigned int type; /**< Type version number. */
        unsigned int build; /**< Build version number. */
    };

    /** 
     * Configuration for a camera. These options are options that are
     * generally should be set before starting isochronous transfer.
     */
    struct FC2Config
    {
        /** Number of buffers used by the FlyCapture2 library to grab images. */
        unsigned int numBuffers;
        
        /**
         * This is the number of notifications per image that will be triggered.
         * The default case is 1 notification at the end of a image. 
         * Setting this parameter to 2 will result in notifications after the 
         * first packet and at the end of image. 
         * Setting this parameter to anything more then 2 will divide the 
         * notifications equally throughout the buffer.
         * The maximum number of notifications possible is bufferSize/packetSize 
         * since notifications need to land on packet boundaries.
         */
        unsigned int numImageNotifications;
        
        /**
         * Time in milliseconds that RetrieveBuffer() will wait for an image
         * before timing out and returning.
         */
        int grabTimeout;
        
        /** Grab mode for the camera. The default is DROP_FRAMES. */
        GrabMode grabMode; 

        /** Isochronous bus speed. */
        BusSpeed isochBusSpeed;

        /** Asynchronous bus speed. */
        BusSpeed asyncBusSpeed;

        /**
         * Bandwidth allocation flag that tells the camera the bandwidth
         * allocation strategy to employ.
         */
        BandwidthAllocation bandwidthAllocation;

        /** Reserved for future use */
        unsigned int reserved[16];

        FC2Config()
        {
            numBuffers = 0;
            numImageNotifications = 0;
            grabTimeout = TIMEOUT_UNSPECIFIED;
            grabMode = UNSPECIFIED_GRAB_MODE;
            isochBusSpeed = BUSSPEED_ANY;
            asyncBusSpeed = BUSSPEED_ANY;
            bandwidthAllocation = BANDWIDTH_ALLOCATION_UNSPECIFIED;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /**
     * Information about a specific camera property. This structure is also
     * also used as the TriggerDelayInfo structure.
     */
    struct PropertyInfo
    {
        /** Property info type. */
        PropertyType type;
        /** Flag indicating if the property is present. */
        bool present;
        /** Flag indicating if auto is supported. */
        bool autoSupported;
        /** Flag indicating if manual is supported. */
        bool manualSupported;
        /** Flag indicating if on/off is supported. */
        bool onOffSupported;
        /** Flag indicating if one push is supported. */
        bool onePushSupported;
        /** Flag indicating if absolute mode is supported. */
        bool absValSupported;
        /** Flag indicating if property value can be read out. */
        bool readOutSupported;
        /** Minimum value (as an integer). */
        unsigned int min;
        /** Maximum value (as an integer). */
        unsigned int max;
        /** Minimum value (as a floating point value). */
        float absMin;
        /** Maximum value (as a floating point value). */
        float absMax;
        /** Textual description of units. */
        char pUnits[sk_maxStringLength];
        /** Abbreviated textual description of units. */
        char pUnitAbbr[sk_maxStringLength];
        /** Reserved for future use. */
        unsigned int reserved[8];

        PropertyInfo()
        {
            type = UNSPECIFIED_PROPERTY_TYPE;
            present = false;
            autoSupported = false;
            manualSupported = false;
            onOffSupported = false;
            onePushSupported = false;
            absValSupported = false;
            readOutSupported = false;
            min = 0;
            max = 0;
            absMin = 0.0f;
            absMax = 0.0f;
            memset( pUnits, 0, sk_maxStringLength );
            memset( pUnitAbbr, 0, sk_maxStringLength );
            memset( reserved, 0, sizeof(reserved) );
        }
    };   

    /** The TriggerDelayInfo structure is identical to PropertyInfo. */
    typedef PropertyInfo TriggerDelayInfo;

    /** A specific camera property. */
    struct Property
    {
        /** Property info type (e.g. BRIGHTNESS). */
        PropertyType type;
        /** Flag indicating if the property is present. */
        bool present;
        /** Flag controlling absolute mode. */
        bool absControl;
        /** Flag controlling one push. */
        bool onePush;
        /** Flag controlling on/off. */
        bool onOff;
        /** Flag controlling auto. */
        bool autoManualMode;
        /** Value A (integer). */
        unsigned int valueA;   
        /**
         * Value B (integer). Applies only to the white balance red value. Use
         * Value A for the blue value.
         */
        unsigned int valueB;   
        /** Floating point value. */
        float absValue;
        /** Reserved for future use. */
        unsigned int reserved[8];

        Property()
        {
            type = UNSPECIFIED_PROPERTY_TYPE;
            present = false;
            absControl = false;
            onePush = false;
            onOff = false;
            autoManualMode = false;
            valueA = 0;
            valueB = 0;
            absValue = 0.0f;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** The TriggerDelay structure is identical to Property. */
    typedef Property TriggerDelay;

    /** Information about a camera trigger property. */
    struct TriggerModeInfo
    {
        /** Presence of trigger mode. */
        bool present;
        /** Flag indicating if trigger value can be read out. */
        bool readOutSupported;
        /** Flag indicating if on/off is supported. */
        bool onOffSupported;
        /** Flag indicating if polarity is supported. */
        bool polaritySupported;
        /** Flag indicating if the value is readable. */
        bool valueReadable;
        /** Source mask. */
        unsigned int sourceMask;
        /** Flag indicating if software trigger is supported. */
        bool softwareTriggerSupported;
        /** Mode mask. */
        unsigned int modeMask;
        /** Reserved for future use. */
        unsigned int reserved[8];

        TriggerModeInfo()
        {
            present = false;
            readOutSupported = false;
            onOffSupported = false;
            polaritySupported = false;
            valueReadable = false;
            sourceMask = 0;
            softwareTriggerSupported = false;
            modeMask = 0;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** A camera trigger. */
    struct TriggerMode
    {      
        /** Flag controlling on/off. */
        bool onOff;
        /** Polarity value. */
        unsigned int polarity;
        /** Source value. */
        unsigned int source;
        /** Mode value. */
        unsigned int mode;
        /** Parameter value. */
        unsigned int parameter;      
        /** Reserved for future use. */
        unsigned int reserved[8];

        TriggerMode()
        {
            onOff = false;
            polarity = 0;
            source = 0;
            mode = 0;
            parameter = 0;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    //
    // Description:
    //	 A camera strobe property
    //
    /** A camera strobe property. */
    struct StrobeInfo
    {
        /** Source value. */
        unsigned int source;
        /** Presence of strobe. */
        bool present;
        /** Flag indicating if strobe value can be read out. */
        bool readOutSupported;
        /** Flag indicating if on/off is supported. */
        bool onOffSupported;
        /** Flag indicating if polarity is supported. */
        bool polaritySupported;
        /** Minimum value. */
        float minValue;
        /** Maximum value. */
        float maxValue;
        /** Reserved for future use. */
        unsigned int reserved[8];

        StrobeInfo()
        {
            source = 0;
            present = false;
            readOutSupported = false;
            onOffSupported = false;
            polaritySupported = false;
            minValue = 0.0f;
            maxValue = 0.0f;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** A camera strobe. */
    struct StrobeControl
    {      
        /** Source value. */
        unsigned int source;
        /** Flag controlling on/off. */
        bool onOff;
        /** Signal polarity. */
        unsigned int polarity;
        /** Signal delay (in ms). */
        float delay;
        /** Signal duration (in ms). */
        float duration;
        /** Reserved for future use. */
        unsigned int reserved[8];
    
        StrobeControl()
        {
            source = 0;
            onOff = false;
            polarity = 0;
            delay = 0.0f;
            duration = 0.0f;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** Format 7 image settings. */
    struct Format7ImageSettings
    {
        /** Format 7 mode. */
        Mode mode;
        /** Horizontal image offset. */
        unsigned int offsetX;
        /** Vertical image offset. */
        unsigned int offsetY;
        /** Width of image. */
        unsigned int width;
        /** Height of image. */
        unsigned int height;
        /** Pixel format of image. */
        PixelFormat pixelFormat;
        /** Reserved for future use. */
        unsigned int reserved[8];

        Format7ImageSettings()
        {
            mode = MODE_0;
            offsetX = 0;
            offsetY = 0;
            width = 0;
            height = 0;
            pixelFormat = UNSPECIFIED_PIXEL_FORMAT;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** Format 7 information for a single mode. */
    struct Format7Info
    {
        /** Format 7 mode. */
        Mode mode;

        /** Maximum image width. */
        unsigned int maxWidth;
        /** Maximum image height. */
        unsigned int maxHeight;
        /** Horizontal step size for the offset. */
        unsigned int offsetHStepSize;
        /** Vertical step size for the offset. */
        unsigned int offsetVStepSize;
        /** Horizontal step size for the image. */
        unsigned int imageHStepSize;
        /** Vertical step size for the image. */
        unsigned int imageVStepSize;
        /** Supported pixel formats in a bit field. */
        unsigned int pixelFormatBitField;

        /** Current packet size in bytes. */
        unsigned int packetSize;
        /** Minimum packet size in bytes for current mode. */
        unsigned int minPacketSize;
        /** Maximum packet size in bytes for current mode. */
        unsigned int maxPacketSize;
        /** Current packet size as a percentage of maximum packet size. */
        float percentage;
        /** Reserved for future use. */
        unsigned int reserved[16];

        Format7Info()
        {
            mode = MODE_0;
            maxWidth = 0;
            maxHeight = 0;
            offsetHStepSize = 0;
            offsetVStepSize = 0;
            imageHStepSize = 0;
            imageVStepSize = 0;
            pixelFormatBitField = 0;
            packetSize = 0;
            minPacketSize = 0;
            maxPacketSize = 0;
            percentage = 0.0f;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    //
    // Description:
    //	 
    //
    /** Format 7 packet information. */
    struct Format7PacketInfo
    {
        /** Recommended bytes per packet. */
        unsigned int recommendedBytesPerPacket;
        /** Maximum bytes per packet. */
        unsigned int maxBytesPerPacket;
        /** Minimum bytes per packet. */
        unsigned int unitBytesPerPacket;
        /** Reserved for future use. */
        unsigned int reserved[8];

        Format7PacketInfo()
        {
            recommendedBytesPerPacket = 0;
            maxBytesPerPacket = 0;
            unitBytesPerPacket = 0;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** Timestamp information. */
    struct TimeStamp
    {
        /** Seconds. */
        long long seconds;
        /** Microseconds. */
        unsigned int microSeconds;
        /** 1394 cycle time seconds. */
        unsigned int cycleSeconds;
        /** 1394 cycle time count. */
        unsigned int cycleCount;
        /** 1394 cycle time offset. */
        unsigned int cycleOffset;
        /** Reserved for future use. */
        unsigned int reserved[8];

        TimeStamp()
        {
            seconds = 0;
            microSeconds = 0;
            cycleSeconds = 0;
            cycleCount = 0;
            cycleOffset = 0;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** Camera configuration ROM. */
    struct ConfigROM
    {
        /** Vendor ID of a node. */
        unsigned int nodeVendorId;
        /** Chip ID (high part). */
        unsigned int chipIdHi;
        /** Chip ID (low part). */
        unsigned int chipIdLo;
        /** Unit Spec ID, usually 0xa02d. */
        unsigned int unitSpecId;
        /** Unit software version. */
        unsigned int unitSWVer;
        /** Unit sub software version. */
        unsigned int unitSubSWVer;
        /** Vendor unique info 0. */
        unsigned int vendorUniqueInfo_0;
        /** Vendor unique info 1. */
        unsigned int vendorUniqueInfo_1;
        /** Vendor unique info 2. */
        unsigned int vendorUniqueInfo_2;
        /** Vendor unique info 3. */
        unsigned int vendorUniqueInfo_3;
        /** Keyword. */
        char pszKeyword[ sk_maxStringLength ];
        /** Reserved for future use. */
        unsigned int reserved[16];

        ConfigROM()
        {
            nodeVendorId = 0;
            chipIdHi = 0;
            chipIdLo = 0;
            unitSpecId = 0;
            unitSWVer = 0;
            unitSubSWVer = 0;
            vendorUniqueInfo_0 = 0;
            vendorUniqueInfo_1 = 0;
            vendorUniqueInfo_2 = 0;
            vendorUniqueInfo_3 = 0;
            memset( pszKeyword, 0, sizeof ( pszKeyword ) );
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** Camera information. */
    struct CameraInfo
    {      
        /** Device serial number. */
        unsigned int serialNumber;
        /** Interface type. */
        InterfaceType interfaceType;
        /** Flag indicating if this is a color camera. */
        bool isColorCamera;
        /** Device model name. */
        char modelName[sk_maxStringLength];
        /** Device vendor name. */
        char vendorName[sk_maxStringLength];
        /** String detailing the sensor information. */
        char sensorInfo[sk_maxStringLength];
        /** String providing the sensor resolution. */
        char sensorResolution[sk_maxStringLength];
        /** Driver name of driver being used. */
        char driverName[sk_maxStringLength];
        /** Firmware version of camera. */
        char firmwareVersion[sk_maxStringLength];
        /** Firmware build time. */
        char firmwareBuildTime[sk_maxStringLength];
        /** DCAM version. */
        unsigned int dcamVer;
        /** Maximum bus speed. */
        BusSpeed maximumBusSpeed;

        /** Configuration ROM data. */
        ConfigROM configROM;

        /** Reserved for future use. */
        unsigned int reserved[16];

        CameraInfo()
        {
            serialNumber = 0;
            interfaceType = INTERFACE_UNKNOWN;
            isColorCamera = false;
            memset( modelName, 0, sizeof( modelName ) );
            memset( vendorName, 0, sizeof( vendorName ) );
            memset( sensorInfo, 0, sizeof( sensorInfo ) );
            memset( sensorResolution, 0, sizeof( sensorResolution ) );
            memset( driverName, 0, sizeof(driverName) );
            memset( firmwareVersion, 0, sizeof( firmwareVersion ) );
            memset( firmwareBuildTime, 0, sizeof( firmwareBuildTime ) );
            dcamVer = 0;
            maximumBusSpeed = BUSSPEED_SPEED_UNKNOWN;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** Properties of a single embedded image info property. */
    struct EmbeddedImageInfoProperty
    {
        /** Whether this property is available. */
        bool available;
        /** Whether this property is on or off. */
        bool onOff;

        EmbeddedImageInfoProperty()
        {
            available = false;
            onOff = false;
        }
    };

    /** Properties of the possible embedded image information. */
    struct EmbeddedImageInfo
    {
        EmbeddedImageInfoProperty timestamp;
        EmbeddedImageInfoProperty gain;
        EmbeddedImageInfoProperty shutter;
        EmbeddedImageInfoProperty brightness;
        EmbeddedImageInfoProperty exposure;
        EmbeddedImageInfoProperty whiteBalance;
        EmbeddedImageInfoProperty frameCounter;
        EmbeddedImageInfoProperty strobePattern;
        EmbeddedImageInfoProperty GPIOPinState;
        EmbeddedImageInfoProperty ROIPosition;
    };

    /** Metadata related to an image. */
    struct ImageMetadata
    {        
        /** Embedded timestamp. */
        unsigned int embeddedTimeStamp;
        /** Embedded gain. */
        unsigned int embeddedGain;
        /** Embedded shutter. */
        unsigned int embeddedShutter;
        /** Embedded brightness. */
        unsigned int embeddedBrightness;
        /** Embedded exposure. */
        unsigned int embeddedExposure;
        /** Embedded white balance. */
        unsigned int embeddedWhiteBalance;
        /** Embedded frame counter. */
        unsigned int embeddedFrameCounter;
        /** Embedded strobe pattern. */
        unsigned int embeddedStrobePattern;
        /** Embedded GPIO pin state. */
        unsigned int embeddedGPIOPinState;
        /** Embedded ROI position. */
        unsigned int embeddedROIPosition;        

        /** Reserved for future use. */
        unsigned int reserved[31];

        ImageMetadata()
        {
            embeddedTimeStamp = 0;
            embeddedGain = 0;
            embeddedShutter = 0;
            embeddedBrightness = 0;
            embeddedExposure = 0;
            embeddedWhiteBalance = 0;
            embeddedFrameCounter = 0;
            embeddedStrobePattern = 0;
            embeddedGPIOPinState = 0;
            embeddedROIPosition = 0;        
            memset( reserved, 0, sizeof(reserved));
        }
    };

    /** Information about the camera's look up table. */
    struct LUTData
    {
        /** Flag indicating if LUT is supported. */
        bool supported;
        /** Flag indicating if LUT is enabled. */
        bool enabled;
        /** The number of LUT banks available (Always 1 for PGR LUT). */
        unsigned int numBanks;
        /** The number of LUT channels per bank available. */
        unsigned int numChannels;
        /** The input bit depth of the LUT. */
        unsigned int inputBitDepth;
        /** The output bit depth of the LUT. */
        unsigned int outputBitDepth;
        /** The number of entries in the LUT. */
        unsigned int numEntries;
        /** Reserved for future use. */
        unsigned int reserved[8];

        LUTData()
        {
            supported = false;
            enabled = false;
            numBanks = 0;
            numChannels = 0;
            inputBitDepth = 0;
            outputBitDepth = 0;
            numEntries = 0;
            memset( reserved, 0, sizeof(reserved) );
        }
    };

    /** Options for saving PNG images. */
    struct PNGOption
    {
        /** Save as interlaced. */
        bool interlaced; 
        /** Compression level (0-9). 0 is no compression, 9 is best compression. */
        unsigned int compressionLevel;
        /** Reserved for future use. */
        unsigned int reserved[16];

        PNGOption()
        {
            interlaced = false;
            compressionLevel = 6;
            memset( reserved, 0, sizeof (reserved));
        }
    };

    /** Options for saving PPM images. */
    struct PPMOption
    {
        /** Whether to save the PPM as a binary file. */
        bool binaryFile;
        /** Reserved for future use. */
        unsigned int reserved[16];

        PPMOption()
        {
            binaryFile = true;
            memset( reserved, 0, sizeof (reserved));
        }
    };
       
    /** Options for saving PGM images. */
    struct PGMOption
    {
        /** Whether to save the PPM as a binary file. */
        bool binaryFile;
        /** Reserved for future use. */
        unsigned int reserved[16];

        PGMOption()
        {
            binaryFile = true;
            memset( reserved, 0, sizeof (reserved));
        }
    };

    /** Options for saving TIFF images. */
    struct TIFFOption
    {
        enum CompressionMethod
        {            
            NONE = 1, /**< Save without any compression. */            
            PACKBITS, /**< Save using PACKBITS compression. */            
            DEFLATE, /**< Save using DEFLATE compression (ZLIB compression). */            
            ADOBE_DEFLATE, /**< Save using ADOBE DEFLATE compression */
            /**
            * Save using CCITT Group 3 fax encoding. This is only valid for
            * 1-bit images only. Default to LZW for other bit depths.
            */
            CCITTFAX3,
            /**
            * Save using CCITT Group 4 fax encoding. This is only valid for
            * 1-bit images only. Default to LZW for other bit depths.
            */
            CCITTFAX4,            
            LZW, /**< Save using LZW compression. */
            /**
             * Save using JPEG compression. This is only valid for 8-bit
             * greyscale and 24-bit only. Default to LZW for other bit depths.
             */
            JPEG
        };

        /** Compression method to use for encoding TIFF images. */
        CompressionMethod compression; 
        /** Reserved for future use. */
        unsigned int reserved[16];

        TIFFOption()
        {
            compression = LZW;
            memset( reserved, 0, sizeof (reserved));
        }
    };

    /** Options for saving JPEG image. */
    struct JPEGOption
    {
        /** Whether to save as a progressive JPEG file. */
        bool progressive; 
        /**
         * JPEG image quality in range (0-100).
         * - 100 - Superb quality.
         * - 75  - Good quality.
         * - 50  - Normal quality.
         * - 10  - Poor quality.
         */
        unsigned int quality;
        /** Reserved for future use. */
        unsigned int reserved[16];

        JPEGOption()
        {
            progressive = false;
            quality = 75;
            memset( reserved, 0, sizeof (reserved));
        }
    };

    /** Options for saving JPEG2000 image. */
    struct JPG2Option
    {
        /** JPEG saving quality in range (1-512). */
        unsigned int quality;
        /** Reserved for future use. */
        unsigned int reserved[16];

        JPG2Option()
        {
            quality = 16;
            memset( reserved, 0, sizeof (reserved));
        }
    };

    /** Options for saving AVI files. */
    struct AVIOption
    {
        /** Frame rate of the stream */
        float frameRate;
        /** Reserved for future use */
        unsigned int reserved[256];

        AVIOption()
        {
            frameRate = 15.0;
            memset( reserved, 0, sizeof (reserved));
        }
    };
}

#endif // PGR_FC2_FLYCAPTURE2DEFS_H

