#include "App.hpp"
#include "TowerDefense/Field.hpp"
#include "types.hpp"

#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <thread>

App::WindowMap App::allApps{};

App::App(const std::string &title,
         const u32 width,
         const u32 height,
         const u32 fieldRows,
         const u32 fieldCols,
         const u8 fieldWaves)
    : field(TowerDefense::Field::Generate(fieldRows, fieldCols, fieldWaves))
{
	using std::srand, std::time;
	srand(static_cast<unsigned int>(time(nullptr)));

	glfwSetErrorCallback(ErrorCallback);

	if (glfwInit() == GLFW_FALSE) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHintString(GLFW_X11_CLASS_NAME, windowClass);
	glfwWindowHintString(GLFW_WAYLAND_APP_ID, windowClass);

	window = glfwCreateWindow((int) width,
	                          (int) height,
	                          title.data(),
	                          nullptr,
	                          nullptr);

	this->width  = width;
	this->height = height;

	if (window == nullptr) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferCallback);
	glfwSetKeyCallback(window, KeyCallback);

	allApps[window] = this;
}

App::~App()
{
	allApps.erase(window);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void App::loop()
{
	setup();

	using std::chrono::milliseconds;
	const milliseconds deltaTimeMs(u32(1e3 / 60));

	while (glfwWindowShouldClose(window) == 0) {
		if (!pause) {
			update();
			draw();
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		using std::this_thread::sleep_for;
		sleep_for(deltaTimeMs);
	}
}

void App::ErrorCallback(const int errorCode, const char *description)
{
	std::cerr << "Error(" << errorCode << "): " << description << std::endl;
}

void App::FramebufferCallback(GLFWwindow *window,
                              const int width,
                              const int height)
{
	(void) window;
	(void) width;
	(void) height;
}

void App::KeyCallback(GLFWwindow *window,
                      const int key,
                      const int scancode,
                      const int action,
                      const int mods)
{
	App *app = allApps.at(window);
	(void) scancode;
	(void) mods;

#ifdef DEBUG
	std::cout << "Key Pressed: " << glfwGetKeyName(key, 0) << std::endl;
#endif

	switch (action) {
	case GLFW_PRESS: {
		app->KeyPress(key);
	} break;
	case GLFW_RELEASE: {
		app->KeyRelease(key);
	} break;
	}
}
