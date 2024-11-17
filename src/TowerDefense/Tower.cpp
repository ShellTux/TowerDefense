#include "TowerDefense/Tower.hpp"

#include "Color.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <array>
#include <cstdint>

namespace TowerDefense {

Vec3 Tower::getGridPosition() const
{
	return gridPosition;
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

void Tower::draw() const
{
	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	const auto [posY, posX, _] = gridPosition.getCoordinates();

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(Colors::BLUE.data());

		glTranslated(posX, posY, 0);
		glScalef(.9, .9, 2);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

} // namespace TowerDefense
