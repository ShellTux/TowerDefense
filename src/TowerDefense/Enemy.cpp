#include "TowerDefense/Enemy.hpp"

#include "Color.hpp"
#include "OpenGL/Material.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <tuple>
#include <vector>

namespace TowerDefense {

f64 Enemy::getHealthRatio() const
{
	return f64(getHealth()) / getPoints();
}

f64 Enemy::getPosition() const
{
	return position;
}

Stats::Speed Enemy::getSpeedUpMs() const
{
	return speedUpMs;
}

Stats::HealthPoints Enemy::getHealth() const
{
	return health;
}

Stats::HealthPoints Enemy::getPoints() const
{
	return fullHealth;
}

Enemy Enemy::Random(const std::vector<Vec3> &path, const f64 position)
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
	if (getPosition() < 0) {
		return;
	}

	const auto [posY, posX, _] = getGridPosition().getCoordinates();

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glColor3ubv(color.data());
		OpenGL::Material::Apply(material);

		glTranslated(posX, posY, 0);
		glScalef(.9, .9, 1);
		Primitives3D::Unit::Cube();

		drawHealth();
	}
	glPopAttrib();
	glPopMatrix();
}

void Enemy::drawHealth() const
{
	if (getHealth() <= 0) {
		return;
	}

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glColor3ubv(Colors::GREEN.data());

		OpenGL::Material::Apply(OpenGL::Material::Material::Emerald);

		glTranslated(0, 0, .5);
		glScaled(.8 * getHealthRatio(), .2, .2);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

void Enemy::update(const Stats::TimeMs deltaTimeMs)
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

	using std::clamp, std::min;

	const f64 clampedPosition = clamp(position, 0.0, 1.0);

	const usize totalPoints     = gridPath.size();
	const f64 distanceAlongPath = clampedPosition * f64(totalPoints - 1);

	const usize startIdx = static_cast<usize>(distanceAlongPath);
	const usize endIdx   = min(startIdx + 1, totalPoints - 1);

	const Vec3 &startPoint = gridPath.at(startIdx);
	const Vec3 &endPoint   = gridPath.at(endIdx);

	const f64 segmentProgress
	    = getPosition() < 0
	          ? (getPosition() * f64(totalPoints - 1)) - f64(startIdx)
	          : distanceAlongPath - f64(startIdx);

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

	using OpenGL::Material;
	switch (tier) {
	case Stats::Tier::A: {
		material = Material::Type::Ruby;
	} break;
	case Stats::Tier::B: {
		material = Material::Type::Bronze;
	} break;
	case Stats::Tier::C: {
		material = Material::Type::Jade;
	} break;
	}
}

std::tuple<Vec3, Vec3, Vec3> Enemy::getLookAt() const
{
	const static f64 z = .5;

	const Vec3 pos     = getGridPosition();
	const Vec3 nextPos = getNextGridPosition();
	const Vec3 dir     = (nextPos - pos).normalize();

	static constexpr f64 r            = .5;
	const auto &[cameraI, cameraJ, _] = (pos + dir * r).getCoordinates();
	const auto &[targetI, targetJ, _1]
	    = (pos + dir * (r + 1)).getCoordinates();

	const Vec3 camera    = Vec3(cameraJ, cameraI, z);
	const Vec3 target    = Vec3(targetJ, targetI, z);
	static const Vec3 up = {0, 0, -1};

	return {camera, target, up};
}

} // namespace TowerDefense
