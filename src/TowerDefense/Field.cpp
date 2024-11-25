#include "TowerDefense/Field.hpp"

#include "Color.hpp"
#include "Primitives/2D/core.hpp"
#include "TowerDefense/Cannon.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "TowerDefense/Tower.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <utility>
#include <vector>

namespace TowerDefense {

Field::Field(const std::vector<std::vector<u32>> &map,
             const Vec3 &enemyGridStartPosition)
    : cols(map.at(0).size())
    , rows(map.size())
{
	this->map = Map(rows, std::vector<Cell>(cols, CWall));

	std::map<u32, Vec3> enemyPathMap;

	Vec3 towerPos;
	u32 highestPos = 0;
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			const u32 c = map.at(i).at(j);
			const Vec3 pos(static_cast<f64>(i),
			               static_cast<f64>(j));

			this->map[i][j] = static_cast<Cell>(c);

			if (c < CWall) {
				enemyPathMap[c] = pos;

				if (c > highestPos) {
					highestPos = c;
					towerPos   = pos;
				}
				continue;
			}

			switch (static_cast<Cell>(c)) {
			case CWall:
			case CFloor:
			case CSlot:
				break;
			case CCannon: {
				switch (std::rand() % 3 + 1) {
				case 1:
					cannons.emplace_back(Stats::Tier::A,
					                     pos);
					break;
				case 2:
					cannons.emplace_back(Stats::Tier::B,
					                     pos);
					break;
				case 3:
					cannons.emplace_back(Stats::Tier::C,
					                     pos);
					break;
				default:
					break;
				}
			} break;
			}
		}
	}

	enemyPath.push_back(enemyGridStartPosition);
	for (const auto &[index, pos] : enemyPathMap) {
		enemyPath.push_back(pos);
	}

	static constexpr u32 enemiesN = 100;
	for (u32 i = 0; i < enemiesN; ++i) {
		enemies.push_back(Enemy::Random(enemyPath, -.1 * i));
	}

	tower = Tower(towerPos);
}

Field Field::Generate(const u32 rows, const u32 cols, const u8 waves)
{
	(void) rows;
	(void) cols;
	(void) waves;
	// TODO: Generate Field
#define F TowerDefense::Field::Cell::CFloor
#define W TowerDefense::Field::Cell::CWall
#define S TowerDefense::Field::Cell::CSlot
#define C TowerDefense::Field::Cell::CCannon

	return Field(
	    {
	        { W,  W,  W,  W,  W, W, W, W, W, W},
	        { W,  W,  W,  W, 10, 9, 8, 7, W, W},
	        { W,  W,  W,  W, 11, W, W, 6, W, W},
	        {26, 25,  W,  S, 12, W, W, 5, W, W},
	        { W, 24,  W,  S, 13, W, W, 4, W, W},
	        { C, 23,  W,  S, 14, W, W, 3, S, S},
	        { C, 22,  W,  S, 15, W, W, 2, 1, 0},
	        { W, 21,  W,  W, 16, W, W, S, S, S},
	        { W, 20, 19, 18, 17, W, W, W, W, W},
        },
	    {6, 10});
#undef F
#undef W
#undef S
#undef C
}

Stats::HealthPoints Field::getPoints() const
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

std::vector<Cannon> Field::getCannons() const
{
	return cannons;
}

size_t Field::getEnemiesSize() const
{
	return enemies.size();
}

size_t Field::getCannonsSize() const
{
	return cannons.size();
}

std::vector<Vec3> Field::getEnemyPath() const
{
	return enemyPath;
}

Tower Field::getTower() const
{
	return tower;
}

u8 Field::getRemainingCannons() const
{
	return remainingCannons;
}

Vec3 Field::getSelectedPosition() const
{
	return selectedGridPosition;
}

std::pair<usize, usize> Field::getMapDimensions() const
{
	return {map.size(), map.at(0).size()};
}

Field &Field::addPoints(const Stats::HealthPoints &points)
{
	this->points += points;
	return *this;
}

void Field::draw() const
{
	drawFloor();

	glPushMatrix();
	{
		glTranslated(0, 0, .5);
		if (bDrawEnemies) {
			for (const auto &enemy : enemies) {
				enemy.draw();
			}
		}

		if (bDrawCannons) {
			for (const auto &cannon : cannons) {
				cannon.draw(selectedGridPosition);
			}
		}

		if (bDrawTower) {
			tower.draw();
		}
	}
	glPopMatrix();

	drawEnemyPath();
}

void Field::drawHUD() const
{
	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT | GL_LINE_BIT;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-.5, 5.5, -.5, .5, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	static constexpr std::array<Color, 5> a = {
	    Colors::WHITE,
	    Colors::RED,
	    Colors::ORANGE,
	    Colors::PURPLE,
	    Colors::YELLOW,
	};

	for (size_t i = 0; i < a.size(); ++i) {
		const Color &color = a.at(i);

		glPushAttrib(glMask);
		glPushMatrix();
		{
			glColor3ubv(color.data());
			glTranslatef(GLfloat(i), 0, 0);
			Primitives2D::Unit::Circle();
		}
		glPopMatrix();
		glPopAttrib();
	}
}

void Field::drawFloor() const
{
	if (!bDrawFloor) {
		return;
	}

	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT;

	const auto [selectedI, selectedJ, _]
	    = selectedGridPosition.getCoordinates();

	glPushAttrib(glMask);
	for (u8 i = 0; i < rows; ++i) {
		for (u8 j = 0; j < cols; ++j) {
			using Colors::WHITE, Colors::YELLOW, Colors::CYAN,
			    Colors::GRAY;

			glPushMatrix();
			glTranslated(j, i, 0);

			if (i == selectedI && j == selectedJ) {
				glColor3ubv(YELLOW.data());
				glScalef(.9, .9, 1);
			} else {
				switch (map.at(i).at(j)) {
				case CSlot:
					glColor3ubv(WHITE.data());
					break;
				case CCannon:
				case CWall:
					glColor3ubv(Colors::MINT.data());
					break;
				default:
					glColor3ubv(GRAY.data());
					break;
				}
			}

			glPushMatrix();
			{
				glTranslated(0, 0, -.01);
				Primitives2D::Unit::Square();
			}
			glPopMatrix();

			glPopMatrix();
		}
	}
	glPopAttrib();
}

void Field::drawEnemyPath() const
{
	if (!bDrawEnemyPath) {
		return;
	}

	static constexpr GLbitfield glMask = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT
	                                     | GL_LIGHTING_BIT | GL_POLYGON_BIT
	                                     | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	                                     | GL_VIEWPORT_BIT | GL_LINE_BIT;

	static constexpr std::array<u8, 4> color = {255, 255, 255, 255};

	glPushMatrix();
	glPushAttrib(glMask);
	{
		glColor3ubv(color.data());
		glLineWidth(10);
		glBegin(GL_LINE_STRIP);
		for (const Vec3 &enemyPathPos : enemyPath) {
			const auto [posY, posX, _]
			    = enemyPathPos.getCoordinates();

			glVertex2d(posX, posY);
		}
		glEnd();
	}
	glPopAttrib();
	glPopMatrix();
}

void Field::update(const Stats::TimeMs deltaTimeMs)
{
	for (Enemy &enemy : enemies) {
		enemy.update(deltaTimeMs);
	}

	for (Cannon &cannon : cannons) {
		cannon.update(deltaTimeMs, enemies);
	}

	std::vector<Enemy> enemiesF;

	using std::copy_if, std::back_inserter;

	copy_if(enemies.begin(),
	        enemies.end(),
	        back_inserter(enemiesF),
	        [this](const Enemy &enemy) {
		        if (enemy.getPosition() >= 1) {
			        tower.damage(enemy.getHealth());
			        return false;
		        }

		        if (enemy.getHealth() <= 0) {
			        addPoints(enemy.getPoints());
			        return false;
		        }

		        return true;
	        });

	enemies = enemiesF;
}

void Field::placeCannon(const Stats::Tier &cannonType)
{
	using Stats::CannonStats, Stats::HealthPoints;

	const CannonStats cannonStats
	    = Stats::Cannon::Get(cannonType, Stats::Level::L1);

	if (cannonStats.cost > getPoints()) {
		return;
	}

	Cell &c = getCell(selectedGridPosition);

	if (c != CSlot) {
		return;
	}

	c = CCannon;
	cannons.emplace_back(cannonType, selectedGridPosition);

	addPoints(-cannonStats.cost);
}

void Field::upgradeCannon()
{
	if (getCell(selectedGridPosition) != CCannon) {
		return;
	}

	for (Cannon &cannon : cannons) {
		if (cannon.getGridPosition() != selectedGridPosition) {
			continue;
		}

		cannon.upgrade();
	}
}

void Field::moveSelectedPosition(const Vec3 &movement)
{
	auto [newI, newJ, _]
	    = (selectedGridPosition + movement).getCoordinates();

	if (newI < 0) {
		newI = rows - 1;
	} else if (newI >= rows) {
		newI = 0;
	}

	if (newJ < 0) {
		newJ = cols - 1;
	} else if (newJ >= cols) {
		newJ = 0;
	}

	selectedGridPosition = Vec3(newI, newJ);
}

Field &Field::setDrawCannons(const bool enable)
{
	bDrawCannons = enable;

	return *this;
}

Field &Field::setDrawEnemies(const bool enable)
{
	bDrawEnemies = enable;

	return *this;
}

Field &Field::setDrawFloor(const bool enable)
{
	bDrawFloor = enable;

	return *this;
}

Field &Field::setDrawTower(const bool enable)
{
	bDrawTower = enable;

	return *this;
}

Field &Field::setDrawEnemyPath(const bool enable)
{
	bDrawEnemyPath = enable;

	return *this;
}

Field::Cell Field::getCell(const u32 row, const u32 col) const
{
	return map.at(row).at(col);
}

Field::Cell Field::getCell(const Vec3 &pos) const
{
	const auto &[row, col, _] = pos.getCoordinates();

	return getCell(u32(row), u32(col));
}

Field::Cell &Field::getCell(const u32 row, const u32 col)
{
	return map.at(row).at(col);
}

Field::Cell &Field::getCell(const Vec3 &pos)
{
	const auto &[row, col, _] = pos.getCoordinates();

	return getCell(u32(row), u32(col));
}

void Field::printInfoAtSelectedPosition() const
{
#ifndef RELEASE
	switch (getCell(selectedGridPosition)) {
	case CWall: {
		std::cout << "Wall: " << selectedGridPosition << std::endl;
	} break;
	case CFloor: {
		std::cout << "Floor: " << selectedGridPosition << std::endl;
	} break;
	case CSlot: {
		std::cout << "Slot: " << selectedGridPosition << std::endl;
	} break;
	case CCannon: {
		std::cout << getCannonAt(selectedGridPosition) << std::endl;
	} break;
	}
#endif
}

std::optional<Cannon> Field::getCannonAt(const Vec3 &pos) const
{
	for (const Cannon &cannon : cannons) {
		if (cannon.getGridPosition() != pos) {
			continue;
		}

		return cannon;
	}

	return std::nullopt;
}

std::optional<Enemy> Field::getEnemy(const u32 enemyIndex) const
{
	if (enemyIndex >= enemies.size()) {
		return std::nullopt;
	}

	return enemies.at(enemyIndex);
}

u32 Field::getWave() const
{
	return wave;
}

} // namespace TowerDefense
