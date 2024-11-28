#include "App.hpp"

#include "Math.hpp"
#include "OpenGL/camera.hpp"
#include "TowerDefense/Enemy.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <array>
#include <cmath>
#include <optional>

struct Rect {
	int x;
	int y;
	int w;
	int h;
};

void App::drawMinimap()
{
	const Rect viewport = focusMinimap
			? (Rect){
				.x = static_cast<int>(width * 1/6),
				.y = static_cast<int>(height * 1/6),
				.w = static_cast<int>(width * 4/6),
				.h = static_cast<int>(height * 4/6),
			}
			: (Rect){
				.x = static_cast<int>(width * 5/6),
				.y = static_cast<int>(height * 5/6),
				.w = static_cast<int>(width * 1/6),
				.h = static_cast<int>(height * 1/6),
			}
			;

	const auto [rows, cols] = field.getMapDimensions();

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

		glPushMatrix();
		{
			glViewport(0, 0, width, height);

			OpenGL::Perspective(30, 45, 45);

			const auto &[rows, cols] = field.getMapDimensions();

			if (!selectedEnemyIndex.has_value()) {
				view.lookAt(selectedView,
				            {.5 * f64(cols), .5 * f64(rows)},
				            f64(cols + rows) * .5,
				            orbitAngle,
				            Math::PI / 6);
			} else {
				const std::optional<TowerDefense::Enemy> enemy
				    = field.getEnemy(
				        selectedEnemyIndex.value());

				if (!enemy.has_value()
				    || enemy->getPosition() < 0) {
					selectedEnemyIndex = std::nullopt;
					selectedView       = std::nullopt;
					view.reset();
				} else {
					selectedView = enemy->getPathInfo().pos;
					view.lookAt(
					    selectedView,
					    {.5 * f64(cols), .5 * f64(rows)},
					    f64(cols + rows) * .5,
					    orbitAngle,
					    Math::PI / 6);
				}
			}

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
	glEnable(GL_DEPTH_TEST);
}

void App::update()
{
	updateTime();

	orbitAngle += .01;
	while (orbitAngle >= 2 * Math::PI) {
		orbitAngle -= 2 * Math::PI;
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
