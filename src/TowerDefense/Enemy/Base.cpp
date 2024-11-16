#include "TowerDefense/Enemy/Base.hpp"

#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <cstdint>
#include <openGGL/3D/primitives/unit.hpp>
#include <vector>

namespace TowerDefense {

double Enemy::getPosition() const
{
	return position;
}

double Enemy::getSpeed() const
{
	return speed;
}

uint8_t Enemy::getHealth() const
{
	return health;
}

uint8_t Enemy::getPoints() const
{
	return points;
}

Vec3 Enemy::getRealPosition() const
{
	return realPosition;
}

void Enemy::update(const std::vector<Vec3> &enemyPath)
{
	updatePosition(enemyPath);
}

uint8_t Enemy::loseHP(const uint8_t healthPoints)
{
	health = (health > healthPoints) ? health - healthPoints : 0;
	return health;
}

Vec3 Enemy::map2path(const std::vector<Vec3> &enemyPath) const
{
	if (enemyPath.empty()) {
		return {0, 0, 0};
	}

	const auto constrain = [](auto var, auto minimum, auto maximum) {
		using std::min, std::max;

		return min(max(var, minimum), maximum);
	};

	const double total = static_cast<double>(enemyPath.size() - 1);
	const int index    = constrain(static_cast<int>(position * total),
                                    0,
                                    (int) enemyPath.size() - 1);

	const Vec3 &currentPoint = enemyPath[index];
	const Vec3 &nextPoint    = enemyPath[index + 1];

	const double t = position * total - index;

	return currentPoint + (nextPoint - currentPoint) * t;
}

void Enemy::updatePosition(const std::vector<Vec3> &enemyPath)
{
	realPosition = map2path(enemyPath);
}

} // namespace TowerDefense
