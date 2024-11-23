#include "TowerDefense/Field.hpp"

#include "Color.hpp"
#include "Primitives/2D/core.hpp"
#include "TowerDefense/Cannon.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "TowerDefense/Tower.hpp"
#include "Vec3.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace TowerDefense {

Field::Field(const std::vector<std::vector<uint32_t>> &map,
             const Vec3 &enemyGridStartPosition)
    : points(10)
    , enemies({})
    , enemyPath({})
    , gameSpeed(1)
    , remainingCannons(10)
    , rows(map.size())
    , cols(map.at(0).size())
    , selectedGridPosition({0, 0})
    , bDrawCannons(true)
    , bDrawEnemies(true)
    , bDrawTower(true)
    , bDrawFloor(true)
    , bDrawEnemyPath(true)
{
	this->map = Map(rows, std::vector<Cell>(cols, CWall));

	std::map<uint32_t, Vec3> enemyPathMap;

	Vec3 towerPos;
	uint32_t highestPos = 0;
	for (size_t i = 0; i < this->map.size(); ++i) {
		for (size_t j = 0; j < this->map.at(0).size(); ++j) {
			const uint32_t c = map.at(i).at(j);
			const Vec3 pos(static_cast<double>(i),
			               static_cast<double>(j));

			if (c < CWall) {
				enemyPathMap[c] = pos;

				if (c > highestPos) {
					highestPos = c;
					towerPos   = pos;
				}
			} else if (c == CCannon) {
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
			}

			this->map[i][j] = static_cast<Cell>(c);
		}
	}

	enemyPath.push_back(enemyGridStartPosition);
	for (const auto &[index, pos] : enemyPathMap) {
		enemyPath.push_back(pos);
	}

	static constexpr int enemiesN = 100;
	for (int i = 0; i < enemiesN; ++i) {
		enemies.push_back(Enemy::Random(enemyPath, -.1 * i));
	}

	enemies.emplace_back(Stats::Tier::A, enemyPath);
	enemies.emplace_back(Stats::Tier::B, enemyPath);
	enemies.emplace_back(Stats::Tier::C, enemyPath);

	tower = Tower(towerPos);
}

std::optional<Field> Field::FromFile(const std::filesystem::path &filepath,
                                     const Vec3 &enemyGridStartPosition)
{
	using std::vector, std::ifstream, std::string, std::stringstream;

	vector<vector<string>> array;
	ifstream file(filepath);

	if (!file) {
		std::cerr << "Could not open the file: " << filepath
		          << std::endl;
		return std::nullopt;
	}

	string line;
	while (std::getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		vector<string> row;
		stringstream ss(line);
		string cell;

		while (std::getline(ss, cell, '|')) {
			cell.erase(0, cell.find_first_not_of(" \t"));
			cell.erase(cell.find_last_not_of(" \t") + 1);

			if (cell.empty()) {
				continue;
			}

			row.push_back(cell);
		}

		array.push_back(row);
	}

	const size_t rows = array.size();
	const size_t cols = array.at(0).size();

	vector<vector<uint32_t>> field(rows, vector<uint32_t>(cols, CWall));
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			const string &c = array.at(i).at(j);

			const double iD = static_cast<double>(i);
			const double jD = static_cast<double>(j);

			try {
				const uint32_t number = std::stoi(c);
				field[i][j] = static_cast<uint32_t>(number);
			} catch (const std::invalid_argument &e) {
				if (c == "W") {
					field[i][j] = Field::CWall;
				} else if (c == "F") {
					field[i][j] = Field::CFloor;
				} else if (c == "S") {
					field[i][j] = Field::CSlot;
				} else if (c == "C") {
					field[i][j] = Field::CCannon;
				} else {
					std::cerr
					    << "Unrecognized cell type: " << c
					    << " at " << Vec3(iD, jD)
					    << ", setting to Wall."
					    << std::endl;
				}
			} catch (const std::out_of_range &e) {
				std::cerr << "Number out of range: " << c
				          << " at " << Vec3(iD, jD)
				          << ", setting to Wall." << std::endl;
			}
		}
	}

	return Field(field, enemyGridStartPosition);
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

Vec3 Field::getSelectedPosition() const
{
	return selectedGridPosition;
}

std::pair<int, int> Field::getMapDimensions() const
{
	const size_t rows = map.size();
	const size_t cols = map.at(0).size();

	return {rows, cols};
}

void Field::setGameSpeed(const uint8_t gameSpeed)
{
	this->gameSpeed = gameSpeed;
}

void Field::draw() const
{
	drawFloor();

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
			Primitives2D::Unit::Circle(30);
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
	for (uint8_t i = 0; i < rows; ++i) {
		for (uint8_t j = 0; j < cols; ++j) {
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

			Primitives2D::Unit::Square();
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

	static constexpr std::array<uint8_t, 4> color = {255, 255, 255, 255};

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

void Field::update(const Stats::CooldownMs deltaTimeMs)
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

		        return enemy.getHealth() > 0;
	        });

	enemies = enemiesF;
}

void Field::placeCannon(const Stats::Tier &cannonType)
{
	const auto [selectedI, selectedJ, _]
	    = selectedGridPosition.getCoordinates();

	Cell &c = map.at(size_t(selectedI)).at(size_t(selectedJ));

	if (c != CSlot) {
		return;
	}

	c = CCannon;
	cannons.emplace_back(cannonType, selectedGridPosition);
}

void Field::upgradeCannon()
{
	const auto [selectedI, selectedJ, _]
	    = selectedGridPosition.getCoordinates();

	const Cell &c = map.at(size_t(selectedI)).at(size_t(selectedJ));

	if (c != CCannon) {
		return;
	}
#ifndef RELEASE
	std::cout << "Upgrade cannon at: " << selectedGridPosition << std::endl;
#endif
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

} // namespace TowerDefense
