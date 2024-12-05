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
	switch (key) {
	case Key::KeyNone:
		return;

	case Key::KeyP: {
		pause = !pause;

		if (!pause) {
			lastFrameTime = Clock::now();
		}
	} break;

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
		cull += 1;
		cull.apply();
	}; break;
	case Key::KeyT: {
		polygonMode += 1;
		polygonMode.apply();
	} break;
	case Key::KeyV: {
		camera += 1;
	} break;
	case Key::KeyM: {
		focusMinimap = !focusMinimap;
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
	case Key::KeyR: {
		selectedView   = std::nullopt;
		selectedEnemyI = std::nullopt;
		camera.reset();
	} break;
	case Key::KeyK: {
		if (selectedView.has_value()) {
			selectedView = std::nullopt;
			camera.setTarget(selectedView->transpose2D());
			camera.setRadius(2);
		} else {
			selectedView
			    = field.getSelectedPosition() + Vec3(0, 0, .5);
			camera.setTarget();
			camera.setRadius();
		}
	} break;
	case Key::KeyJ: {
		selectedEnemyI
		    = static_cast<i64>(selectedEnemyI.value_or(-1)) + 1;
	} break;
	case Key::KeyKPSubtract: {
		camera.zoomIn(-1);
	} break;
	case Key::KeyKPAdd: {
		camera.zoomIn(1);
	} break;

	case Key::KeyQ:
	case Key::KeyE:
	case Key::KeyY:
	case Key::KeyO:
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
	case Key::Key9:
	case Key::KeySpace:
	case Key::KeyApostrophe:
	case Key::KeyComma:
	case Key::KeyPeriod:
	case Key::KeySemicolon:
	case Key::KeyEqual:
	case Key::KeyLeftBracket:
	case Key::KeyBackslash:
	case Key::KeyRightBracket:
	case Key::KeyEscape:
	case Key::KeyEnter:
	case Key::KeyTab:
	case Key::KeyBackspace:
	case Key::KeyInsert:
	case Key::KeyDelete:
	case Key::KeyPageUp:
	case Key::KeyPageDown:
	case Key::KeyHome:
	case Key::KeyEnd:
	case Key::KeyCapsLock:
	case Key::KeyScrollLock:
	case Key::KeyNumLock:
	case Key::KeyPrintScreen:
	case Key::KeyPause:
	case Key::KeyF1:
	case Key::KeyF2:
	case Key::KeyF3:
	case Key::KeyF4:
	case Key::KeyF5:
	case Key::KeyF6:
	case Key::KeyF7:
	case Key::KeyF8:
	case Key::KeyF9:
	case Key::KeyF10:
	case Key::KeyF11:
	case Key::KeyF12:
	case Key::KeyF13:
	case Key::KeyF14:
	case Key::KeyF15:
	case Key::KeyF16:
	case Key::KeyF17:
	case Key::KeyF18:
	case Key::KeyF19:
	case Key::KeyF20:
	case Key::KeyF21:
	case Key::KeyF22:
	case Key::KeyF23:
	case Key::KeyF24:
	case Key::KeyF25:
	case Key::KeyKPDecimal:
	case Key::KeyKPDivide:
	case Key::KeyKPMultiply:
	case Key::KeyKPEnter:
	case Key::KeyKPEqual:
	case Key::KeyLeftShift:
	case Key::KeyLeftControl:
	case Key::KeyLeftAlt:
	case Key::KeyRightShift:
	case Key::KeyRightControl:
	case Key::KeyRightAlt:
	case Key::KeyRightSuper:
	case Key::KeyGraveAccent:
	case Key::KeyKP0:
	case Key::KeyKP1:
	case Key::KeyKP2:
	case Key::KeyKP3:
	case Key::KeyKP4:
	case Key::KeyKP5:
	case Key::KeyKP6:
	case Key::KeyKP7:
	case Key::KeyKP8:
	case Key::KeyKP9:
	case Key::KeyLEFT_SUPER:
	case Key::KeyMenu:
		break;
	}
}

void App::KeyRelease(const Key key)
{
	(void) key;
}

void App::MouseScroll(const f64 xoffset, const f64 yoffset)
{
	camera.orbit(xoffset, yoffset);
}
