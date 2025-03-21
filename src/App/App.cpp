#include "App.hpp"

#include "Math.hpp"
#include "OpenGL/Camera.hpp"
#include "OpenGL/Cull.hpp"
#include "TowerDefense/Enemy.hpp"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <array>
#include <cmath>
#include <iostream>
#include <optional>

void App::drawMinimap()
{
	struct Rect {
		int x;
		int y;
		int w;
		int h;
	};

	Rect rect1{};
	rect1.x = static_cast<int>(width * 1 / 6);
	rect1.y = static_cast<int>(height * 1 / 6);
	rect1.w = static_cast<int>(width * 4 / 6);
	rect1.h = static_cast<int>(height * 4 / 6);

	Rect rect2{};
	rect2.x = static_cast<int>(width * 5 / 6);
	rect2.y = static_cast<int>(height * 5 / 6);
	rect2.w = static_cast<int>(width * 1 / 6);
	rect2.h = static_cast<int>(height * 1 / 6);

	const Rect viewport = focusMinimap ? rect1 : rect2;

	const auto &[rows, cols] = field.getMapDimensions();

	glPushAttrib(glMask);
	{
		glViewport(viewport.x, viewport.y, viewport.w, viewport.h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-.7, f64(cols) - .2, f64(rows) - .2, -.7, -1000, 1000);

		field.draw();
	}
	glPopAttrib();
}

void App::drawField()
{
	static constexpr std::array<f32, 4> lightDirection = {0, 0, 1, 0};
	static constexpr std::array<f32, 4> ambientColor   = {.1, .1, .1, 1};
	static constexpr std::array<f32, 4> diffuseColor   = {1, 1, 1, 1};

	glPushAttrib(glMask);
	{
		switch (lighting) {
		case 0:
			break;
		case 1: {
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0,
			          GL_POSITION,
			          lightDirection.data());

			glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor.data());

			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor.data());
		} break;
		}

		glViewport(0, 0, i32(width), i32(height));

		glPushMatrix();
		{
			if (selectedEnemyI.has_value()) {
				using TowerDefense::Enemy;

				const usize enemyI = selectedEnemyI.value();

				const std::optional<Enemy> &enemy
				    = field.getEnemy(enemyI);

				if (enemy.has_value()
				    && enemy->getPosition() > 0) {
					camera.setTarget(
					    enemy->getPathInfo()
					        .pos.transpose2D());
				} else {
					selectedEnemyI = std::nullopt;
					selectedView   = std::nullopt;
					camera.reset();
				}
			}

			camera.apply();

			field.draw();
		}
		glPopMatrix();
	}
	glPopAttrib();
}

void App::drawHUD()
{
	glPushAttrib(glMask);
	{
		const i32 p1 = i32(width) / 8;
		const i32 p2 = i32(height) / 8;
		glViewport(0, 7 * p2, 5 * p1, p2);
		field.drawHUD();
	}
	glPopAttrib();
}

void App::setup()
{
	std::cout << "\033[2J\033[H";

	// HACK: Need to figure why culling is f*cked up
	cull.setCullingFace(OpenGL::Cull::Disabled);

	camera.perspective(30, 45, 45);
	cull.apply();

	glEnable(GL_DEPTH_TEST);
}

void App::update()
{
	updateTime();

	orbitAngle += .01;
	while (orbitAngle >= 2 * Math::PId) {
		orbitAngle -= 2 * Math::PId;
	}
	while (orbitAngle < 0) {
		orbitAngle += 2 * Math::PId;
	}

	for (u8 i = 0; i < gameSpeed * static_cast<u8>(!pause); ++i) {
		field.update(deltaTimeMs);
	}
}

void App::updateTime()
{
	deltaTimeMs   = (Clock::now() - lastFrameTime).count() / (long) 1e6;
	lastFrameTime = Clock::now();
}

void App::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawField();
	drawMinimap();
	drawHUD();

	printStats();
}

void App::updateDimensions(const u32 width, const u32 height)
{
	this->width  = width;
	this->height = height;

	camera.setWidthHeight(width, height);
	camera.perspective(30, 45, 45);
}
