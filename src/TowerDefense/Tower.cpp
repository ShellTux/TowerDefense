#include "TowerDefense/Tower.hpp"

#include "Color.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"

#include <GL/gl.h>

namespace TowerDefense {

Vec3 Tower::getGridPosition() const
{
	return gridPosition;
}

Stats::HealthPoints Tower::getHealth() const
{
	return health;
}

Stats::HealthPoints Tower::damage(const Stats::HealthPoints damage)
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
		glColor3ubv(Colors::AQUA.data());

		glTranslated(posX, posY, 0);
		glScalef(.9, .9, 1);
		Primitives3D::Unit::Cube();

		drawHealth();
	}
	glPopAttrib();
	glPopMatrix();
}

void Tower::drawHealth() const
{
	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(Colors::GREEN.data());

		glTranslated(0, 0, .5);
		glScalef(.8, .3, .3);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

} // namespace TowerDefense
