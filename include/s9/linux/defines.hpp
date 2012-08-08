/**
* @brief UVC Camera controller for Linux
* @file uvc_camera.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#ifndef S9_LINUX_DEFINES
#define S9_LINUX_DEFINES



// typedefs for the controlling of the C910
typedef enum {
    UVC_BRIGHTNESS = 0x00980900,
    UVC_CONTRAST = 0x00980901,
    UVC_SATURATION = 0x00980902,
    UVC_GAIN = 0x00980913,
    UVC_SHARPNESS = 0x0098091b,
    UVC_AUTO_EXPOSURE = 0x009a0901,
    UVC_EXPOSURE = 0x009a0902,
    UVC_FOCUS = 0x009a090a,
    UVC_AUTO_FOCUS = 0x009a090c
}UVCControl;

#endif