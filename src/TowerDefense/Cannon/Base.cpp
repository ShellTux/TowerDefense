#include "TowerDefense/Cannon/Base.hpp"

#include "Color.hpp"
#include "Math.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "Vec3.hpp"

#include <GL/gl.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <openGGL/3D/primitives/unit.hpp>
#include <optional>
#include <vector>

#ifdef DEBUG
	#include <iostream>
#endif

namespace TowerDefense {

void Cannon::draw() const
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

		glTranslated(posX, posY, 0);
		glScalef(.9, .9, 2);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

void Cannon::update(const std::vector<Enemy> &enemies)
{
	cooldown = cooldown > 0 ? cooldown - 1 : defaultCooldown;

	const std::optional<Enemy *> enemy = targetEnemy(enemies);
	if (!enemy) {
		return;
	}

	Enemy &target = *enemy.value();
	updateAngle(target);
	if (cooldown <= 0) {
		shot(target);
	}
}

void Cannon::shot(Enemy &target) const
{
	target.loseHP(shotDamage);

#ifdef DEBUG
	std::cout << "Cannon shooting at " << target << std::endl;
#endif
}

void Cannon::upgrade()
{
	using std::max;

	range           += 1;
	shotDamage      += 1;
	defaultCooldown  = max(1, defaultCooldown - 1);

#ifdef DEBUG
	std::cout << "Cannon upgraded! " << *this << std::endl;
#endif
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

} // namespace TowerDefense
