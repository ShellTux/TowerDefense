#ifndef INCLUDE_TOWERDEFENSE_FIELD_HPP_
#define INCLUDE_TOWERDEFENSE_FIELD_HPP_

#include "TowerDefense/Cannon.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "TowerDefense/Tower.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <chrono>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>

namespace TowerDefense {

class Field {
      public:
	using Cell = enum
	    : u32 { CWall = 500,
		    CFloor,
		    CSlot,
		    CCannon,
	    };

	using Map    = std::vector<std::vector<Cell>>;
	using u32Map = std::vector<std::vector<u32>>;

	using FrameTime = std::chrono::steady_clock::time_point;

	Field(const u32Map &map, const Vec3 &enemyGridStartPosition);
	Field(Field &&)                 = default;
	Field(const Field &)            = default;
	Field &operator=(Field &&)      = default;
	Field &operator=(const Field &) = default;
	~Field()                        = default;

	static Field Generate(const u32 rows, const u32 cols, const u8 waves);

	friend std::ostream &operator<<(std::ostream &os, const Field &field)
	{
		os << "Field {" << std::endl;
		for (usize y = 0; y < field.map.at(0).size(); ++y) {
			if (y > 0) {
				os << std::endl;
			}

			for (usize x = 0; x < field.map.size(); ++x) {
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

	Field &addPoints(const Stats::HealthPoints &points);
	void moveSelectedPosition(const Vec3 &movement);
	void placeCannon(const Stats::Tier &cannonType);
	void update(const Stats::TimeMs deltaTimeMs);
	void upgradeCannon();

	Field &setDrawCannons(const bool enable);
	Field &setDrawEnemies(const bool enable);
	Field &setDrawEnemyPath(const bool enable);
	Field &setDrawFloor(const bool enable);
	Field &setDrawTower(const bool enable);

	void draw() const;
	void drawEnemyPath() const;
	void drawFloor() const;
	void drawHUD() const;
	void printInfoAtSelectedPosition() const;

	[[nodiscard]] Cell &getCell(const u32 row, const u32 col);
	[[nodiscard]] Cell getCell(const u32 row, const u32 col) const;
	[[nodiscard]] Cell &getCell(const Vec3 &pos);
	[[nodiscard]] Cell getCell(const Vec3 &pos) const;
	[[nodiscard]] Map getMap() const;
	[[nodiscard]] Stats::HealthPoints getPoints() const;
	[[nodiscard]] std::optional<Cannon> getCannonAt(const Vec3 &pos) const;
	[[nodiscard]] std::optional<Enemy> getEnemy(const u32 enemyIndex) const;
	[[nodiscard]] std::pair<usize, usize> getMapDimensions() const;
	[[nodiscard]] std::vector<Cannon> getCannons() const;
	[[nodiscard]] std::vector<Enemy> getEnemies() const;
	[[nodiscard]] std::vector<Vec3> getEnemyPath() const;
	[[nodiscard]] Tower getTower() const;
	[[nodiscard]] u32 getWave() const;
	[[nodiscard]] u8 getGameSpeed() const;
	[[nodiscard]] u8 getRemainingCannons() const;
	[[nodiscard]] usize getCannonsSize() const;
	[[nodiscard]] usize getEnemiesSize() const;
	[[nodiscard]] Vec3 getEnemyStartPosition() const;
	[[nodiscard]] Vec3 getSelectedPosition() const;


      private:
	bool bDrawCannons{true};
	bool bDrawEnemies{true};
	bool bDrawEnemyPath{true};
	bool bDrawFloor{true};
	bool bDrawTower{true};
	Map map;
	Stats::HealthPoints points = 100;
	std::vector<Cannon> cannons;
	std::vector<Enemy> enemies;
	std::vector<Vec3> enemyPath;
	Tower tower;
	u32 cols{0};
	u32 remainingCannons{100};
	u32 rows{0};
	u32 wave{1};
	Vec3 selectedGridPosition{0, 0};
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_FIELD_HPP_
