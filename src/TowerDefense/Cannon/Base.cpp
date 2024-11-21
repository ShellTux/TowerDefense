#include "TowerDefense/Cannon/Base.hpp"

#include "Color.hpp"
#include "Math.hpp"
#include "Primitives/2D/core.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"

#include <GL/gl.h>
#include <cmath>
#include <limits>
#include <optional>
#include <vector>

#ifndef RELEASE
	#include <iostream>
#endif

extern TowerDefense::Stats::CooldownMs deltaTimeMs;

namespace TowerDefense {

void Cannon::draw(const Vec3 &selectedGridPosition) const
{
	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	const auto [posY, posX, _] = gridPosition.getCoordinates();

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(color.data());

		glTranslated(posX, posY, .5);
		glRotated(angle, 0, 0, 1);

		glPushMatrix();
		{
			glScalef(.9, .9, 2);
			Primitives3D::Unit::Cube();
		}
		glPopMatrix();

		drawRange(selectedGridPosition);
		drawShot();
	}
	glPopAttrib();
	glPopMatrix();
}

void Cannon::update(const std::vector<Enemy> &enemies)
{
	cooldownMs = (deltaTimeMs > cooldownMs) ? 0 : cooldownMs - deltaTimeMs;
	angle      = 0;

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

#ifndef RELEASE
	std::cout << "Cannon shooting at " << target << std::endl;
#endif
}

void Cannon::upgrade()
{
	switch (currentLevel) {
	case Stats::Level::L1: {
		currentLevel = Stats::Level::L2;
	} break;
	case Stats::Level::L2: {
		currentLevel = Stats::Level::L3;
	} break;
	case Stats::Level::L3: {
		return;
	} break;
	}

	updateStats();

#ifndef RELEASE
	std::cout << "Cannon upgraded: " << *this << std::endl;
#endif
}

void Cannon::updateStats()
{
	using Stats::Cannon::Get;

	const Stats::CannonStats cannonStats = Get(currentTier, currentLevel);

	shotDamage        = cannonStats.damage;
	range             = cannonStats.range;
	defaultCooldownMs = cannonStats.cooldownMs;
	color             = cannonStats.color;
}

void Cannon::updateAngle(const Enemy &target)
{
	const auto [deltaY, deltaX, _]
	    = (target.getGridPosition() - gridPosition).getCoordinates();

	angle = Math::radiansToDegrees(std::atan2(deltaY, deltaX));
}

[[nodiscard]] std::optional<Enemy *>
Cannon::targetEnemy(const std::vector<Enemy> &enemies) const
{
	double closestDistanceSq = std::numeric_limits<double>::max();
	Enemy *closestEnemy      = nullptr;

	const double rangeSq = range * range;
	for (const auto &enemy : enemies) {
		const double distanceSq
		    = (enemy.getGridPosition() - gridPosition).magnitudeSq();
		if (distanceSq >= rangeSq || distanceSq >= closestDistanceSq) {
			continue;
		}

		closestDistanceSq = distanceSq;
		closestEnemy      = const_cast<Enemy *>(&enemy);
	}

	if (closestEnemy == nullptr) {
		return std::nullopt;
	}

	return closestEnemy;
}

void Cannon::drawRange(const Vec3 &selectedGridPosition) const
{
	if (selectedGridPosition != gridPosition) {
		return;
	}

	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(color.data());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5);

		glScalef(2, 2, 1);
		glScaled(range, range, 1);
		Primitives2D::Unit::Circle(36, false);
	}
	glPopAttrib();
	glPopMatrix();
}

void Cannon::drawShot() const
{
	const double ratio = (double) cooldownMs / defaultCooldownMs;
	if (ratio < .92) {
		return;
	}


	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(Colors::SALMON.data());

		glScaled(range * .25, 1, 1);
		glTranslated(1, 0, 0);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

} // namespace TowerDefense
