#include "TowerDefense/Cannon/Base.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Field.hpp"
#include "TowerDefense/Tower.hpp"
#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <openGGL/3D/primitives/unit.hpp>
#include <openGGL/App/core.hpp>

namespace openGGL {

void App::setup() {}
void App::update() {}
void App::draw() {}

} // namespace openGGL

int main()
{
	using TowerDefense::Tower, TowerDefense::Field;
	using TowerDefense::Vec3, TowerDefense::Enemy, TowerDefense::Cannon;

#ifdef DEBUG
	Vec3 v = Vec3::RandomUnitVec3();
	std::cout << "||" << v << "|| = " << v.magnitude() << std::endl;

	Enemy e(.5, 0.1, 10, 10, {0.5, 0.5, 0});
	std::cout << std::endl << e << std::endl;

	Cannon c;
	std::cout << std::endl << c << std::endl;

	Tower t;
	std::cout << std::endl << t << std::endl;

	#define F TowerDefense::Field::Cell::Floor
	#define W TowerDefense::Field::Cell::Wall
	#define S TowerDefense::Field::Cell::Slot
	Field field({
	    {W, W, W, W, W},
	    {6, 5, 4, 3, W},
	    {W, W, S, 2, W},
	    {W, W, W, 1, 0},
	    {W, W, W, W, W},
	    {W, W, W, W, W},
	});
	#undef F
	#undef W
	#undef S

	std::cout << std::endl << field << std::endl;
#endif

	return 0;
}
