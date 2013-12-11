/**
* @brief FBO Basic Class
* @file fbo.cpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 05/07/2012
*
*/

#include "s9/gl/fbo.hpp"

using namespace std;
using namespace s9::gl;

/*
 * A Basic FBO with Rectangular textures FBO
 */

FBO::FBO (size_t w, size_t h) : obj_( shared_ptr<SharedObject> (new SharedObject())){
		
	obj_->width = w;
	obj_->height = h;

	// Create depth renderbuffer
  ///\todo this needs to be a texture really
	glGenRenderbuffers(1, &(obj_->depth));
  glBindRenderbuffer(GL_RENDERBUFFER, obj_->depth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);

	// Create the texture - RGBA, unsigned_byte
  obj_->colour = Texture(w, h, RGBA);
  glGenFramebuffers(1, &(obj_->id));
  glBindFramebuffer(GL_FRAMEBUFFER, obj_->id);
	
	// Attach texture to first color attachment and the depth to the depth attachment
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, obj_->colour.gl_type(), obj_->colour.id(), 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, obj_->depth);

	if (checkStatus() )  {
		obj_->ok = true;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
  CXGLERROR
}


bool FBO::checkStatus() {
 GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        std::cout << "SEBURO Framebuffer complete." << std::endl;
        return true;

    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        std::cerr << "SEBURO Framebuffer incomplete: Attachment is NOT complete." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        std::cerr << "SEBURO Framebuffer incomplete: No image is attached to FBO." << std::endl;
        return false;
/*
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
        std::cout << "[ERROR] Framebuffer incomplete: Attached images have different dimensions." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
        std::cout << "[ERROR] Framebuffer incomplete: Color attached images have different internal formats." << std::endl;
        return false;
*/
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        std::cerr << "SEBURO Framebuffer incomplete: Draw buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        std::cerr << "SEBURO Framebuffer incomplete: Read buffer." << std::endl;
        return false;

    case GL_FRAMEBUFFER_UNSUPPORTED:
        std::cerr << "SEBURO Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
        return false;

    default:
        std::cerr << "SEBURO Framebuffer incomplete: Unknown error." << std::endl;
        return false;
    }
}

void FBO::printFramebufferInfo() {
    std::cout << "\n===== FBO STATUS =====\n";

    // print max # of colorbuffers supported by FBO
    int colorBufferCount = 0;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
    std::cout << "SEBURO Max Number of Color Buffer Attachment Points: " << colorBufferCount << std::endl;

    int objectType;
    int objectId;

    // print info of the colorbuffer attachable image
    for(int i = 0; i < colorBufferCount; ++i)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_COLOR_ATTACHMENT0+i,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                              &objectType);
        if(objectType != GL_NONE)
        {
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                                  GL_COLOR_ATTACHMENT0+i,
                                                  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                                  &objectId);

            std::string formatName;

            std::cout << "SEBURO Colour Attachment " << i << ": ";
            if(objectType == GL_TEXTURE)
            {
                std::cout << "SEBURO GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
            }
            else if(objectType == GL_RENDERBUFFER)
            {
                std::cout << "SEBURO GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
            }
        }
    }

    // print info of the depthbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_DEPTH_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_DEPTH_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

        std::cout << "SEBURO Depth Attachment: ";
        switch(objectType) {
        case GL_TEXTURE:
            std::cout << "SEBURO GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
            break;
        case GL_RENDERBUFFER:
            std::cout << "SEBURO GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
            break;
        }
    }

    // print info of the stencilbuffer attachable image
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_STENCIL_ATTACHMENT,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                                          &objectType);
    if(objectType != GL_NONE)
    {
        glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                              GL_STENCIL_ATTACHMENT,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &objectId);

        std::cout << "SEBURO Stencil Attachment: ";
        switch(objectType) {
        case GL_TEXTURE:
            std::cout << "SEBURO GL_TEXTURE, " << getTextureParameters(objectId) << std::endl;
            break;
        case GL_RENDERBUFFER:
            std::cout << "SEBURO GL_RENDERBUFFER, " << getRenderbufferParameters(objectId) << std::endl;
            break;
        }
    }

    std::cout << std::endl;
}


void FBO::resize(size_t w, size_t h){
  if(obj_ == nullptr) return;
	if(!obj_->ok) return;
	
	obj_->width = w;
	obj_->height = h;
	
  obj_->colour.bind();
  obj_->colour.resize(w,h);
	
	glBindRenderbuffer(GL_RENDERBUFFER, obj_->depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);

  obj_->colour.unbind();

  CXGLERROR
	
}


FBO::SharedObject::~SharedObject() { 
   glDeleteRenderbuffers(1, &depth);
   //Bind 0, which means render to back buffer, as a result, fb is unbound
   glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
   glDeleteFramebuffersEXT(1, &id);
}



namespace s9{
	namespace gl {

		std::string convertInternalFormatToString(GLenum format) {
			std::string formatName;

			switch(format)
			{
			case GL_STENCIL_INDEX:
				formatName = "GL_STENCIL_INDEX";
				break;
			case GL_DEPTH_COMPONENT:
				formatName = "GL_DEPTH_COMPONENT";
				break;
			case GL_DEPTH_STENCIL:
				formatName = "GL_DEPTH_STENCIL";
				break;
			case GL_ALPHA:
				formatName = "GL_ALPHA";
				break;
			case GL_RGB:
				formatName = "GL_RGB";
				break;
			case GL_RGBA:
				formatName = "GL_RGBA";
				break;
			case GL_LUMINANCE:
				formatName = "GL_LUMINANCE";
				break;
			case GL_LUMINANCE_ALPHA:
				formatName = "GL_LUMINANCE_ALPHA";
				break;
			case GL_ALPHA4:
				formatName = "GL_ALPHA4";
				break;
			case GL_ALPHA8:
				formatName = "GL_ALPHA8";
				break;
			case GL_ALPHA12:
				formatName = "GL_ALPHA12";
				break;
			case GL_ALPHA16:
				formatName = "GL_ALPHA16";
				break;
			case GL_LUMINANCE4:
				formatName = "GL_LUMINANCE4";
				break;
			case GL_LUMINANCE8:
				formatName = "GL_LUMINANCE8";
				break;
			case GL_LUMINANCE12:
				formatName = "GL_LUMINANCE12";
				break;
			case GL_LUMINANCE16:
				formatName = "GL_LUMINANCE16";
				break;
			case GL_LUMINANCE4_ALPHA4:
				formatName = "GL_LUMINANCE4_ALPHA4";
				break;
			case GL_LUMINANCE6_ALPHA2:
				formatName = "GL_LUMINANCE6_ALPHA2";
				break;
			case GL_LUMINANCE8_ALPHA8:
				formatName = "GL_LUMINANCE8_ALPHA8";
				break;
			case GL_LUMINANCE12_ALPHA4:
				formatName = "GL_LUMINANCE12_ALPHA4";
				break;
			case GL_LUMINANCE12_ALPHA12:
				formatName = "GL_LUMINANCE12_ALPHA12";
				break;
			case GL_LUMINANCE16_ALPHA16:
				formatName = "GL_LUMINANCE16_ALPHA16";
				break;
			case GL_INTENSITY:
				formatName = "GL_INTENSITY";
				break;
			case GL_INTENSITY4:
				formatName = "GL_INTENSITY4";
				break;
			case GL_INTENSITY8:
				formatName = "GL_INTENSITY8";
				break;
			case GL_INTENSITY12:
				formatName = "GL_INTENSITY12";
				break;
			case GL_INTENSITY16:
				formatName = "GL_INTENSITY16";
				break;
			case GL_R3_G3_B2:
				formatName = "GL_R3_G3_B2";
				break;
			case GL_RGB4:
				formatName = "GL_RGB4";
				break;
			case GL_RGB5:
				formatName = "GL_RGB4";
				break;
			case GL_RGB8:
				formatName = "GL_RGB8";
				break;
			case GL_RGB10:
				formatName = "GL_RGB10";
				break;
			case GL_RGB12:
				formatName = "GL_RGB12";
				break;
			case GL_RGB16:
				formatName = "GL_RGB16";
				break;
			case GL_RGBA2:
				formatName = "GL_RGBA2";
				break;
			case GL_RGBA4:
				formatName = "GL_RGBA4";
				break;
			case GL_RGB5_A1:
				formatName = "GL_RGB5_A1";
				break;
			case GL_RGBA8:
				formatName = "GL_RGBA8";
				break;
			case GL_RGB10_A2:
				formatName = "GL_RGB10_A2";
				break;
			case GL_RGBA12:
				formatName = "GL_RGBA12";
				break;
			case GL_RGBA16:
				formatName = "GL_RGBA16";
				break;
			default:
				formatName = "Unknown Format";
			}

			return formatName;
		}

		std::string getTextureParameters(GLuint id)
		{
			if(glIsTexture(id) == GL_FALSE)
				return "SEBURO Not texture object";

			int width, height, format;
			std::string formatName;
			glBindTexture(GL_TEXTURE_2D, id);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
			glBindTexture(GL_TEXTURE_2D, 0);

			formatName = convertInternalFormatToString(format);

			std::stringstream ss;
			ss << width << "x" << height << ", " << formatName;
			return ss.str();
		}


		std::string getRenderbufferParameters(GLuint id)
		{
			if(glIsRenderbuffer(id) == GL_FALSE)
				return "SEBURO Not Renderbuffer object";

			int width, height, format;
			std::string formatName;
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);    // get renderbuffer width
			glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);  // get renderbuffer height
			glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); // get renderbuffer internal format
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			formatName = convertInternalFormatToString(format);

			std::stringstream ss;
			ss << width << "x" << height << ", " << formatName;
			return ss.str();
		}


	}
}



