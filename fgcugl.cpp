// file: fgcugl.h
// 
// Copyright (c) 2021 Paul Allen
// 
// This code is licensed under MIT license (see LICENSE for details)
// 
// Implementation of a 2D graphics library built on OpenGL and Glew
// --------------------------------------------------------

#define _USE_MATH_DEFINES
#include <cmath>
#include "fgcugl.h"

namespace fgcugl
{
	static GLFWwindow* s_window;

	// GLFW window resize callback function prototype
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// color function prototype 
	void setColor(unsigned int);

	/**
	 Initialize a new OpenGL window
	 Parameters:
		width - width of the window in pixels
		height - height of the window in pixels
		title - text to display in window titlebar
	 Returns:
		void
	*/
	void openWindow(int width, int height, std::string title)
	{
		// inititalize the GLFW
		if (!glfwInit())
			return;

		// create a windowed mode and its OpenGL Contect
		s_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

		if (!s_window)
		{
			glfwTerminate();
			return;
		}

		// set callback function to resize the window
		glfwSetFramebufferSizeCallback(s_window, framebuffer_size_callback);

		// make the window's context current
		glfwMakeContextCurrent(s_window);

		// specify the part of the window to which OpenGL will 
		// draw (in pixels), confert from normalized to pixels
		glViewport(0, 0, width, height);
		// defines the properties of the camera that views the objects
		// in the world coordinate frame, typically set zoom factor,
		// aspect ratio and near and far clipping planes
		glMatrixMode(GL_PROJECTION);
		// replace the current matrix with the identitiy matrix and starts
		// fresh because matrix transforms such as glOrtho and glRotate cumulate
		// basically puts us at (0, 0, 0)
		glLoadIdentity();
		// set the coordinate system
		glOrtho(0, width, 0, height, 0, 1);
		// (defalut matrix mode) modelview matrix defines how objects are
		// transformed (meaning translation, rotation and scaling) in the world
		glMatrixMode(GL_MODELVIEW);
		// same as above
		glLoadIdentity();
	}

	/**
	 Returns true if the OpenGL window is closing
	 Returns:
		bool - true if closing
	*/
	bool windowClosing()
	{
		return glfwWindowShouldClose(s_window);
	}

	/**
	 Performs double buffering of window painting so 
	 changes are written to one buffer while the other
	 is being drawn, and swaps the buffers when called.
	 Returns:
		void
	*/
	void windowPaint()
	{
		// swap front and back buffers
		glfwSwapBuffers(s_window);
	}

	/**
	 Get's current program execution time in best possible precision, 
	 typically nano or micro seconds
	 Returns:
		double	- amount of time since program began execution
	*/
	double getTime()
	{
		return glfwGetTime();
	}

	/**
	 Returns ASCII charcter code for key pressed. Non-ASCII keyboard codes 
	 are mapped as best as possible to ASCII character codes

	 Can only return 1 key code, order of precedence is:
		ESC,X,W,S,A,D,UP,DOWN,LEFT,RIGHT,NONE

	 Returns:
		unsigned char - ASCII key code or 0 (null)
	*/
	unsigned char getKey()
	{
		unsigned char key = 0;	// none

		if (glfwGetKey(s_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			key = 0x1B; // ESC
		else if (glfwGetKey(s_window, GLFW_KEY_X) == GLFW_PRESS)
			key = 'X';
		else if (glfwGetKey(s_window, GLFW_KEY_W) == GLFW_PRESS)
			key = 'W';
		else if (glfwGetKey(s_window, GLFW_KEY_S) == GLFW_PRESS)
			key = 'S';
		else if (glfwGetKey(s_window, GLFW_KEY_A) == GLFW_PRESS)
			key = 'A';
		else if (glfwGetKey(s_window, GLFW_KEY_D) == GLFW_PRESS)
			key = 'D';
		else if (glfwGetKey(s_window, GLFW_KEY_UP) == GLFW_PRESS)
			key = 'W';
		else if (glfwGetKey(s_window, GLFW_KEY_DOWN) == GLFW_PRESS)
			key = 'S';
		else if (glfwGetKey(s_window, GLFW_KEY_LEFT) == GLFW_PRESS)
			key = 'A';
		else if (glfwGetKey(s_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			key = 'D';

		return key;
	}

	/**
	* Check for system events like keyboard input or window closing
	 Returns:
		void
	*/
	void getEvents()
	{
		// poll for and process events
		glfwPollEvents();
	}

	/**
	* cleanup and exit the OpenGL environment
	 Returns:
		void
	*/
	void cleanup()
	{
		glfwTerminate();
	}


	/**
	 Draw a 4 sided filled block
	 Parameters:
		x		- left side coordinate
		y		- bottom coordinate
		width	- in pixels
		height	- in pixels
		color	- fill color (default=White)
	 Returns:
		void
	*/
	void drawQuad(float x, float y, float width, float height, unsigned int color)
	{
		GLfloat vertices[] =
		{
			 x        , y         , 	// bottom left corner
			 x + width, y         , 	// bottom Right corner
			 x + width, y + height, 	// top right corner
			 x        , y + height  	// top left corner
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		setColor(color);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	/**
	 Draw a scaled filled point, i.e. 1 or more pixel dot.
	 Use drawCircle for anything more than 2-3 pixes to get a
	 smoot circle
	 Parameters:
		x		- horizontal center
		y		- vertical center
		size	- of point in pixels (default=1)
		color	- fill color (default=White)
		smooth	- smooth edges of circle as best as possible (default=true)
	 Returns:
		void
	*/
	void drawPoint(float x, float y, float size, unsigned int color, bool smooth)
	{
		GLfloat pointVertex[] = { x, y };

		glPushAttrib(GL_POINT_BIT);
		if (smooth)
			glEnable(GL_POINT_SMOOTH);
		glEnableClientState(GL_VERTEX_ARRAY);
		glPointSize(size);
		glVertexPointer(2, GL_FLOAT, 0, pointVertex);
		setColor(color);
		glDrawArrays(GL_POINTS, 0, 1);
		glDisableClientState(GL_VERTEX_ARRAY);
		if (smooth)
			glDisable(GL_POINT_SMOOTH);
		glPopAttrib();
	}

	/**
	 Draw a 4 sided filled block
	 Parameters:
		x1		- left side coordinate for point 1
		y1		- bottom coordinate for point 1
		x2		- left side coordinate for point 2
		y2		- bottom coordinate for point 2
		width	- of line in pixels
		color	- fill color (default=White)
		smooth	- smooth edges angled lines as best as possible (default=true)
	 Returns:
		void
	*/
	void drawLine(float x1, float y1, float x2, float y2, float width, unsigned int color, bool smooth)
	{
		GLfloat lineVertices[] = {
			x1, y1,
			x2, y2
		};


		glPushAttrib(GL_LINE_BIT);
		if (smooth)
			glEnable(GL_LINE_SMOOTH);
		glLineWidth(width);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, lineVertices);
		setColor(color);
		glDrawArrays(GL_LINES, 0, 2);
		glDisableClientState(GL_VERTEX_ARRAY);
		if (smooth)
			glDisable(GL_LINE_SMOOTH);
		glPopAttrib(); // restore line attributes
	}

	/**
	 Draw a filled circle made up of a triangle-fan.  360 triangles will
	 produce a smooth circle.  6 triangles will produce a hexagon.  etc.
	 Parameters:
		x		- horizontal center
		y		- vertical center
		radius	- of circle in pixels
		color	- fill color (default=White)
		sides	- number of triangles drawn (default=360)
	 Returns:
		void
	*/
	void drawCircle(float x, float y, float radius, unsigned int color, int sides)
	{
		GLint numberOfVertices = sides + 2;

		GLfloat doublePi = 2.0f * M_PI;

		GLfloat* circleVerticesX = new GLfloat[numberOfVertices];
		GLfloat* circleVerticesY = new GLfloat[numberOfVertices];

		circleVerticesX[0] = x;
		circleVerticesY[0] = y;

		for (int i = 1; i < numberOfVertices; i++)
		{
			circleVerticesX[i] = x + (radius * cos(i * doublePi / sides));
			circleVerticesY[i] = y + (radius * sin(i * doublePi / sides));
		}

		GLfloat* allCircleVertices = new GLfloat[numberOfVertices * 2];

		for (int i = 0; i < numberOfVertices; i++)
		{
			allCircleVertices[i * 2] = circleVerticesX[i];
			allCircleVertices[(i * 2) + 1] = circleVerticesY[i];
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, allCircleVertices);
		setColor(color);
		glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
		glDisableClientState(GL_VERTEX_ARRAY);

		delete[] circleVerticesX;
		delete[] circleVerticesY;
		delete[] allCircleVertices;
	}

	/**
	 Draw 8x8 pixel characters as text on the screen
	 Parameters:
		x		- left side of first character
		y		- bottom of of characters
		text	- string of characters to draw
		size	- multiplier for size of characters (default=1), i.e 2=16x16
		color	- fill color (default=White)
	 Returns:
		void
	*/
	void drawText(float x, float y, std::string text, int size, unsigned int color)
	{
		GLfloat xpos, ypos;

		setColor(color);

		for (int c = 0; c < text.length(); c++)
		{
			ypos = y + 8;
			for (int i = 0; i < 8; i++)
			{
				xpos = x;
				GLubyte byte = CHARACTERS[text[c] - 32][i];
				for (int b = 0; b < 8; b++)
				{
					if (byte & 0x80)
					{
						for (GLfloat ys = ypos; ys < ypos + size; ys++)
						{
							for (GLfloat xs = xpos; xs < xpos + size; xs++)
							{
								drawPoint(xs, ys, 1);
							}
						}
					}
					byte = byte << 1;
					xpos += size;
				}
				ypos -= size;
			}
			x = xpos;
		}
	}

	//-----------------------------------------------------------------------------
	// GLFW function declarations
	//-----------------------------------------------------------------------------

	/**
	 callback function that keep the viewport size in sync with changes user makes
	 to the displayed window
	 parameters:
		window		- pointer to the GLFW window object
		width		- new width of the window on the screen
		height		- new height of the window on the screen
	 returns:
		void
	 */
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	//-----------------------------------------------------------------------------
	// private functions
	//-----------------------------------------------------------------------------

	/**
	 Set the global OpenGL 3-float color matrix to an integer color RGB
	 Parameters
		color - 3-byte value in RGB form
	*/
	void setColor(unsigned int color)
	{
		GLfloat red = ((color >> 16) & 0xFF) / 255.0;
		GLfloat green = ((color >> 8) & 0xFF) / 255.0;
		GLfloat blue = (color & 0xFF) / 255.0;
		glColor3f(red, green, blue);
	}

} // namespace fgcugl
