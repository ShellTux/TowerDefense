#ifndef INCLUDE_APP_BASE_HPP_
#define INCLUDE_APP_BASE_HPP_

#include "Math.hpp"
#include "TowerDefense/Field.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <chrono>
#include <cstdlib>
#include <optional>
#include <ostream>
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

	class Look {
	      public:
		enum Type : u32 {
			Front3rd,
			Left3rd,
			Back3rd,
			Right3rd,
			Above3rd,
			Below3rd,
			OrbitHorizontal3rd,
			OrbitVertical3rd,
			FreeView3rd,
			FirstView,
		};

		Look()                        = default;
		Look(const Look &)            = default;
		Look(Look &&)                 = default;
		Look &operator=(const Look &) = default;
		Look &operator=(Look &&)      = default;
		Look &operator+=(const usize i);
		~Look() = default;

		friend std::ostream &operator<<(std::ostream &os,
		                                const Look &look)
		{
			switch (look.value) {
			case App::Look::Front3rd: {
				os << "Front3rd";
			} break;
			case App::Look::Left3rd: {
				os << "Left3rd";
			} break;
			case App::Look::Back3rd: {
				os << "Back3rd";
			} break;
			case App::Look::Right3rd: {
				os << "Right3rd";
			} break;
			case App::Look::Above3rd: {
				os << "Above3rd";
			} break;
			case App::Look::Below3rd: {
				os << "Below3rd";
			} break;
			case App::Look::OrbitHorizontal3rd: {
				os << "OrbitHorizontal3rd";
			} break;
			case App::Look::OrbitVertical3rd: {
				os << "OrbitVertical3rd";
			} break;
			case App::Look::FreeView3rd: {
				os << "FreeView3rd";
			} break;
			case App::Look::FirstView: {
				os << "FirstView";
			} break;
			}

			os << " (φ: " << look.freeViewAzimuthal
			   << ", ϑ: " << look.freeViewPolar << ")";

			return os;
		}

		Look &reset();

		void lookAt(const std::optional<Vec3> &pos,
		            const Vec3 orbitCenter,
		            const f64 orbitRadius,
		            const f64 azimuthalAngle,
		            const f64 polarAngle) const;

		Type value{Front3rd};
		f64 freeViewAzimuthal{};
		f64 freeViewPolar{Math::PId / 4};
	};

	enum class Key {
		KeyQ,
		KeyW,
		KeyE,
		KeyR,
		KeyT,
		KeyY,
		KeyU,
		KeyI,
		KeyO,
		KeyP,
		KeyA,
		KeyS,
		KeyD,
		KeyF,
		KeyG,
		KeyH,
		KeyJ,
		KeyK,
		KeyL,
		KeyZ,
		KeyX,
		KeyC,
		KeyV,
		KeyB,
		KeyN,
		KeyM,

		KeyUp,
		KeyDown,
		KeyLeft,
		KeyRight,

		Key0,
		Key1,
		Key2,
		Key3,
		Key4,
		Key5,
		Key6,
		Key7,
		Key8,
		Key9,
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

	std::string &getTitle() const;

      private:
#ifdef NOOF
	GLFWwindow *window = nullptr;
#endif
	bool pause        = false;
	bool focusMinimap = false;
	u32 width         = 0;
	u32 height        = 0;
	u64 fps           = 60;
	u8 cull           = 0;
	u8 polygonMode    = 0;
	f64 orbitAngle    = 0;
	u8 gameSpeed      = 1;
	u8 maxGameSpeed   = 16;
	u8 lighting       = 0;
	Look view;
	std::string windowTitle;
	std::optional<Vec3> selectedView      = std::nullopt;
	std::optional<u32> selectedEnemyIndex = std::nullopt;

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
