#include "OpenGL/camera.hpp"
#include "TowerDefense/Field.hpp"

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define F TowerDefense::Field::Cell::CFloor
#define W TowerDefense::Field::Cell::CWall
#define S TowerDefense::Field::Cell::CSlot

using TowerDefense::Field;
static Field field = TowerDefense::Field::FromFile("assets/example.map")
                         .value_or(Field({
                             {W, W, W, W, W},
                             {6, 5, 4, 3, W},
                             {W, W, S, 2, W},
                             {W, W, W, 1, 0},
                             {W, W, W, W, W},
                             {W, W, W, W, W},
}));
#undef F
#undef W
#undef S

void errorCallback(const int errorCode, const char *description) {}
void framebufferCallback(GLFWwindow *window, const int width, const int height)
{}

void setup()
{
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
	field.update();
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glPushAttrib(0);
	{
		/*OpenGL::Camera::LookAt({0, -.5, .5}, {0, 0, 0}, {0, 1, 0});*/
		field.draw();
	}
	glPopAttrib();
}
