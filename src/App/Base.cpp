#include "App.hpp"
#include "Texture.hpp"
#include "TowerDefense/Field.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

#ifdef NOOF
	#include <GLFW/glfw3.h>
	#include <chrono>
	#include <thread>

bool texturesNeedReload = false;

App::WindowMap App::allApps{};
#endif

App::App(const std::string &title,
         const u32 width,
         const u32 height,
         const u32 fieldRows,
         const u32 fieldCols,
         const u8 fieldWaves)
    : width(width)
    , height(height)
    , windowTitle(title)
    , camera({f64(fieldCols) * .5, f64(fieldRows) * .5},
             f64(fieldCols + fieldRows) * .5,
             width,
             height,
             false,
             true)
    , field(TowerDefense::Field::Generate(fieldRows, fieldCols, fieldWaves))
{
	using std::srand, std::time;
	srand(static_cast<unsigned int>(time(nullptr)));

#ifdef NOOF
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

	if (window == nullptr) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	allApps[window] = this;
#endif

	texturesNeedReload = true;
	Texture::Loads({
	    "assets/red-cannon.png",
	    "assets/orange-cannon.png",
	    "assets/purple-cannon.png",
	    "assets/upgrade.png",
	    "assets/pcb-circuit-1.png",
	    "assets/pcb-circuit-slot.png",
	});
}

App::~App()
{
#ifdef NOOF
	allApps.erase(window);
	glfwDestroyWindow(window);
	glfwTerminate();
#endif
}

void App::loop()
{
#ifdef NOOF
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
#endif
}

#ifdef NOOF
void App::ErrorCallback(const int errorCode, const char *description)
{
	std::cerr << "Error(" << errorCode << "): " << description << std::endl;
}

void App::FramebufferCallback(GLFWwindow *window,
                              const int width,
                              const int height)
{
	glViewport(0, 0, width, height);

	App *app = App::allApps.at(window);
	assert(app != nullptr);

	app->updateDimensions(width, height);
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
		app->KeyPress(FromGLFWKey(key));
	} break;
	case GLFW_RELEASE: {
		app->KeyRelease(FromGLFWKey(key));
	} break;
	}
}

void App::ScrollCallback(GLFWwindow *window,
                         const double xoffset,
                         const double yoffset)
{
	App *app = allApps.at(window);
	assert(app != nullptr);
	app->MouseScroll(xoffset, yoffset);
}
#endif

void App::printStats() const
{
	std::ostringstream os;
	static std::string lines{};

	std::cout << "\033[H" << lines;

	std::cout << "\033[H";

	os << "-------Tower Defense Stats-------" << std::endl;
	os << "\033[5m" << "\033[35m" << "🌊 " << field.getWave() << "\033[0m"
	   << std::endl;
	os << "󰫢 " << field.getPoints() << std::endl;
	os << " " << field.getTower().getHealthRatio() * 100 << "%";
	os << std::endl;
	os << "[X] 󰓅 " << static_cast<int>(gameSpeed) << std::endl;
	os << "[I] 󰋇 " << field.infoAtSelectedPosition() << std::endl;
	os << "(Enemies/Cannons/Cannons Max): " << field.getEnemiesSize() << "/"
	   << field.getCannonsSize() << "/"
	   << field.getCannonsSize() + field.getRemainingCannons() << std::endl;
	os << "[V]  " << camera << std::endl;
	os << "[C] Cull: " << cull << std::endl;
	os << "[T] PolygonMode: " << polygonMode << std::endl;
	os << "---------------------------------" << std::endl;
	os << std::endl;

	static const char *const TOWER_DEFENSE_KEYS
	    = "---------------------Tower Defense Keys-----------------------\n"
	      "WASD/↑←↓→    Move selected position\n"        
	      "1,2,3        Place Cannon at selected position of tier A,B,C "
	      "(respectively)\n"
	      "4/U          Upgrade Cannon at selected position\n"
	      "L            Switch between lighting modes.\n"
	      "M            Focus Minimap\n"
	      "P            Pause Game\n"
	      "R            Reset view and minimap focus to default\n"
	      "-------------------------------------------------------------";

	os << TOWER_DEFENSE_KEYS << std::endl;

	if (os.str().size() > lines.size()) {
		lines = os.str();

		for (char &c : lines) {
			if (c != '\n') {
				c = ' ';
			}
		}
	}

	std::cout << os.str();
}

#ifdef NOOF
App::Key App::FromGLFWKey(const i32 key)
{
	switch (key) {
	#define KEY(ENUM, GLFW_KEY, OF_KEY) \
	case GLFW_KEY:                      \
		return App::Key::ENUM;
		KEYS
	#undef KEY
	}


	return Key::Key0;
}
#endif
