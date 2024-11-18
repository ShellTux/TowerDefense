#include "Math.hpp"
#include "OpenGL/camera.hpp"
#include "TowerDefense/Field.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdint>
#include <iostream>

#define F TowerDefense::Field::Cell::CFloor
#define W TowerDefense::Field::Cell::CWall
#define S TowerDefense::Field::Cell::CSlot

using TowerDefense::Field;
Field field = TowerDefense::Field::FromFile("assets/example.map",
                                            {
                                                6, 11
})
                  .value_or(Field(
                      {
                          {W, W, W, W, W},
                          {6, 5, 4, 3, W},
                          {W, W, S, 2, W},
                          {W, W, W, 1, 0},
                          {W, W, W, W, W},
                          {W, W, W, W, W},
                      },
                      {3, 5}));
#undef F
#undef W
#undef S

static int width;
static int height;
static int rows;
static int cols;
static GLfloat orbitAngle = 0;

extern GLFWwindow *window;

void errorCallback(const int errorCode, const char *description)
{
	std::cerr << "Error(" << errorCode << "): " << description << std::endl;
}

void framebufferCallback(GLFWwindow *window, const int width, const int height)
{
	(void) window;
	(void) width;
	(void) height;
}

void setup()
{
	glfwGetWindowSize(window, &width, &height);

	field.setDrawFloor(true);
	field.setDrawEnemyPath(true);
	field.setDrawTower(true);
	field.setDrawCannons(true);
	field.setDrawEnemies(true);

	const auto [rowsT, colsT] = field.getMapDimensions();
	rows                      = rowsT;
	cols                      = colsT;

	std::cout << field << std::endl;

	glEnable(GL_DEPTH_TEST);
}

void update()
{
	glfwGetWindowSize(window, &width, &height);

	orbitAngle += .01;

	field.update();
}

uint8_t view      = 0;
bool focusMinimap = false;

namespace Draw {

static constexpr int glMask = GL_VIEWPORT_BIT | GL_TRANSFORM_BIT;

static void Minimap()
{
	glPushAttrib(glMask);
	{
		using Rect = struct {
			int x;
			int y;
			int w;
			int h;
		};

		const Rect viewport = focusMinimap
			? (Rect){
				.x = width * 1/6,
				.y = height * 1/6,
				.w = width * 4/6,
				.h = height * 4/6,
			}
			: (Rect){
				.x = width * 5/6,
				.y = height * 5/6,
				.w = width * 1/6,
				.h = height * 1/6,
			}
			;


		glViewport(viewport.x, viewport.y, viewport.w, viewport.h);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-.7, cols - .2, rows - .2, -.7, -1000, 1000);

		field.draw();
	}
	glPopAttrib();
}

static void Field()
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

			Vec3 camera;
			switch (view) {
			case 0: {
				camera = {double(cols / 2.),
				          double(1.5 * rows),
				          5};
			} break;
			case 1: {
				using std::cos, std::sin;
				camera = {cols / 2. * (1 + cos(orbitAngle)),
				          rows / 2. * (1 + sin(-orbitAngle)),
				          5};
			} break;
			}

			static const Vec3 target{double(cols / 2.),
			                         double(rows / 2.),
			                         0};

			OpenGL::Camera::LookAt(camera, target, {0, 1, 0});
			field.draw();
		}
		glPopMatrix();
	}
	glPopAttrib();
}

static void HUD()
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

} // namespace Draw

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Draw::Field();
	Draw::Minimap();
	Draw::HUD();
}
