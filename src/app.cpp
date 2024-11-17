#include "TowerDefense/Field.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
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

	std::cout << field << std::endl;

	glEnable(GL_DEPTH_TEST);
}

void update()
{
	glfwGetWindowSize(window, &width, &height);

	field.update();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static constexpr int glMask = GL_VIEWPORT_BIT;


	glPushAttrib(glMask);
	{
		const GLint p1 = width / 8;
		const GLint p2 = height / 8;
		glViewport(p1, p2, 6 * p1, 6 * p2);
		field.draw();
	}
	glPopAttrib();

	glPushAttrib(glMask);
	{
		const GLint p1 = width / 8;
		const GLint p2 = height / 8;
		glViewport(0, 7 * p2, 4 * p1, p2);
	}
	glPopAttrib();
}
