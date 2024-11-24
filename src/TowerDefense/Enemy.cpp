#include "TowerDefense/Enemy.hpp"

#include "Color.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <vector>

namespace TowerDefense {

double Enemy::getPosition() const
{
	return position;
}

double Enemy::getSpeedUpMs() const
{
	return speedUpMs;
}

uint8_t Enemy::getHealth() const
{
	return health;
}

uint8_t Enemy::getPoints() const
{
	return fullHealth;
}

Enemy Enemy::Random(const std::vector<Vec3> &path, const double position)
{
	switch (std::rand() % 3 + 1) {
	default:
	case 1: {
		return {Stats::Tier::A, path, position};
	} break;
	case 2: {
		return {Stats::Tier::B, path, position};
	} break;
	case 3: {
		return {Stats::Tier::C, path, position};
	} break;
	}
}

void Enemy::draw() const
{
	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	const auto [posY, posX, _] = getGridPosition().getCoordinates();

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(color.data());

		glTranslated(posX, posY, 0);
		glScalef(.9, .9, 2);
		Primitives3D::Unit::Cube();

		drawHealth();
	}
	glPopAttrib();
	glPopMatrix();
}

void Enemy::drawHealth() const
{
	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	const double healthRatio = 1. * health / fullHealth;

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(Colors::GREEN.data());

		glTranslated(0, 0, .5);
		glScaled(.8 * healthRatio, .2, .2);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

void Enemy::update(const Stats::CooldownMs deltaTimeMs)
{
	position += speedUpMs * f32(deltaTimeMs) * 2;
}

Stats::HealthPoints Enemy::loseHP(const Stats::HealthPoints damagePoints)
{
	health = (health > damagePoints) ? health - damagePoints : 0;
	return health;
}

std::tuple<Vec3, Vec3, f64> Enemy::getInterpolatingGridPositions() const
{
	if (gridPath.empty()) {
		return {};
	}

	const double clampedPosition = std::clamp(position, 0.0, 1.0);

	const size_t totalPoints       = gridPath.size();
	const double distanceAlongPath = clampedPosition * (totalPoints - 1);

	const size_t startIdx = static_cast<size_t>(distanceAlongPath);
	const size_t endIdx   = std::min(startIdx + 1, totalPoints - 1);

	const Vec3 &startPoint = gridPath.at(startIdx);
	const Vec3 &endPoint   = gridPath.at(endIdx);


	const double segmentProgress = distanceAlongPath - startIdx;

	return {startPoint, endPoint, segmentProgress};
}

Vec3 Enemy::getGridPosition() const
{
	if (gridPath.empty()) {
		return {};
	}

	const auto &[startPoint, endPoint, ratio]
	    = getInterpolatingGridPositions();

	return startPoint * (1 - ratio) + endPoint * ratio;
}

Vec3 Enemy::getNextGridPosition() const
{
	if (gridPath.empty()) {
		return {};
	}

	const auto &[_, endPoint, _1] = getInterpolatingGridPositions();

	return endPoint;
}

void Enemy::updateStats(const Stats::Tier &tier)
{
	const Stats::EnemyStats enemyStats = Stats::Enemy::Get(tier);

	speedUpMs = enemyStats.speedUpMs;
	health = fullHealth = enemyStats.health;
	color               = enemyStats.color;
}

std::tuple<Vec3, Vec3, Vec3> Enemy::getLookAt() const
{
	// TODO: NOT working
	const static f64 z = .2;

	const Vec3 pos     = getGridPosition();
	const Vec3 nextPos = getNextGridPosition();

	const auto &[cameraI, cameraJ, _]  = pos.getCoordinates();
	const auto &[targetI, targetJ, _1] = nextPos.getCoordinates();

	const Vec3 camera    = Vec3(cameraJ + 1, cameraI, z);
	const Vec3 target    = Vec3(targetJ, targetI, z);
	static const Vec3 up = {0, 0, -1};

	return {camera, target, up};
}

} // namespace TowerDefense
