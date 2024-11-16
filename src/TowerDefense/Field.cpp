#include "TowerDefense/Field.hpp"

#include "Primitives/2D/core.hpp"
#include "Primitives/3D/core.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Tower.hpp"
#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace TowerDefense {

Field::Field(const std::vector<std::vector<uint32_t>> &map)
    : points(10)
    , enemies({})
    , enemyPath({})
    , gameSpeed(1)
    , remainingCannons(10)
    , rows(map.size())
    , cols(map.at(0).size())
    , enemyStartPosition({0, 0})
    , selectedPosition({0, 0})
{
	this->map = Map(rows, std::vector<Cell>(cols, Wall));

	Vec3 towerPos;
	uint32_t highestPos = 0;
	for (size_t y = 0; y < this->map.size(); ++y) {
		for (size_t x = 0; x < this->map.at(0).size(); ++x) {
			const uint32_t c = map.at(y).at(x);

			if (c < Wall && c > highestPos) {
				highestPos = c;
				towerPos   = Vec3{static_cast<double>(x),
                                                static_cast<double>(y)};
			}

			this->map[x][y] = static_cast<Cell>(c);
		}
	}

	this->tower = Tower(towerPos);
}

double Field::getPoints() const
{
	return points;
}

Field::Map Field::getMap() const
{
	return map;
}

std::vector<Enemy> Field::getEnemies() const
{
	return enemies;
}

std::vector<Vec3> Field::getEnemyPath() const
{
	return enemyPath;
}

Tower Field::getTower() const
{
	return tower;
}

uint8_t Field::getGameSpeed() const
{
	return gameSpeed;
}

uint8_t Field::getRemainingCannons() const
{
	return remainingCannons;
}

Vec3 Field::getEnemyStartPosition() const
{
	return enemyStartPosition;
}

Vec3 Field::getSelectedPosition() const
{
	return selectedPosition;
}

void Field::setGameSpeed(const uint8_t gameSpeed)
{
	this->gameSpeed = gameSpeed;
}

void Field::draw() const
{
	Primitives3D::Unit::Grid(rows, cols);

	for (const auto &enemy : enemies) {
		enemy.draw(rows, cols);
	}

	for (const auto &cannon : cannons) {
		cannon.draw(rows, cols);
	}

	tower.draw(rows, cols);
}

void Field::drawHUD() const
{
	static constexpr GLfloat p = 1 / 19;

	for (int i = 0; i < 5; ++i) {
		glPushMatrix();
		{
			glTranslatef(-.5 + 1.5 * p + 4 * p * i, 0, 0);
			Primitives2D::Unit::Circle(10);
		}
		glPopMatrix();
	}
}

void Field::drawMap() const {}

void Field::update() {}

void Field::placeCannon() {}

void Field::upgradeCannon() {}

void Field::moveSelectedPosition(const Vec3 &movement)
{
	this->selectedPosition += movement;
}

} // namespace TowerDefense
