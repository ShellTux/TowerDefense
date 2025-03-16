#include "TowerDefense/Field.hpp"

#include "Color.hpp"
#include "OpenGL/Material.hpp"
#include "Primitives/2D/core.hpp"
#include "Primitives/3D/core.hpp"
#include "Texture.hpp"
#include "TowerDefense/Cannon.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "TowerDefense/Tower.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace TowerDefense {

std::ostream &operator<<(std::ostream &os, const enum Field::Cell &cell)
{
	switch (cell) {
	case Field::CWall: {
		os << "W";
	} break;
	case Field::CSlot: {
		os << "\033[33m";
		os << "S";
	} break;
	case Field::CCannon: {
		os << "\033[33m";
		os << "C";
	} break;
	case Field::CPcb1:
	case Field::CPcb2:
	case Field::CPcb3: {
		os << "\033[34m";
		os << "P";
	} break;
	}

	os << "\033[0m";

	return os;
}

std::ostream &operator<<(std::ostream &os, const Field &field)
{
	os << "Field {" << std::endl;

	const usize rows = field.map.size();
	const usize cols = field.map.at(0).size();
	for (usize i = 0; i < rows; ++i) {
		for (usize j = 0; j < cols; ++j) {
			const enum Field::Cell c = field.map.at(i).at(j);
			if (j > 0) {
				os << " ";
			}

			if (c < Field::CPcb1) {
				os << static_cast<int>(c);
				continue;
			}

			os << c;
		}
		os << std::endl;
	}

	os << std::endl << field.getTower();
	os << std::endl << "points: " << field.getPoints();
	os << std::endl << "}";

	return os;
}

Field::Field(const std::vector<std::vector<u32>> &map)
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
			case CSlot:
			case CPcb1:
			case CPcb2:
			case CPcb3:
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

	for (const auto &[index, pos] : enemyPathMap) {
		enemyPath.push_back(pos);
	}

	static constexpr u32 enemiesN = 100;
	for (u32 i = 0; i < enemiesN; ++i) {
		enemies.push_back(Enemy::Random(enemyPath, -.2 - .3 * i));
	}

	tower = Tower(towerPos);
}

Field::u32Map Field::GenerateMap(const u32 rows, const u32 cols)
{
	u32Map map(rows, std::vector<u32>(cols, CWall));

	for (u32 i = 0; i < rows; ++i) {
		for (u32 j = 0; j < cols; ++j) {
			u32 &c = map[i][j];

			switch (std::rand() % 7 + 1) {
			case 1: {
				c = CPcb1;
			} break;
			case 2: {
				c = CPcb2;
			} break;
			case 3: {
				c = CPcb3;
			} break;
			case 4: {
				if (std::rand() % 10 == 0) {
					c = CSlot;
				}
			} break;
			default:
				break;
			}
		}
	}

	u32 enemyPathPos = 0;

	u32 i     = Random::u32(0, rows - 1);
	u32 nextI = Random::u32(0, rows - 1);

	for (u32 j = 0; j < cols; ++j) {
		map[i][j] = enemyPathPos++;

		while (i < nextI) {
			map[++i][j] = enemyPathPos++;
		}

		while (i > nextI) {
			map[--i][j] = enemyPathPos++;
		}

		i = nextI;

		if (std::rand() % 3 == 0) {
			nextI = Random::u32(0, rows - 1);
		}
	}

	// WARN: Possible infinite loop
	for (u8 slots = 10; slots > 0;) {
		usize i = Random::u32(0, rows - 1);
		usize j = Random::u32(0, cols - 1);

		u32 &c = map[i][j];
		/*if (c >= CWall) {*/
		/*	continue;*/
		/*}*/

		switch (static_cast<Cell>(c)) {
		case CPcb1:
		case CPcb2:
		case CPcb3: {
			c = CSlot;
			slots--;
		} break;
		default:
			break;
		}
	}

	return map;
}

Field Field::Generate(const u32 rows, const u32 cols, const u8 waves)
{
	(void) waves;

	Field field(GenerateMap(rows, cols));

#ifdef DEBUG
	std::cout << field << std::endl;
#endif

	return field;
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-.5, 5.5, -.5, .5, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushAttrib(drawGlMask);
	glEnable(GL_TEXTURE_2D);
	for (usize i = 0; i < 4; ++i) {
		glPushMatrix();
		{
			switch (i) {
			case 0: {
				Texture::Get("assets/red-cannon.png").Bind();
			} break;
			case 1: {
				Texture::Get("assets/orange-cannon.png").Bind();
			} break;
			case 2: {
				Texture::Get("assets/purple-cannon.png").Bind();
			} break;
			case 3: {
				Texture::Get("assets/upgrade.png").Bind();
			} break;
			}

			glTranslated(f64(i), 0, 0);
			Primitives2D::Unit::Square();

			Texture::UnBind();
		}
		glPopMatrix();
	}
	glPopAttrib();
}

void Field::drawFloor() const
{
	if (!bDrawFloor) {
		return;
	}

	glPushAttrib(drawGlMask);
	{
		glEnable(GL_TEXTURE_2D);

		Texture::Get("assets/pcb-circuit-1.png").Bind();
		OpenGL::Material::Apply(OpenGL::Material::Type::Gold);

		glPushMatrix();
		{
			glTranslated(-.5 + cols * .5, -.5 + rows * .5, -.01);
			glScaled(cols, rows, 1);
			Primitives2D::Unit::Grid(rows, cols);
		}
		glPopMatrix();

		Texture::UnBind();

		for (u32 i = 0; i < rows; ++i) {
			for (u32 j = 0; j < cols; ++j) {
				drawPcb(i, j);
			}
		}

		drawSelected();
	}
	glPopAttrib();
}

void Field::drawEnemyPath() const
{
	if (!bDrawEnemyPath) {
		return;
	}

	glPushMatrix();
	glPushAttrib(drawGlMask);
	{
		glColor3ubv(Colors::BROWN.data());
		OpenGL::Material::Apply(OpenGL::Material::Type::Copper);

		glLineWidth(10);

		glTranslated(0, 0, .05);

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

	std::sort(enemiesF.begin(),
	          enemiesF.end(),
	          [](const Enemy &a, const Enemy &b) {
		          return a.getPosition() > b.getPosition();
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
	std::cout << infoAtSelectedPosition();
}

std::string Field::infoAtSelectedPosition() const
{
	std::ostringstream info{};

#ifndef RELEASE
	switch (getCell(selectedGridPosition)) {
	case CPcb1: {
		info << "PCB1: " << selectedGridPosition;
	} break;
	case CPcb2: {
		info << "PCB2: " << selectedGridPosition;
	} break;
	case CPcb3: {
		info << "PCB3: " << selectedGridPosition;
	} break;
	case CSlot: {
		info << "Slot: " << selectedGridPosition;
	} break;
	case CCannon: {
		info << getCannonAt(selectedGridPosition);
	} break;
	case CWall: {
		info << "Wall: " << selectedGridPosition;
	} break;
	default: {
		info << selectedGridPosition;
	} break;
	}
#endif

	return info.str();
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

void Field::drawPcb(const u32 i, const u32 j) const
{
	const Cell &cell = map[i][j];

	switch (cell) {
	case CWall:
	case CCannon:
		return;
	default:
		break;
	}

	glPushAttrib(drawGlMask);
	glEnable(GL_TEXTURE_2D);
	{
		switch (cell) {
		case CPcb1: {
			glColor3ubv(Colors::GREEN.data());
			OpenGL::Material::Apply(OpenGL::Material::GreenPlastic);
		} break;
		case CPcb2: {
			glColor3ubv(Colors::PURPLE.data());
			OpenGL::Material::Apply(OpenGL::Material::Purple);
		} break;
		case CPcb3: {
			glColor3ubv(Colors::YELLOW.data());
			OpenGL::Material::Apply(
			    OpenGL::Material::YellowPlastic);
		} break;
		case CSlot: {
			Texture::Get("assets/pcb-circuit-slot.png").Bind();
		} break;
		case CWall:
		case CCannon:
			break;
		default:
			glColor3ubv(Colors::GRAY.data());
			break;
		}

		glPushMatrix();
		{
			glTranslated(j, i, 0);

			switch (cell) {
			case CPcb1:
			case CPcb2:
			case CPcb3: {
				glTranslated(0, 0, .2);
				glScaled(.8, .8, .2);
				Primitives3D::Unit::Cube();
			} break;
			case CSlot: {
				Primitives2D::Unit::Circle();
			} break;
			case CWall:
			case CCannon:
				break;
			default:
				Primitives2D::Unit::Square();
			}
		}

		Texture::UnBind();
	}
	glPopMatrix();
	glPopAttrib();
}

void Field::drawSelected() const
{
	const auto &[posY, posX, _] = getSelectedPosition().getCoordinates();

	glPushAttrib(drawGlMask);
	glPushMatrix();
	{
		glColor3ubv(Colors::PINK.data());

		glTranslated(posX, posY, .01);

		Primitives2D::Unit::Square();
	}
	glPopAttrib();
	glPopMatrix();
}

} // namespace TowerDefense
