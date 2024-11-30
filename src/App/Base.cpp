#include "App.hpp"
#include "Math.hpp"
#include "OpenGL/camera.hpp"
#include "Texture.hpp"
#include "TowerDefense/Field.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>
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
	app->width  = width;
	app->height = height;
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
	std::cout << "\033[2J\033[H";
	std::cout << "-------Tower Defense Stats-------" << std::endl;
	std::cout << "\033[5m" << "\033[35m" << "Wave " << field.getWave()
	          << "\033[0m" << std::endl;
	std::cout << "Points: " << field.getPoints() << std::endl;
	std::cout << "Tower Health(%): "
	          << field.getTower().getHealthRatio() * 100 << "%"
	          << std::endl;
	std::cout << "Game Speed: " << static_cast<int>(gameSpeed) << std::endl;
	std::cout << "Selected Position: ";
	field.printInfoAtSelectedPosition();
	std::cout << "Cannons: " << field.getCannonsSize() << "/"
	          << static_cast<int>(field.getRemainingCannons()
	                              + field.getCannonsSize())
	          << std::endl;
	std::cout << "Enemies: " << field.getEnemiesSize() << std::endl;
	std::cout << "Orbit Angle: ";
	std::cout << Math::radiansToDegrees(orbitAngle);
	std::cout << "º" << std::endl;
	std::cout << "view: " << view << std::endl;
	std::cout << "selected view: ";
	if (selectedView.has_value()) {
		std::cout << selectedView.value() << std::endl;
	} else {
		std::cout << "none" << std::endl;
	}
	std::cout << "selected enemy index: "
	          << static_cast<int>(selectedEnemyIndex.value_or(-1))
	          << std::endl;
	std::cout << "---------------------------------" << std::endl;
	std::cout << std::endl;

#define TOWER_DEFENSE_KEYS                                             \
	"---------------------Tower Defense "                          \
	"Keys---------------------------\n"                            \
	"WASD               Move selected position\n"                  \
	"Up,Left,Down,Right Move selected position\n"                  \
	"1,2,3              Place Cannon at selected position of "     \
	"tier A,B,C "                                                  \
	"(respectively)\n"                                             \
	"4/U                Upgrade Cannon at selected position\n"     \
	"I                  Print Info at selected position (debug "   \
	"mode).\n"                                                     \
	"C                  Toggle between cull face modes (opengl "   \
	"debug "                                                       \
	"mode).\n"                                                     \
	"T                  Toggle between polygon modes (opengl "     \
	"debug "                                                       \
	"mode).\n"                                                     \
	"L                  Switch between lighting modes.\n"          \
	"V                  Switch between views\n"                    \
	"M                  Focus Minimap\n"                           \
	"P                  Pause Game\n"                              \
	"R                  Reset view and minimap focus to default\n" \
	"------------------------------------------------------------" \
	"------"

	std::cout << TOWER_DEFENSE_KEYS << std::endl;
#undef TOWER_DEFENSE_KEYS
}

App::Look &App::Look::operator+=(const usize i)
{
	for (usize j = 0; j < i; ++j) {
		switch (value) {
		case Front3rd: {
			value = Left3rd;
		} break;
		case Left3rd: {
			value = Back3rd;
		} break;
		case Back3rd: {
			value = Right3rd;
		} break;
		case Right3rd: {
			value = Above3rd;
		} break;
		case Above3rd: {
			value = Below3rd;
		} break;
		case Below3rd: {
			value = OrbitHorizontal3rd;
		} break;
		case OrbitHorizontal3rd: {
			value = OrbitVertical3rd;
		} break;
		case OrbitVertical3rd: {
			value = FreeView3rd;
		} break;
		case FreeView3rd: {
			value = FirstView;
		} break;
		case FirstView:
			value = Front3rd;
			break;
		}
	}

	return *this;
}

void App::Look::lookAt(const std::optional<Vec3> &pos,
                       const Vec3 orbitCenter,
                       const f64 orbitRadius,
                       const f64 azimuthalAngle,
                       const f64 polarAngle) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static Vec3 camera;
	static Vec3 target;
	static Vec3 up = {0, 0, -1};

	if (pos.has_value()) {
		const Vec3 realPos               = pos->transpose2D();
		static constexpr f64 targetAngle = 0;

		// NOTE: Target
		switch (value) {
		case Front3rd:
		case Left3rd:
		case Back3rd:
		case Right3rd:
		case Above3rd:
		case Below3rd:
		case OrbitHorizontal3rd:
		case OrbitVertical3rd:
		case FreeView3rd: {
			target = realPos;
		} break;
		case FirstView: {
			target = realPos - Vec3(0, 0, -.5)
			         + Vec3::Polar2D(targetAngle) * 2;
		} break;
		}

		// NOTE: Camera
		switch (value) {
		case Front3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(3 * Math::PId / 2, Math::PId / 4)
			            * radius;
		} break;
		case Left3rd: {
			camera = realPos
			         + Vec3::Polar3D(Math::PId, Math::PId / 4)
			               * radius;
		} break;
		case Back3rd: {
			camera = realPos
			         + Vec3::Polar3D(Math::PId / 2, Math::PId / 4)
			               * radius;
		} break;
		case Right3rd: {
			camera = realPos
			         + Vec3::Polar3D(0, Math::PId / 4) * radius;
		} break;
		case Above3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(3 * Math::PId / 2, Math::PId / 4)
			            * radius;
		} break;
		case Below3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(3 * Math::PId / 2, -Math::PId / 4)
			            * radius;
		} break;
		case OrbitHorizontal3rd: {
			camera = realPos
			         + Vec3::Polar3D(azimuthalAngle, Math::PId / 4)
			               * radius;
		} break;
		case OrbitVertical3rd: {
			camera = realPos
			         + Vec3::Polar3D(3 * Math::PId / 2, polarAngle)
			               * radius;
		} break;
		case FreeView3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(freeViewAzimuthal, freeViewPolar)
			            * radius;
		} break;
		case FirstView: {
			camera = realPos + Vec3::Polar2D(targetAngle);
		} break;
		}

		OpenGL::Camera::LookAt(camera, target, up);
		return;
	}

	// NOTE: Target
	switch (value) {
	case Front3rd:
	case Left3rd:
	case Back3rd:
	case Right3rd:
	case Above3rd:
	case Below3rd:
	case OrbitHorizontal3rd:
	case OrbitVertical3rd:
	case FreeView3rd: {
		target = orbitCenter;
	} break;
	case FirstView:
		break;
	}

	// NOTE: Camera
	switch (value) {
	case Front3rd: {
		camera
		    = orbitCenter
		      + Vec3::Polar3D(Math::PId / 2, polarAngle) * orbitRadius;
	} break;
	case Left3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(2 * Math::PId / 2, polarAngle)
		               * orbitRadius;
	} break;
	case Back3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(3 * Math::PId / 2, polarAngle)
		               * orbitRadius;
	} break;
	case Right3rd: {
		camera
		    = orbitCenter
		      + Vec3::Polar3D(2 * Math::PId, polarAngle) * orbitRadius;
	} break;
	case Above3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(0, Math::PId / 2) * orbitRadius;
	} break;
	case Below3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(0, 3 * Math::PId / 2) * orbitRadius;
	} break;
	case OrbitHorizontal3rd: {
		camera
		    = orbitCenter
		      + Vec3::Polar3D(azimuthalAngle, polarAngle) * orbitRadius;
	} break;
	case OrbitVertical3rd: {
		// TODO: Vertical Orbit
		camera
		    = orbitCenter
		      + Vec3::Polar3D(polarAngle, azimuthalAngle) * orbitRadius;
	} break;
	case FreeView3rd: {
		camera
		    = orbitCenter
		      + (pos.has_value()
		             ? Vec3::Polar3D(azimuthalAngle, polarAngle)
		             : Vec3::Polar3D(freeViewAzimuthal, freeViewPolar))
		            * orbitRadius;
	} break;
	case FirstView: {
	} break;
	}

	OpenGL::Camera::LookAt(camera, target, up);
}

App::Look &App::Look::reset()
{
	value             = Front3rd;
	freeViewAzimuthal = 0;
	freeViewPolar     = Math::PId / 4;

	return *this;
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
