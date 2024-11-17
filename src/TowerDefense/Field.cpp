#include "TowerDefense/Field.hpp"

#include "Color.hpp"
#include "Primitives/2D/core.hpp"
#include "TowerDefense/Cannon/Base.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Tower.hpp"
#include "TowerDefense/Vec3.hpp"

#include <GL/gl.h>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
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
    , enemyGridStartPosition({0, 0})
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
				cannons.emplace_back(pos);
			}

			this->map[i][j] = static_cast<Cell>(c);
		}
	}

	for (const auto &[index, pos] : enemyPathMap) {
		enemyPath.push_back(pos);
	}

	enemies.emplace_back(enemyPath);

	tower = Tower(towerPos);
}

std::optional<Field> Field::FromFile(const std::filesystem::path &filepath)
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

	return Field(field);
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
	return enemyGridStartPosition;
}

Vec3 Field::getSelectedPosition() const
{
	return selectedGridPosition;
}

void Field::setGameSpeed(const uint8_t gameSpeed)
{
	this->gameSpeed = gameSpeed;
}

void Field::draw() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-.7, cols - .2, rows - .2, -.7, -1000, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	drawFloor();

	if (bDrawEnemies) {
		for (const auto &enemy : enemies) {
			enemy.draw();
		}
	}

	if (bDrawCannons) {
		for (const auto &cannon : cannons) {
			cannon.draw();
		}
	}

	if (bDrawTower) {
		tower.draw();
	}

	drawEnemyPath();
}

void Field::drawHUD() const
{
	for (GLfloat i = 0; i < 5; ++i) {
		glPushMatrix();
		{
			glTranslatef(i, 0, 0);
			Primitives2D::Unit::Circle(10);
		}
		glPopMatrix();
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
	{
		for (uint8_t i = 0; i < rows; ++i) {
			for (uint8_t j = 0; j < cols; ++j) {
				using Colors::WHITE, Colors::ORANGE,
				    Colors::CYAN, Colors::GRAY;

				if (i == selectedI && j == selectedJ) {
					glColor3ubv(ORANGE.data());
				} else {
					switch (map.at(i).at(j)) {
					case CFloor:
						glColor3ubv(WHITE.data());
						break;
					case CSlot:
						glColor3ubv(CYAN.data());
						break;
					default:
						glColor3ubv(GRAY.data());
						break;
					}
				}

				glPushMatrix();
				{
					glTranslated(j, i, 0);
					Primitives2D::Unit::Square();
				}
				glPopMatrix();
			}
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

void Field::update()
{
	for (Enemy &enemy : enemies) {
		enemy.update();
	}

	for (Cannon &cannon : cannons) {
		cannon.update(enemies);
	}

	std::vector<Enemy> enemiesF;

	using std::copy_if, std::back_inserter;

	copy_if(enemies.begin(),
	        enemies.end(),
	        back_inserter(enemiesF),
	        [](const Enemy &enemy) { return enemy.getPosition() < 1; });

	enemies = enemiesF;
}

void Field::placeCannon() {}

void Field::upgradeCannon() {}

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
