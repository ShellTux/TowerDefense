#include "TowerDefense/Cannon/Base.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Field.hpp"
#include "TowerDefense/Tower.hpp"
#include "Vec3.hpp"

#include <GL/gl.h>
#include <GL/glext.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <thread>

#ifdef DEBUG
	#include <iostream>
#endif

#ifdef OPEN_FRAMEWORKS
#else
	#include <GLFW/glfw3.h>

static constexpr int defaultWidth    = 600;
static constexpr int defaultHeight   = 600;
static constexpr char defaultTitle[] = "Tower Defense";
static constexpr char windowClass[]  = "OpenGL";

GLFWwindow *window = nullptr;

void errorCallback(const int errorCode, const char *description);
void framebufferCallback(GLFWwindow *window, const int width, const int height);
void keyCallback(GLFWwindow *window,
                 const int key,
                 const int scancode,
                 const int action,
                 const int mods);

void setup();
void update();
void draw();
#endif

int main()
{
#ifndef RELEASE
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
#endif

	using TowerDefense::Tower, TowerDefense::Field;
	using TowerDefense::Vec3, TowerDefense::Enemy, TowerDefense::Cannon;

#ifdef DEBUG
	Vec3 v = Vec3::RandomUnitVec3();
	std::cout << "||" << v << "|| = " << v.magnitude() << std::endl;

	Enemy e(.5, 0.1, 10, 10, {0.5, 0.5, 0});
	std::cout << std::endl << e << std::endl;

	Cannon c;
	std::cout << std::endl << c << std::endl;

	Tower t;
	std::cout << std::endl << t << std::endl;

	#define F TowerDefense::Field::Cell::Floor
	#define W TowerDefense::Field::Cell::Wall
	#define S TowerDefense::Field::Cell::Slot
	Field f({
	    {W, W, W, W, W},
	    {6, 5, 4, 3, W},
	    {W, W, S, 2, W},
	    {W, W, W, 1, 0},
	    {W, W, W, W, W},
	    {W, W, W, W, W},
	});
	#undef F
	#undef W
	#undef S

	std::cout << std::endl << f << std::endl;
#else
	glfwSetErrorCallback(errorCallback);

	if (glfwInit() == GLFW_FALSE) {
		return 1;
	}

	glfwWindowHintString(GLFW_X11_CLASS_NAME, windowClass);
	glfwWindowHintString(GLFW_WAYLAND_APP_ID, windowClass);

	window = glfwCreateWindow(defaultWidth,
	                          defaultHeight,
	                          defaultTitle,
	                          nullptr,
	                          nullptr);

	if (window == nullptr) {
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferCallback);
	glfwSetKeyCallback(window, keyCallback);

	setup();

	while (glfwWindowShouldClose(window) == 0) {
		update();
		draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		// HACK: 16ms == 1 / 60FPS
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	glfwDestroyWindow(window);
	glfwTerminate();
#endif

	return 0;
}
