#include "App.hpp"

#include "OpenGL/camera.hpp"

#include <GL/gl.h>
#include <cmath>
#include <iostream>

static constexpr int glMask = GL_VIEWPORT_BIT | GL_TRANSFORM_BIT;

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
		glOrtho(-.7, cols - .2, rows - .2, -.7, -1000, 1000);

		field.draw();
	}
	glPopAttrib();
}

void App::drawField()
{
	glPushAttrib(glMask);
	{
		glPushMatrix();
		{
			const GLint p1 = width / 8;
			const GLint p2 = height / 8;
			glViewport(p1, p2, 6 * p1, 6 * p2);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			OpenGL::Perspective(30, 45, 45);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			const auto [rows, cols] = field.getMapDimensions();

			Vec3 camera;
			switch (view) {
			case 0: {
				camera = {cols / 2., 1.5 * rows, 5};
			} break;
			case 1: {
				using std::cos, std::sin;
				camera = {cols / 2. * (1 + cos(orbitAngle)),
				          rows / 2. * (1 + sin(-orbitAngle)),
				          5};
			} break;
			}

			static const Vec3 target{cols / 2., rows / 2., 0};

			OpenGL::Camera::LookAt(camera, target, {0, 1, 0});
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
		const GLint p1 = width / 8;
		const GLint p2 = height / 8;
		glViewport(0, 7 * p2, 5 * p1, p2);
		field.drawHUD();
	}
	glPopAttrib();
}

void App::setup()
{
#ifdef DEBUG
	std::cout << field << std::endl;
#endif

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}

void App::update()
{
	orbitAngle += .01;
	updateTime();

	field.update(deltaTimeMs);
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
}
