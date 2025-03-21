#include "TowerDefense/Tower.hpp"

#include "Color.hpp"
#include "OBJModel.hpp"
#include "OpenGL/Material.hpp"
#include "Primitives/3D/core.hpp"
#include "Texture.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <ostream>

namespace TowerDefense {

std::ostream &operator<<(std::ostream &os, const Tower &tower)
{
	os << "Tower: " << tower.getGridPosition()
	   << ", health: " << static_cast<int>(tower.getHealth());
	return os;
}

Vec3 Tower::getGridPosition() const
{
	return gridPosition;
}

f64 Tower::getHealthRatio() const
{
	return f64(getHealth()) / getFullHealth();
}

Stats::HealthPoints Tower::getFullHealth() const
{
	return fullHealth;
}

Stats::HealthPoints Tower::getHealth() const
{
	return health;
}

Stats::HealthPoints Tower::damage(const Stats::HealthPoints damage)
{
	health = damage > health ? 0 : health - damage;
	return health;
}

void Tower::draw() const
{
	const auto [posY, posX, _] = gridPosition.getCoordinates();

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glEnable(GL_TEXTURE_2D);
		glColor3ubv(Colors::AQUA.data());
		OpenGL::Material::Apply(OpenGL::Material::CyanPlastic);

		glTranslated(posX, posY, 0);
		glScalef(.9, .9, 1);
#if 1
		Texture::Get("assets/tower.png").Bind();
		Primitives3D::Unit::Cube();
		Texture::UnBind();
#else
		OBJModel::Get("models/monkey.obj").draw();
#endif

		drawHealth();
	}
	glPopAttrib();
	glPopMatrix();
}

void Tower::drawHealth() const
{
	if (getHealth() <= 0) {
		return;
	}

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glColor3ubv(Colors::GREEN.data());
		OpenGL::Material::Apply(OpenGL::Material::Emerald);

		glTranslated(0, 0, .5);
		glScaled(.8 * getHealthRatio(), .3, .3);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

} // namespace TowerDefense
