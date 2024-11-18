#include "TowerDefense/Enemy/Base.hpp"

#include "Color.hpp"
#include "Primitives/3D/core.hpp"
#include "Vec3.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
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

Enemy Enemy::Random(const std::vector<Vec3> &path, const double position)
{
	switch (std::rand() % 3 + 1) {
	case 1: {
		return {Type::TierA, path, position};
	} break;
	case 2: {
		return {Type::TierB, path, position};
	} break;
	case 3: {
		return {Type::TierC, path, position};
	} break;
	}

	return {Type::TierA, path, position};
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

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(Colors::GREEN.data());

		glTranslated(0, 0, .5);
		glScalef(.8, .2, .2);
		Primitives3D::Unit::Cube();
	}
	glPopAttrib();
	glPopMatrix();
}

void Enemy::update()
{
	position += speed;
}

uint8_t Enemy::loseHP(const uint8_t healthPoints)
{
	health = (health > healthPoints) ? health - healthPoints : 0;
	return health;
}

Vec3 Enemy::getGridPosition() const
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

	return startPoint * (1 - segmentProgress) + endPoint * segmentProgress;
}

} // namespace TowerDefense
