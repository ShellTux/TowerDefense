#ifndef INCLUDE_APP_BASE_HPP_
#define INCLUDE_APP_BASE_HPP_

#include "TowerDefense/Field.hpp"
#include "TowerDefense/Stats.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cstdlib>
#include <string>
#include <unordered_map>

class App {
      public:
	using WindowMap = std::unordered_map<GLFWwindow *, App *>;
	using Clock     = std::chrono::steady_clock;

	App(const std::string &title,
	    const u32 width,
	    const u32 height,
	    const u32 fieldRows,
	    const u32 fieldCols,
	    const u8 fieldWaves);

	App(App &&)                 = default;
	App(const App &)            = default;
	App &operator=(App &&)      = default;
	App &operator=(const App &) = default;
	~App();

	static WindowMap allApps;

	void loop();

	void printStats() const;

      private:
	bool pause         = false;
	bool focusMinimap  = false;
	GLFWwindow *window = nullptr;
	u32 width          = 0;
	u32 height         = 0;
	u64 fps            = 60;
	u8 cull            = 0;
	u8 polygonMode     = 0;
	u8 view            = 0;
	f64 orbitAngle     = 0;
	u8 gameSpeed       = 1;
	u8 maxGameSpeed    = 16;
	u8 lighting        = 0;

	Clock::time_point lastFrameTime         = Clock::now();
	TowerDefense::Stats::TimeMs deltaTimeMs = 0;

	TowerDefense::Field field;

	static constexpr GLbitfield glMask = GL_VIEWPORT_BIT | GL_TRANSFORM_BIT
	                                     | GL_POLYGON_BIT | GL_LIGHTING_BIT;

	void setup();
	void update();
	void updateTime();
	void draw();

	void drawField();
	void drawHUD();
	void drawMinimap();

	static constexpr char windowClass[] = "OpenGL";
	static void ErrorCallback(const int errorCode, const char *description);
	static void FramebufferCallback(GLFWwindow *window,
	                                const int width,
	                                const int height);
	static void KeyCallback(GLFWwindow *window,
	                        const int key,
	                        const int scancode,
	                        const int action,
	                        const int mods);
	static void ScrollCallback(GLFWwindow *window,
	                           const double xoffset,
	                           const double yoffset);


	void KeyPress(const int key);
	void KeyRelease(const int key);
	void MouseScroll(const f64 xoffset, const f64 yoffset);
};

#endif // INCLUDE_APP_BASE_HPP_
