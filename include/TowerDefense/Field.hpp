#ifndef INCLUDE_TOWERDEFENSE_FIELD_HPP_
#define INCLUDE_TOWERDEFENSE_FIELD_HPP_

#include "TowerDefense/Cannon/Base.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Stats.hpp"
#include "TowerDefense/Tower.hpp"
#include "Vec3.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>

namespace TowerDefense {

/**
 * @class Field
 * @brief Represents the game field containing various elements like walls, floors, and slots.
 *
 * The `Field` class encapsulates the structure of a game field as well as
 * functionalities for different operations that can be performed within the field.
 * It includes definitions for the types of cells in the field, and methods to manipulate
 * and retrieve information about the game field.
 */
class Field {
      public:
	/**
	 * @enum Cell
	 * @brief Represents the possible types of cells in the field.
	 */
	using Cell = enum : uint32_t {
		CWall = 500, ///< Represents a wall in the field.
		CFloor,      ///< Represents a floor in the field.
		CSlot,       ///< Represents a slot in the field.
		CCannon,     ///< Represents a cannon in the field.
	};

	/**
	 * @brief A 2D vector to represent the field map.
	 */
	using Map = std::vector<std::vector<Cell>>;

	using FrameTime = std::chrono::steady_clock::time_point;

	/**
	 * @brief Constructs a Field object from a given map.
	 *
	 * @param map A 2D vector of uint32_t values representing the initial state of the field.
	 */
	Field(const std::vector<std::vector<uint32_t>> &map,
	      const Vec3 &enemyGridStartPosition);
	Field(Field &&)                 = default;
	Field(const Field &)            = default;
	Field &operator=(Field &&)      = default;
	Field &operator=(const Field &) = default;
	~Field()                        = default;

	static std::optional<Field>
	FromFile(const std::filesystem::path &filepath,
	         const Vec3 &enemyGridStartPosition);

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
		os << std::endl << "}";

		return os;
	}

	/**
	 * @brief Gets the current points scored in the game.
	 *
	 * @return The current points as a double.
	 */
	[[nodiscard]] double getPoints() const;

	/**
	 * @brief Retrieves the map of the field.
	 *
	 * @return The map as a 2D vector of Cell.
	 */
	[[nodiscard]] Map getMap() const;

	/**
	 * @brief Retrieves the list of enemies in the field.
	 *
	 * @return A vector of Enemy objects.
	 */
	[[nodiscard]] std::vector<Enemy> getEnemies() const;

	/**
	 * @brief Retrieves the path of the enemies in the field.
	 *
	 * @return A vector of Vec3 representing enemy paths.
	 */
	[[nodiscard]] std::vector<Vec3> getEnemyPath() const;

	/**
	 * @brief Gets the tower object associated with the field.
	 *
	 * @return The Tower object.
	 */
	[[nodiscard]] Tower getTower() const;

	/**
	 * @brief Retrieves the game speed.
	 *
	 * @return The game speed as an 8-bit unsigned integer.
	 */
	[[nodiscard]] uint8_t getGameSpeed() const;

	/**
	 * @brief Retrieves the number of remaining cannons.
	 *
	 * @return The number of remaining cannons as an 8-bit unsigned integer.
	 */
	[[nodiscard]] uint8_t getRemainingCannons() const;

	/**
	 * @brief Retrieves the starting position of the enemies.
	 *
	 * @return A Vec3 representing the enemy start position.
	 */
	[[nodiscard]] Vec3 getEnemyStartPosition() const;

	/**
	 * @brief Retrieves the currently selected position.
	 *
	 * @return A Vec3 representing the selected position.
	 */
	[[nodiscard]] Vec3 getSelectedPosition() const;

	[[nodiscard]] std::pair<int, int> getMapDimensions() const;

	/**
	 * @brief Sets the game speed.
	 *
	 * @param gameSpeed The new game speed to set.
	 */
	void setGameSpeed(const uint8_t gameSpeed);

	/**
	 * @brief Draws the game field.
	 */
	void draw() const;

	/**
	 * @brief Draws the HUD (Heads-Up Display) of the game.
	 */
	void drawHUD() const;

	/**
	 * @brief Draws the floor of the game field.
	 */
	void drawFloor() const;

	/**
	 * @brief Draws the enemy path.
	 */
	void drawEnemyPath() const;

	/**
	 * @brief Updates the state of the field.
	 */
	void update();

	/**
	 * @brief Places a cannon in the field.
	 */
	void placeCannon(const Stats::Tier &cannonType);

	/**
	 * @brief Upgrades a cannon in the field.
	 */
	void upgradeCannon();

	/**
	 * @brief Moves the selected position by a specified vector.
	 *
	 * @param movement A Vec3 representing the movement vector.
	 */
	void moveSelectedPosition(const Vec3 &movement);

	/**
	 * @brief Toggle drawing of cannons
	 *
	 * @param enable A bool to set drawing of cannons
	 */
	Field &setDrawCannons(const bool enable);

	/**
	 * @brief Toggle drawing of Enemies
	 *
	 * @param enable A bool to set drawing of enemies
	 */
	Field &setDrawEnemies(const bool enable);

	/**
	 * @brief Toggle drawing of Floor
	 *
	 * @param enable A bool to set drawing of floor
	 */
	Field &setDrawFloor(const bool enable);

	/**
	 * @brief Toggle drawing of Tower
	 *
	 * @param enable A bool to set drawing of tower
	 */
	Field &setDrawTower(const bool enable);

	/**
	 * @brief Toggle drawing of Enemy path
	 *
	 * @param enable A bool to set drawing of enemy path
	 */
	Field &setDrawEnemyPath(const bool enable);


      private:
	double points; ///< The current points scored.
	Map map;       ///< The 2D map of the field.
	// TODO: use another structure for the cannons/enemies
	std::vector<Cannon> cannons; ///< The list of cannons in the field.
	std::vector<Enemy> enemies;  ///< The list of enemies in the field.
	std::vector<Vec3> enemyPath; ///< The path of the enemies.
	Tower tower;                 ///< The tower object in the field.
	uint8_t gameSpeed;           ///< The speed of the game.
	uint8_t remainingCannons;    ///< The number of remaining cannons.
	uint8_t rows, cols; ///< The number of rows and columns in the map.
	Vec3 selectedGridPosition; ///< The currently selected position.
	bool bDrawCannons;         ///< Whether to draw the cannons
	bool bDrawEnemies;         ///< Whether to draw the enemies
	bool bDrawTower;           ///< Whether to draw the tower
	bool bDrawFloor;           ///< Whether to draw the floor
	bool bDrawEnemyPath;       ///< Whether to draw the enemy path
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_FIELD_HPP_
