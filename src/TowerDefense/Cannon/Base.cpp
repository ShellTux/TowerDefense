#include "TowerDefense/Cannon/Base.hpp"

#include "Math.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <cmath>
#include <limits>
#include <openGGL/3D/primitives/unit.hpp>
#include <optional>
#include <vector>

#ifdef DEBUG
	#include <iostream>
	#include <cstdint>
#endif

namespace TowerDefense {

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
	std::cout << "Cannon shooting at " << enemy << std::endl;
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
	const auto [deltaX, deltaY, _]
	    = (target.getRealPosition() - position).getCoordinates();

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
		    = (enemy.getRealPosition() - position).magnitudeSq();
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
