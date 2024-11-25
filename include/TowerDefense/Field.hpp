#ifndef INCLUDE_TOWERDEFENSE_FIELD_HPP_
#define INCLUDE_TOWERDEFENSE_FIELD_HPP_

#include "TowerDefense/Cannon.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "TowerDefense/Tower.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>

namespace TowerDefense {

class Field {
      public:
	using Cell = enum : uint32_t {
		CWall = 500, ///< Represents a wall in the field.
		CFloor,      ///< Represents a floor in the field.
		CSlot,       ///< Represents a slot in the field.
		CCannon,     ///< Represents a cannon in the field.
	};

	using Map = std::vector<std::vector<Cell>>;

	using FrameTime = std::chrono::steady_clock::time_point;

	Field(const std::vector<std::vector<u32>> &map,
	      const Vec3 &enemyGridStartPosition);
	Field(Field &&)                 = default;
	Field(const Field &)            = default;
	Field &operator=(Field &&)      = default;
	Field &operator=(const Field &) = default;
	~Field()                        = default;

	static std::optional<Field>
	FromFile(const std::filesystem::path &filepath,
	         const Vec3 &enemyGridStartPosition);

	static Field Generate(const u32 rows, const u32 cols, const u8 waves);

	friend std::ostream &operator<<(std::ostream &os, const Field &field)
	{
		os << "Field {" << std::endl;
		for (size_t y = 0; y < field.map.at(0).size(); ++y) {
			if (y > 0) {
				os << std::endl;
			}

			for (size_t x = 0; x < field.map.size(); ++x) {
				const Cell c = field.map.at(x).at(y);
				if (x > 0) {
					os << " ";
				}

				if (c < CWall) {
					os << static_cast<int>(c);
					continue;
				}

				switch (c) {
				case Field::CWall: {
					os << "\033[31mW\033[0m";
				} break;
				case Field::CFloor: {
					os << "\033[32mF\033[0m";
				} break;
				case Field::CSlot: {
					os << "\033[33mS\033[0m";
				} break;
				case Field::CCannon: {
					os << "\033[33mC\033[0m";
				} break;
				}
			}
		}
		os << std::endl << field.getTower();
		os << std::endl << "points: " << field.getPoints();
		os << std::endl << "}";

		return os;
	}

	[[nodiscard]] double getPoints() const;
	[[nodiscard]] Map getMap() const;
	[[nodiscard]] std::vector<Enemy> getEnemies() const;
	[[nodiscard]] std::vector<Cannon> getCannons() const;
	[[nodiscard]] size_t getEnemiesSize() const;
	[[nodiscard]] size_t getCannonsSize() const;
	[[nodiscard]] std::vector<Vec3> getEnemyPath() const;
	[[nodiscard]] Tower getTower() const;
	[[nodiscard]] uint8_t getGameSpeed() const;
	[[nodiscard]] uint8_t getRemainingCannons() const;
	[[nodiscard]] Vec3 getEnemyStartPosition() const;
	[[nodiscard]] Vec3 getSelectedPosition() const;

	[[nodiscard]] std::pair<int, int> getMapDimensions() const;
	void setGameSpeed(const uint8_t gameSpeed);
	Field &addPoints(const Stats::HealthPoints &points);
	void printPoints() const;
	void draw() const;
	void drawHUD() const;
	void drawFloor() const;
	void drawEnemyPath() const;
	void update(const Stats::CooldownMs deltaTimeMs);
	void placeCannon(const Stats::Tier &cannonType);
	void upgradeCannon();

	void moveSelectedPosition(const Vec3 &movement);
	Field &setDrawCannons(const bool enable);
	Field &setDrawEnemies(const bool enable);
	Field &setDrawFloor(const bool enable);
	Field &setDrawTower(const bool enable);

	Field &setDrawEnemyPath(const bool enable);

	[[nodiscard]] Cell getCell(const u32 row, const u32 col) const;
	[[nodiscard]] Cell getCell(const Vec3 &pos) const;
	[[nodiscard]] Cell &getCell(const u32 row, const u32 col);
	[[nodiscard]] Cell &getCell(const Vec3 &pos);

	void printInfoAtSelectedPosition() const;

	[[nodiscard]] std::optional<Cannon> getCannonAt(const Vec3 &pos) const;
	[[nodiscard]] std::optional<Enemy> getEnemy(const u32 enemyIndex) const;

	[[nodiscard]] u32 getWave() const;


      private:
	Stats::HealthPoints points; ///< The current points scored.
	Map map;                    ///< The 2D map of the field.
	// TODO: use another structure for the cannons/enemies
	std::vector<Cannon> cannons; ///< The list of cannons in the field.
	std::vector<Enemy> enemies;  ///< The list of enemies in the field.
	std::vector<Vec3> enemyPath; ///< The path of the enemies.
	Tower tower;                 ///< The tower object in the field.
	u32 gameSpeed;               ///< The speed of the game.
	u32 remainingCannons;        ///< The number of remaining cannons.
	u32 rows, cols; ///< The number of rows and columns in the map.
	Vec3 selectedGridPosition; ///< The currently selected position.
	bool bDrawCannons;         ///< Whether to draw the cannons
	bool bDrawEnemies;         ///< Whether to draw the enemies
	bool bDrawTower;           ///< Whether to draw the tower
	bool bDrawFloor;           ///< Whether to draw the floor
	bool bDrawEnemyPath;       ///< Whether to draw the enemy path
	u32 wave;
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_FIELD_HPP_
