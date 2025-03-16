#ifndef INCLUDE_APP_BASE_HPP_
#define INCLUDE_APP_BASE_HPP_

#include "OpenGL/Camera.hpp"
#include "OpenGL/Cull.hpp"
#include "OpenGL/PolygonMode.hpp"
#include "TowerDefense/Field.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <chrono>
#include <cstdlib>
#include <optional>
#include <string>

#ifdef NOOF
	#include <GLFW/glfw3.h>
	#include <unordered_map>
#endif

class App {
      public:
#ifdef NOOF
	using WindowMap = std::unordered_map<GLFWwindow *, App *>;
#endif
	using Clock = std::chrono::steady_clock;

#define KEYS_OF_IMPLEMENTED                               \
	KEY(KeySpace, GLFW_KEY_SPACE, ' ')                \
	KEY(KeyApostrophe, GLFW_KEY_APOSTROPHE, '\'')     \
	KEY(KeyComma, GLFW_KEY_COMMA, ',')                \
	/* KEY(KeyMinus, GLFW_KEY_MINUS, '-') */          \
	KEY(KeyPeriod, GLFW_KEY_PERIOD, '.')              \
	/* KEY(KeySlash, GLFW_KEY_SLASH, '/') */          \
	KEY(Key0, GLFW_KEY_0, '0')                        \
	KEY(Key1, GLFW_KEY_1, '1')                        \
	KEY(Key2, GLFW_KEY_2, '2')                        \
	KEY(Key3, GLFW_KEY_3, '3')                        \
	KEY(Key4, GLFW_KEY_4, '4')                        \
	KEY(Key5, GLFW_KEY_5, '5')                        \
	KEY(Key6, GLFW_KEY_6, '6')                        \
	KEY(Key7, GLFW_KEY_7, '7')                        \
	KEY(Key8, GLFW_KEY_8, '8')                        \
	KEY(Key9, GLFW_KEY_9, '9')                        \
	KEY(KeySemicolon, GLFW_KEY_SEMICOLON, ';')        \
	KEY(KeyEqual, GLFW_KEY_EQUAL, '=')                \
	KEY(KeyA, GLFW_KEY_A, 'a')                        \
	KEY(KeyB, GLFW_KEY_B, 'b')                        \
	KEY(KeyC, GLFW_KEY_C, 'c')                        \
	KEY(KeyD, GLFW_KEY_D, 'd')                        \
	KEY(KeyE, GLFW_KEY_E, 'e')                        \
	KEY(KeyF, GLFW_KEY_F, 'f')                        \
	KEY(KeyG, GLFW_KEY_G, 'g')                        \
	KEY(KeyH, GLFW_KEY_H, 'h')                        \
	KEY(KeyI, GLFW_KEY_I, 'i')                        \
	KEY(KeyJ, GLFW_KEY_J, 'j')                        \
	KEY(KeyK, GLFW_KEY_K, 'k')                        \
	KEY(KeyL, GLFW_KEY_L, 'l')                        \
	KEY(KeyM, GLFW_KEY_M, 'm')                        \
	KEY(KeyN, GLFW_KEY_N, 'n')                        \
	KEY(KeyO, GLFW_KEY_O, 'o')                        \
	KEY(KeyP, GLFW_KEY_P, 'p')                        \
	KEY(KeyQ, GLFW_KEY_Q, 'q')                        \
	KEY(KeyR, GLFW_KEY_R, 'r')                        \
	KEY(KeyS, GLFW_KEY_S, 's')                        \
	KEY(KeyT, GLFW_KEY_T, 't')                        \
	KEY(KeyU, GLFW_KEY_U, 'u')                        \
	KEY(KeyV, GLFW_KEY_V, 'v')                        \
	KEY(KeyW, GLFW_KEY_W, 'w')                        \
	KEY(KeyX, GLFW_KEY_X, 'x')                        \
	KEY(KeyY, GLFW_KEY_Y, 'y')                        \
	KEY(KeyZ, GLFW_KEY_Z, 'z')                        \
	KEY(KeyLeftBracket, GLFW_KEY_LEFT_BRACKET, '{')   \
	KEY(KeyRightBracket, GLFW_KEY_RIGHT_BRACKET, '}') \
	KEY(KeyKPDivide, GLFW_KEY_KP_DIVIDE, '/')         \
	KEY(KeyKPMultiply, GLFW_KEY_KP_MULTIPLY, '*')     \
	KEY(KeyKPSubtract, GLFW_KEY_KP_SUBTRACT, '-')     \
	KEY(KeyKPAdd, GLFW_KEY_KP_ADD, '+')

#define KEYS_OF_NOT_IMPLEMENTED                         \
	KEY(KeyNone, 0, 0)                              \
	KEY(KeyBackslash, GLFW_KEY_BACKSLASH, 0)        \
	KEY(KeyGraveAccent, GLFW_KEY_GRAVE_ACCENT, 0)   \
	KEY(KeyEscape, GLFW_KEY_ESCAPE, 0)              \
	KEY(KeyEnter, GLFW_KEY_ENTER, 0)                \
	KEY(KeyTab, GLFW_KEY_TAB, 0)                    \
	KEY(KeyBackspace, GLFW_KEY_BACKSPACE, 0)        \
	KEY(KeyInsert, GLFW_KEY_INSERT, 0)              \
	KEY(KeyDelete, GLFW_KEY_DELETE, 0)              \
	KEY(KeyRight, GLFW_KEY_RIGHT, 0)                \
	KEY(KeyLeft, GLFW_KEY_LEFT, 0)                  \
	KEY(KeyDown, GLFW_KEY_DOWN, 0)                  \
	KEY(KeyUp, GLFW_KEY_UP, 0)                      \
	KEY(KeyPageUp, GLFW_KEY_PAGE_UP, 0)             \
	KEY(KeyPageDown, GLFW_KEY_PAGE_DOWN, 0)         \
	KEY(KeyHome, GLFW_KEY_HOME, 0)                  \
	KEY(KeyEnd, GLFW_KEY_END, 0)                    \
	KEY(KeyCapsLock, GLFW_KEY_CAPS_LOCK, 0)         \
	KEY(KeyScrollLock, GLFW_KEY_SCROLL_LOCK, 0)     \
	KEY(KeyNumLock, GLFW_KEY_NUM_LOCK, 0)           \
	KEY(KeyPrintScreen, GLFW_KEY_PRINT_SCREEN, 0)   \
	KEY(KeyPause, GLFW_KEY_PAUSE, 0)                \
	KEY(KeyF1, GLFW_KEY_F1, 0)                      \
	KEY(KeyF2, GLFW_KEY_F2, 0)                      \
	KEY(KeyF3, GLFW_KEY_F3, 0)                      \
	KEY(KeyF4, GLFW_KEY_F4, 0)                      \
	KEY(KeyF5, GLFW_KEY_F5, 0)                      \
	KEY(KeyF6, GLFW_KEY_F6, 0)                      \
	KEY(KeyF7, GLFW_KEY_F7, 0)                      \
	KEY(KeyF8, GLFW_KEY_F8, 0)                      \
	KEY(KeyF9, GLFW_KEY_F9, 0)                      \
	KEY(KeyF10, GLFW_KEY_F10, 0)                    \
	KEY(KeyF11, GLFW_KEY_F11, 0)                    \
	KEY(KeyF12, GLFW_KEY_F12, 0)                    \
	KEY(KeyF13, GLFW_KEY_F13, 0)                    \
	KEY(KeyF14, GLFW_KEY_F14, 0)                    \
	KEY(KeyF15, GLFW_KEY_F15, 0)                    \
	KEY(KeyF16, GLFW_KEY_F16, 0)                    \
	KEY(KeyF17, GLFW_KEY_F17, 0)                    \
	KEY(KeyF18, GLFW_KEY_F18, 0)                    \
	KEY(KeyF19, GLFW_KEY_F19, 0)                    \
	KEY(KeyF20, GLFW_KEY_F20, 0)                    \
	KEY(KeyF21, GLFW_KEY_F21, 0)                    \
	KEY(KeyF22, GLFW_KEY_F22, 0)                    \
	KEY(KeyF23, GLFW_KEY_F23, 0)                    \
	KEY(KeyF24, GLFW_KEY_F24, 0)                    \
	KEY(KeyF25, GLFW_KEY_F25, 0)                    \
	KEY(KeyKP0, GLFW_KEY_KP_0, 0)                   \
	KEY(KeyKP1, GLFW_KEY_KP_1, 0)                   \
	KEY(KeyKP2, GLFW_KEY_KP_2, 0)                   \
	KEY(KeyKP3, GLFW_KEY_KP_3, 0)                   \
	KEY(KeyKP4, GLFW_KEY_KP_4, 0)                   \
	KEY(KeyKP5, GLFW_KEY_KP_5, 0)                   \
	KEY(KeyKP6, GLFW_KEY_KP_6, 0)                   \
	KEY(KeyKP7, GLFW_KEY_KP_7, 0)                   \
	KEY(KeyKP8, GLFW_KEY_KP_8, 0)                   \
	KEY(KeyKP9, GLFW_KEY_KP_9, 0)                   \
	KEY(KeyKPDecimal, GLFW_KEY_KP_DECIMAL, 0)       \
	KEY(KeyKPEnter, GLFW_KEY_KP_ENTER, 0)           \
	KEY(KeyKPEqual, GLFW_KEY_KP_EQUAL, 0)           \
	KEY(KeyLeftShift, GLFW_KEY_LEFT_SHIFT, 0)       \
	KEY(KeyLeftControl, GLFW_KEY_LEFT_CONTROL, 0)   \
	KEY(KeyLeftAlt, GLFW_KEY_LEFT_ALT, 0)           \
	KEY(KeyLEFT_SUPER, GLFW_KEY_LEFT_SUPER, 0)      \
	KEY(KeyRightShift, GLFW_KEY_RIGHT_SHIFT, 0)     \
	KEY(KeyRightControl, GLFW_KEY_RIGHT_CONTROL, 0) \
	KEY(KeyRightAlt, GLFW_KEY_RIGHT_ALT, 0)         \
	KEY(KeyRightSuper, GLFW_KEY_RIGHT_SUPER, 0)     \
	KEY(KeyMenu, GLFW_KEY_MENU, 0)

#define KEYS                \
	KEYS_OF_IMPLEMENTED \
	KEYS_OF_NOT_IMPLEMENTED


	enum class Key {
#define KEY(ENUM, GLFW_KEY, OF_KEY) ENUM,
		KEYS
#undef KEY
	};

#ifdef NOOF
	static Key FromGLFWKey(const i32 key);
#endif

	App(const std::string &title,
	    const u32 width,
	    const u32 height,
	    const u32 fieldRows,
	    const u32 fieldCols,
	    const u8 fieldWaves);

	App()
	    : App("", 600, 600, 10, 10, 10)
	{}
	App(App &&)                 = default;
	App(const App &)            = default;
	App &operator=(App &&)      = default;
	App &operator=(const App &) = default;
	~App();

#ifdef NOOF
	static WindowMap allApps;
#endif

	void draw();
	void loop();
	void setup();
	void update();
	void updateDimensions(const u32 width, const u32 height);

	void KeyPress(const Key key);
	void KeyRelease(const Key key);
	void MouseScroll(const f64 xoffset, const f64 yoffset);

	void printStats() const;

	[[nodiscard]] std::string &getTitle() const;

      private:
#ifdef NOOF
	GLFWwindow *window = nullptr;
#endif
	bool pause        = false;
	bool focusMinimap = false;
	u32 width         = 0;
	u32 height        = 0;
	u64 fps           = 60;
	f64 orbitAngle    = 0;
	u8 gameSpeed      = 1;
	u8 maxGameSpeed   = 16;
	u8 lighting       = 0;
	std::string windowTitle;

	std::optional<Vec3> selectedView  = std::nullopt;
	std::optional<u32> selectedEnemyI = std::nullopt;
	OpenGL::Camera camera;

	OpenGL::Cull cull;

	OpenGL::PolygonMode polygonMode;

	Clock::time_point lastFrameTime         = Clock::now();
	TowerDefense::Stats::TimeMs deltaTimeMs = 0;

	TowerDefense::Field field;

	static constexpr GLbitfield glMask = GL_VIEWPORT_BIT | GL_TRANSFORM_BIT
	                                     | GL_POLYGON_BIT | GL_LIGHTING_BIT;

	void updateTime();

	void drawField();
	void drawHUD();
	void drawMinimap();

#ifdef NOOF
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
#endif
};

#endif // INCLUDE_APP_BASE_HPP_
