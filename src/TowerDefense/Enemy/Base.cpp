#include "TowerDefense/Enemy/Base.hpp"

#include "Primitives/3D/core.hpp"
#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <array>
#include <cstddef>
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

void Enemy::draw() const
{
	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	static constexpr std::array<uint8_t, 4> color = {255, 0, 0, 255};

	const auto [posX, posY, _] = getGridPosition().getCoordinates();

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
	if (path.empty()) {
		return {};
	}

	const double clampedPosition = std::clamp(position, 0.0, 1.0);

	const size_t totalPoints       = path.size();
	const double distanceAlongPath = clampedPosition * (totalPoints - 1);

	const size_t startIdx = static_cast<size_t>(distanceAlongPath);
	const size_t endIdx   = std::min(startIdx + 1, totalPoints - 1);

	const Vec3 &startPoint = path.at(startIdx);
	const Vec3 &endPoint   = path.at(endIdx);

	const double segmentProgress = distanceAlongPath - startIdx;

	return startPoint * (1 - segmentProgress) + endPoint * segmentProgress;
}

} // namespace TowerDefense
