#ifndef INCLUDE_APP_BASE_HPP_
#define INCLUDE_APP_BASE_HPP_

#include "TowerDefense/Field.hpp"
#include "TowerDefense/Stats.hpp"
#include "types.hpp"

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

      private:
	bool focusMinimap  = false;
	GLFWwindow *window = nullptr;
	u32 width          = 0;
	u32 height         = 0;
	u64 fps            = 60;
	u8 cull            = 0;
	u8 polygonMode     = 0;
	u8 view            = 0;
	f64 orbitAngle     = 0;

	Clock::time_point lastFrameTime             = Clock::now();
	TowerDefense::Stats::CooldownMs deltaTimeMs = 0;

	TowerDefense::Field field;

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


	void KeyPress(const int key);
	void KeyRelease(const int key);
};

#endif // INCLUDE_APP_BASE_HPP_
