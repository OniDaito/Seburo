/**
* @brief UVC Camera controller for Linux
* @file uvc_camera.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/05/2012
*
*/

#ifndef __UVC_VIDEO__
#define __UVC_VIDEO__


#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <thread>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>

// videodev2 under ubuntu apparently
#include <linux/videodev2.h>

// Include GUVCView decoding
// http://sourceforge.net/p/guvcview/code/795/tree/trunk/src/

extern "C" {
    #include "s9/jpeg.h"
    #include "s9/colorspaces.h"
}
 

#define V4L_BUFFERS_DEFAULT	8
#define V4L_BUFFERS_MAX		32

#define OPT_ENUM_INPUTS		256
#define OPT_SKIP_FRAMES		257


/************************************************************************
 * values for the "interlace" parameter [y4m_*_interlace()]
 ************************************************************************/
#define Y4M_ILACE_NONE          0   /* non-interlaced, progressive frame */
#define Y4M_ILACE_TOP_FIRST     1   /* interlaced, top-field first       */
#define Y4M_ILACE_BOTTOM_FIRST  2   /* interlaced, bottom-field first    */
#define Y4M_ILACE_MIXED         3   /* mixed, "refer to frame header"    */

/************************************************************************
 * values for the "chroma" parameter [y4m_*_chroma()]
 ************************************************************************/
#define Y4M_CHROMA_420JPEG     0  /* 4:2:0, H/V centered, for JPEG/MPEG-1 */
#define Y4M_CHROMA_420MPEG2    1  /* 4:2:0, H cosited, for MPEG-2         */
#define Y4M_CHROMA_420PALDV    2  /* 4:2:0, alternating Cb/Cr, for PAL-DV */
#define Y4M_CHROMA_444         3  /* 4:4:4, no subsampling, phew.         */
#define Y4M_CHROMA_422         4  /* 4:2:2, H cosited                     */
#define Y4M_CHROMA_411         5  /* 4:1:1, H cosited                     */
#define Y4M_CHROMA_MONO        6  /* luma plane only                      */
#define Y4M_CHROMA_444ALPHA    7  /* 4:4:4 with an alpha channel          */


/*
 * Class to deal directly with the video. Does not contain OpenCV methods - this is decoupled into the camera manager
 */

class UVCVideo {
public:
	UVCVideo () {};
	bool startCapture(std::string devname, unsigned int width, unsigned int height, unsigned int fps);
	void stop();
	unsigned char* getBuffer();

	void video_list_controls(int dev);
	void set_control(unsigned int id, int value) { uvc_set_control(dev,id,value); };
	
protected:
	
	// UVC Linux Functions

	void uvc_set_control(int dev, unsigned int id, int value);
	int video_open(const char *devname);
	int video_set_format(int dev, unsigned int w, unsigned int h, unsigned int format);
	int video_set_framerate(int dev, int fps);
	int video_reqbufs(int dev, int nbufs);
	int video_enable(int dev, int enable);
	void video_query_menu(int dev, unsigned int id);
	
	void video_enum_inputs(int dev);
	int video_get_input(int dev);
	int video_set_input(int dev, unsigned int input);

	// Decoding functions
	void decode_frame(unsigned char *jpeg_data, int len, unsigned char *raw);
	void toRGB(unsigned char *jpeg_data, int len, unsigned char *raw);

	void capture();
	bool mRunning;
	std::thread *pWorkerThread;
	std::mutex mMutex;
	
	bool mT;
	int mWidth, mHeight;
	int mFPS;
	void *mem[V4L_BUFFERS_MAX];
	int dev;
	unsigned char *jbuffer;
	unsigned int mBufferSize;
	struct v4l2_buffer buf;
	
};

#endif
