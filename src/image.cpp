/**
* @brief Image Class
* @file image.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 21/10/2013
*
*/

#include "s9/image.hpp"

using namespace std;
using namespace s9;


#ifdef _SEBURO_OSX

///\todo not sure how we are dealing with multi-lingual paths :S
///\todo not sure how to sort out errors in this context - Actually the data will be zero :P
// https://developer.apple.com/library/mac/documentation/graphicsimaging/conceptual/opengl-macprogguide/opengl_texturedata/opengl_texturedata.html

byte_t * Image::imageReference (const char* imageName) {
    CGImageRef image;
    CGDataProviderRef provider;
    CFURLRef url;
    CFBundleRef mainBundle = CFBundleGetMainBundle();
 
    // Create the data provider object
    provider = CGDataProviderCreateWithFilename(imageName);
 
    // Create the image object from that provider.
    image = CGImageCreateWithJPEGDataProvider (provider, NULL, true, kCGRenderingIntentDefault);
    CGDataProviderRelease (provider); 

    width_ = CGImageGetWidth(image);
    height_ = CGImageGetHeight(image);

    bits_per_component_ = CGImageGetBitsPerComponent(image);
    bits_per_pixel_ = CGImageGetBitsPerPixel(image);
    bytes_per_row_ = CGImageGetBytesPerRow(image);

    CGRect rect = {{0, 0}, {static_cast<double>(width_), static_cast<double>(height_)}};
    unsigned char * data = new unsigned char [width_ * 4 * height_]();

    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    CGContextRef myBitmapContext = CGBitmapContextCreate (data,
                        width_, height_, 8,
                        width_*4, space,
                        kCGBitmapByteOrder32Host |
                          kCGImageAlphaPremultipliedFirst);

    CGContextSetBlendMode(myBitmapContext, kCGBlendModeCopy);
    CGContextDrawImage(myBitmapContext, rect, image);
    CGContextRelease(myBitmapContext);

    component_ = BGRA;
    colour_type_ = UNSIGNED_BYTE;
/*
glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
glGenTextures(1, &myTextureName);
glBindTexture(GL_TEXTURE_RECTANGLE_ARB, myTextureName);
glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA8, width, height,
                    0, GL_BGRA_EXT, GL_UNSIGNED_INT_8_8_8_8_REV, myData);
free(myData);*/

    cout << "SEBURO IMAGE: " <<  width_ << "x" << height_ << " bpp: " << bits_per_pixel_ << endl;

    return data;
}

#endif