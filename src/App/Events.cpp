#include "App.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <optional>

#ifdef NOOF
	#include <GLFW/glfw3.h>
#endif

void App::KeyPress(const Key key)
{
	if (key == Key::KeyP) {
		if (pause) {
			lastFrameTime = Clock::now();
		}

		pause = !pause;
	}

	switch (key) {
	case Key::KeyS:
	case Key::KeyDown: {
		field.moveSelectedPosition(Vec3(1, 0));
	} break;
	case Key::KeyW:
	case Key::KeyUp: {
		field.moveSelectedPosition(Vec3(-1, 0));
	} break;
	case Key::KeyA:
	case Key::KeyLeft: {
		field.moveSelectedPosition(Vec3(0, -1));
	} break;
	case Key::KeyD:
	case Key::KeyRight: {
		field.moveSelectedPosition(Vec3(0, 1));
	} break;

	case Key::Key1: {
		field.placeCannon(TowerDefense::Stats::Tier::A);
	} break;
	case Key::Key2: {
		field.placeCannon(TowerDefense::Stats::Tier::B);
	} break;
	case Key::Key3: {
		field.placeCannon(TowerDefense::Stats::Tier::C);
	} break;
	case Key::KeyU:
	case Key::Key4: {
		field.upgradeCannon();
	} break;
	case Key::KeyI: {
		field.printInfoAtSelectedPosition();
	} break;

	case Key::KeyC: {
		cull = (cull + 1) % 4;
		switch (cull) {
		case 0: {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		} break;
		case 1: {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		} break;
		case 2: {
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT_AND_BACK);
		} break;
		case 3: {
			glDisable(GL_CULL_FACE);
		} break;
		}
	}; break;
	case Key::KeyT: {
		polygonMode = (polygonMode + 1) % 2;

		switch (polygonMode) {
		case 0: {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		} break;
		case 1: {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} break;
		}

	} break;
	case Key::KeyV: {
		view += 1;
	} break;
	case Key::KeyM: {
		focusMinimap = !focusMinimap;
	} break;
	case Key::KeyR: {
		view.reset();
		focusMinimap = false;
	} break;
	case Key::KeyX: {
		gameSpeed *= 2;
		if (gameSpeed > maxGameSpeed) {
			gameSpeed = 1;
		}
	} break;
	case Key::KeyL: {
		lighting = (lighting + 1) % 2;
	} break;
	case Key::KeyK: {
		if (selectedView.has_value()) {
			selectedView = std::nullopt;
			break;
		}

		selectedView = field.getSelectedPosition() + Vec3(0, 0, .5);
	} break;
	case Key::KeyJ: {
		selectedEnemyIndex
		    = static_cast<i64>(selectedEnemyIndex.value_or(-1)) + 1;
	} break;
	case Key::KeyQ:
	case Key::KeyE:
	case Key::KeyY:
	case Key::KeyO:
	case Key::KeyP:
	case Key::KeyF:
	case Key::KeyG:
	case Key::KeyH:
	case Key::KeyZ:
	case Key::KeyB:
	case Key::KeyN:
	case Key::Key0:
	case Key::Key5:
	case Key::Key6:
	case Key::Key7:
	case Key::Key8:
	case Key::Key9: {
	} break;
	}
}

void App::KeyRelease(const Key key)
{
	(void) key;
}

void App::MouseScroll(const f64 xoffset, const f64 yoffset)
{
	view.freeViewAzimuthal += xoffset;
	view.freeViewPolar     += yoffset;
}
