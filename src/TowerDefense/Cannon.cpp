#include "TowerDefense/Cannon.hpp"

#include "Color.hpp"
#include "Math.hpp"
#include "OpenGL/Material.hpp"
#include "Primitives/2D/core.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <cmath>
#include <iostream>
#include <optional>
#include <vector>

namespace TowerDefense {

void Cannon::draw(const Vec3 &selectedGridPosition) const
{
	const auto [posY, posX, _] = gridPosition.getCoordinates();

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glColor3ubv(color.data());

		OpenGL::Material::Apply(material);

		glTranslated(posX, posY, 0);
		glRotated(angle, 0, 0, 1);

		glPushMatrix();
		{
			glScalef(.9, .9, 1);
			Primitives3D::Unit::Cube();
		}
		glPopMatrix();

		drawRange(selectedGridPosition);

		glPushMatrix();
		{
			glTranslated(0, 0, .5);
			drawShot();
		}
		glPopMatrix();
	}
	glPopAttrib();
	glPopMatrix();
}

void Cannon::update(const Stats::TimeMs deltaTimeMs,
                    const std::vector<Enemy> &enemies)
{
	cooldownMs = (deltaTimeMs > cooldownMs) ? 0 : cooldownMs - deltaTimeMs;

	const std::optional<Enemy *> enemy = targetEnemy(enemies);
	if (!enemy) {
		return;
	}

	Enemy &target = *enemy.value();
	updateAngle(target);
	shot(target);
}

void Cannon::shot(Enemy &target)
{
	if (cooldownMs > 0) {
		return;
	}

	cooldownMs = defaultCooldownMs;

	target.loseHP(shotDamage);

#ifdef DEBUG
	std::cout << "Cannon shooting at " << target << std::endl;
#endif
}

bool Cannon::upgrade()
{
	switch (currentLevel) {
	case Stats::Level::L1: {
		currentLevel = Stats::Level::L2;
	} break;
	case Stats::Level::L2: {
		currentLevel = Stats::Level::L3;
	} break;
	case Stats::Level::L3: {
		return false;
	} break;
	}

	updateStats();

	std::cout << "Upgrade Cannon: " << getGridPosition() << std::endl;

	return true;
}

Vec3 Cannon::getGridPosition() const
{
	return gridPosition;
}

void Cannon::updateStats()
{
	using Stats::Cannon::Get;

	const Stats::CannonStats cannonStats = Get(currentTier, currentLevel);

	shotDamage        = cannonStats.damage;
	range             = cannonStats.range;
	defaultCooldownMs = cannonStats.cooldownMs;
	color             = cannonStats.color;

	switch (currentTier) {
	case Stats::Tier::A: {
		material = OpenGL::Material::Red;
	} break;
	case Stats::Tier::B: {
		material = OpenGL::Material::Orange;
	} break;
	case Stats::Tier::C: {
		material = OpenGL::Material::Purple;
	} break;
	}
}

void Cannon::updateAngle(const Enemy &target)
{
	const Vec3 delta = target.getPathInfo().pos - gridPosition;
	const auto &[deltaY, deltaX, _] = delta.getCoordinates();

	angle = Math::radiansToDegrees(std::atan2(deltaY, deltaX));
}

std::optional<Enemy *>
Cannon::targetEnemy(const std::vector<Enemy> &enemies) const
{
	Enemy *target = nullptr;

	const f64 rangeSq = range * range;
	for (const Enemy &enemy : enemies) {
		if (enemy.getPosition() < 0) {
			continue;
		}

		const Vec3 enemyPos  = enemy.getPathInfo().pos;
		const f64 distanceSq = (enemyPos - gridPosition).magnitudeSq();

		if (distanceSq >= rangeSq) {
			continue;
		}

		if (target == nullptr) {
			target = const_cast<Enemy *>(&enemy);
			continue;
		}

		if (target->getPosition() > enemy.getPosition()) {
			continue;
		}

		target = const_cast<Enemy *>(&enemy);
	}

	return target == nullptr ? std::nullopt : std::make_optional(target);
}

void Cannon::drawRange(const Vec3 &selectedGridPosition) const
{
	if (selectedGridPosition != gridPosition) {
		return;
	}

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glColor3ubv(color.data());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5);

		glScalef(2, 2, 1);
		glScaled(range, range, 1);
		Primitives2D::Unit::Circle();
	}
	glPopAttrib();
	glPopMatrix();
}

void Cannon::drawShot() const
{
	static constexpr f64 maxRatio = .5;

	const f64 ratio = (f64) cooldownMs / defaultCooldownMs;
	if (ratio < maxRatio) {
		return;
	}

	Color c = Colors::SALMON;
	c[3]    = u8(0xFF * (1 - (1 - ratio) / (1 - maxRatio)));

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glColor4ubv(c.data());

		glScaled(range * .25, .2, .2);
		glTranslated(1, 0, 0);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

} // namespace TowerDefense
