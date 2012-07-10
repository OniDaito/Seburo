/**
* @brief GLFW Application
* @file app.hpp
* @author Benjamin Blundell <oni@section9.co.uk>
* @date 03/07/2012
*
*/

#ifndef GLAPP_HPP
#define GLAPP_HPP

#include "s9gear.hpp"
#include "common.hpp"

#include <GL/glfw3.h>
#include <anttweakbar/AntTweakBar.h>

#ifdef _GEAR_X11_GLX
#include <gtkmm.h>
#endif


/*
 * The main Application. Based around GLFW
 */
 
namespace s9 {

	typedef enum{
		MOUSE_LEFT = 0x01,
		MOUSE_RIGHT = 0x02,
		MOUSE_MIDDLE = 0x04
	}MouseButton;
	
	class MouseStatus {
	public:
		MouseStatus() {mButton = 0x00; mX = mY = mDX = mDY = 0; };
		uint8_t mButton;
		int16_t mX,mY,mDX,mDY;
	};
	
	class GLApp{
	public:
		GLApp();
		
		static GLFWwindow createWindow(std::string title, size_t w, size_t h);
		
		static void mainLoop();
		static void init();
		static void reshape(GLFWwindow window, int w, int h);
		static void display(GLFWwindow window);
		
		static void resizeHUD(int w, int h);
		static void drawMesh(Camera &c);
		static void drawGripper(Camera &c);
		
		static void picked(glm::vec2 m, glm::vec4 &c);
		static void movePicked(glm::vec2 d);
		
		static glm::mat4 getMatrix(Camera &c, Primitive p) { return c.getMatrix() * p.getMatrix(); }
		
		static PrimPtr pPicked;

		// OS Dependent
		static void loadFile();

		static void keyCallback(GLFWwindow window, int key, int action);
		static void mouseButtonCallback(GLFWwindow window, int button, int action);
		static void mousePositionCallback(GLFWwindow window, int x, int y);
		static void mouseWheelCallback(GLFWwindow window, int xpos, int ypos);
		static void monitorCallback( GLFWmonitor m, int p);
		
	protected:

		static std::vector<GLFWwindow> vWindows;
		static GLboolean mRunning;
		
		static MouseStatus mMouseStatus;
			
	};
}

#endif
