#include "App.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

void App::KeyPress(const int key)
{
	using TowerDefense::Vec3;

	switch (key) {
	case GLFW_KEY_S:
	case GLFW_KEY_DOWN: {
		field.moveSelectedPosition(Vec3(1, 0));
	} break;
	case GLFW_KEY_W:
	case GLFW_KEY_UP: {
		field.moveSelectedPosition(Vec3(-1, 0));
	} break;
	case GLFW_KEY_A:
	case GLFW_KEY_LEFT: {
		field.moveSelectedPosition(Vec3(0, -1));
	} break;
	case GLFW_KEY_D:
	case GLFW_KEY_RIGHT: {
		field.moveSelectedPosition(Vec3(0, 1));
	} break;

	case GLFW_KEY_1: {
		field.placeCannon(TowerDefense::Stats::Tier::A);
	} break;
	case GLFW_KEY_2: {
		field.placeCannon(TowerDefense::Stats::Tier::B);
	} break;
	case GLFW_KEY_3: {
		field.placeCannon(TowerDefense::Stats::Tier::C);
	} break;
	case GLFW_KEY_4: {
		field.upgradeCannon();
	} break;

	case GLFW_KEY_C: {
		cull = (cull + 1) % 4;

#ifdef DEBUG
		std::cout << "Cull: " << cull << std::endl;
#endif
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
	case GLFW_KEY_P: {
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
	case GLFW_KEY_V: {
		view = (view + 1) % 2;
	} break;
	case GLFW_KEY_M: {
		focusMinimap = !focusMinimap;
	} break;
	case GLFW_KEY_R: {
		view         = 0;
		focusMinimap = false;
	} break;
	}
}

void App::KeyRelease(const int key)
{
	(void) key;
}