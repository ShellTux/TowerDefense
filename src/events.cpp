#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <iostream>

enum DebugInput : uint32_t {
	All,
	KeyPress,
	KeyRelease,
	MouseEntered,
	MouseExited,
	MouseMove,
	MousePress,
	MouseRelease,
	MouseScrolled,
};

static int debugInput  = KeyPress;
static int cull        = 0;
static int polygonMode = 0;

static bool isDebugInputOn(const int debugFlag)
{
	if (debugInput == All) {
		return true;
	}

	return (debugInput & debugFlag) != 0u;
}

static void keyPress(GLFWwindow *window,
                     const int key,
                     const int scancode,
                     const int action)
{
	(void) window;
	(void) scancode;
	(void) action;

	if (isDebugInputOn(KeyPress)) {
		std::cout << "Key Pressed: " << glfwGetKeyName(key, 0)
		          << std::endl;
	}

	switch (key) {
	case GLFW_KEY_C: {
		cull = (cull + 1) % 4;

		std::cout << "Cull: " << cull << std::endl;
		switch (cull) {
		case 0:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			break;
		case 1:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;
		case 2:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
			break;
		case 3:
			glDisable(GL_CULL_FACE);
			break;
		}
	}; break;
	case GLFW_KEY_P: {
		polygonMode = (polygonMode + 1) % 2;

		switch (polygonMode) {
		case 0:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}

	} break;
	}
}

void keyCallback(GLFWwindow *window,
                 const int key,
                 const int scancode,
                 const int action,
                 const int mods)
{
	(void) mods;

	switch (action) {
	case GLFW_PRESS: {
		keyPress(window, key, scancode, action);
	} break;
	}
}
