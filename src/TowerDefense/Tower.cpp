#include "TowerDefense/Tower.hpp"

#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <cstdint>
#include <openGGL/3D/primitives/unit.hpp>

namespace TowerDefense {

Vec3 Tower::getPosition() const
{
	return position;
}

uint8_t Tower::getHealth() const
{
	return health;
}

uint8_t Tower::damage(const uint8_t damage)
{
	health -= damage;
	return health;
}

} // namespace TowerDefense
