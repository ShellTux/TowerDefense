#include "App.hpp"
#include "Math.hpp"
#include "OpenGL/camera.hpp"
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
                       const f64 polarAngle)
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

		static constexpr f64 r = 3;
		// NOTE: Camera
		switch (value) {
		case Front3rd: {
			camera = realPos
			         + Vec3::Polar3D(3 * Math::PI / 2, Math::PI / 4)
			               * r;
		} break;
		case Left3rd: {
			camera = realPos
			         + Vec3::Polar3D(Math::PI, Math::PI / 4) * r;
		} break;
		case Back3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(Math::PI / 2, Math::PI / 4) * r;
		} break;
		case Right3rd: {
			camera = realPos + Vec3::Polar3D(0, Math::PI / 4) * r;
		} break;
		case Above3rd: {
			camera = realPos
			         + Vec3::Polar3D(3 * Math::PI / 2, Math::PI / 4)
			               * r;
		} break;
		case Below3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(3 * Math::PI / 2, -Math::PI / 4)
			            * r;
		} break;
		case OrbitHorizontal3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(azimuthalAngle, Math::PI / 4) * r;
		} break;
		case OrbitVertical3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(3 * Math::PI / 2, polarAngle) * r;
		} break;
		case FreeView3rd: {
			camera
			    = realPos
			      + Vec3::Polar3D(freeViewAzimuthal, freeViewPolar)
			            * r;
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
		      + Vec3::Polar3D(Math::PI / 2, polarAngle) * orbitRadius;
	} break;
	case Left3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(2 * Math::PI / 2, polarAngle)
		               * orbitRadius;
	} break;
	case Back3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(3 * Math::PI / 2, polarAngle)
		               * orbitRadius;
	} break;
	case Right3rd: {
		camera
		    = orbitCenter
		      + Vec3::Polar3D(2 * Math::PI, polarAngle) * orbitRadius;
	} break;
	case Above3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(0, Math::PI / 2) * orbitRadius;
	} break;
	case Below3rd: {
		camera = orbitCenter
		         + Vec3::Polar3D(0, 3 * Math::PI / 2) * orbitRadius;
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
	freeViewPolar     = Math::PI / 4;

	return *this;
}

#ifdef NOOF
App::Key App::FromGLFWKey(const i32 key)
{
	switch (key) {
	case GLFW_KEY_Q:
		return Key::KeyQ;
	case GLFW_KEY_W:
		return Key::KeyW;
	case GLFW_KEY_E:
		return Key::KeyE;
	case GLFW_KEY_R:
		return Key::KeyR;
	case GLFW_KEY_T:
		return Key::KeyT;
	case GLFW_KEY_Y:
		return Key::KeyY;
	case GLFW_KEY_U:
		return Key::KeyU;
	case GLFW_KEY_I:
		return Key::KeyI;
	case GLFW_KEY_O:
		return Key::KeyO;
	case GLFW_KEY_P:
		return Key::KeyP;
	case GLFW_KEY_A:
		return Key::KeyA;
	case GLFW_KEY_S:
		return Key::KeyS;
	case GLFW_KEY_D:
		return Key::KeyD;
	case GLFW_KEY_F:
		return Key::KeyF;
	case GLFW_KEY_G:
		return Key::KeyG;
	case GLFW_KEY_H:
		return Key::KeyH;
	case GLFW_KEY_J:
		return Key::KeyJ;
	case GLFW_KEY_K:
		return Key::KeyK;
	case GLFW_KEY_L:
		return Key::KeyL;
	case GLFW_KEY_Z:
		return Key::KeyZ;
	case GLFW_KEY_X:
		return Key::KeyX;
	case GLFW_KEY_C:
		return Key::KeyC;
	case GLFW_KEY_V:
		return Key::KeyV;
	case GLFW_KEY_B:
		return Key::KeyB;
	case GLFW_KEY_N:
		return Key::KeyN;
	case GLFW_KEY_M:
		return Key::KeyM;

		/*case GLFW_KEY_Up:*/
		/*	return Key::KeyUp;*/
		/*case GLFW_KEY_Down:*/
		/*	return Key::KeyDown;*/
		/*case GLFW_KEY_Left:*/
		/*	return Key::KeyLeft;*/
		/*case GLFW_KEY_Right:*/
		/*	return Key::KeyRight;*/

	case GLFW_KEY_0:
		return Key::Key0;
	case GLFW_KEY_1:
		return Key::Key1;
	case GLFW_KEY_2:
		return Key::Key2;
	case GLFW_KEY_3:
		return Key::Key3;
	case GLFW_KEY_4:
		return Key::Key4;
	case GLFW_KEY_5:
		return Key::Key5;
	case GLFW_KEY_6:
		return Key::Key6;
	case GLFW_KEY_7:
		return Key::Key7;
	case GLFW_KEY_8:
		return Key::Key8;
	case GLFW_KEY_9:
		return Key::Key9;
	}

	return Key::Key0;
}
#endif
