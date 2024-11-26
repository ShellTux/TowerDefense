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
		update();
		draw();

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
	case GLFW_PRESS:
	case GLFW_REPEAT: {
		app->KeyPress(key);
	} break;
	case GLFW_RELEASE: {
		app->KeyRelease(key);
	} break;
	}
}

void App::printStats() const
{
	std::cout << "\033[2J\033[H";
	std::cout << "-------Tower Defense Stats-------" << std::endl;
	std::cout << "\033[5m" << "\033[35m" << "Wave " << field.getWave()
	          << "\033[0m" << std::endl;
	std::cout << "Points: " << field.getPoints() << std::endl;
	std::cout << "Tower Health(%): "
	          << field.getTower().getHealthRatio() * 100 << "%"
	          << std::endl;
	std::cout << "Game Speed: " << static_cast<int>(gameSpeed) << std::endl;
	std::cout << "Selected Position: " << field.getSelectedPosition()
	          << std::endl;
	std::cout << "Cannons: " << field.getCannonsSize() << "/"
	          << static_cast<int>(field.getRemainingCannons()
	                              + field.getCannonsSize())
	          << std::endl;
	std::cout << "Enemies: " << field.getEnemiesSize() << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::cout << std::endl;

#define TOWER_DEFENSE_KEYS                                                     \
	"---------------------Tower Defense Keys---------------------------\n" \
	"WASD               Move selected position\n"                          \
	"Up,Left,Down,Right Move selected position\n"                          \
	"1,2,3              Place Cannon at selected position of tier A,B,C "  \
	"(respectively)\n"                                                     \
	"4/U                Upgrade Cannon at selected position\n"             \
	"I                  Print Info at selected position (debug mode).\n"   \
	"C                  Toggle between cull face modes (opengl debug "     \
	"mode).\n"                                                             \
	"T                  Toggle between polygon modes (opengl debug "       \
	"mode).\n"                                                             \
	"L                  Switch between lighting modes.\n"                  \
	"V                  Switch between views\n"                            \
	"M                  Focus Minimap\n"                                   \
	"P                  Pause Game\n"                                      \
	"R                  Reset view and minimap focus to default\n"         \
	"------------------------------------------------------------------"

	std::cout << TOWER_DEFENSE_KEYS << std::endl;
#undef TOWER_DEFENSE_KEYS
}
