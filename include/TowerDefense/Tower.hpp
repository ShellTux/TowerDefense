#ifndef INCLUDE_TOWERDEFENSE_TOWER_HPP_
#define INCLUDE_TOWERDEFENSE_TOWER_HPP_

#include "Vec3.hpp"

#include <cstdint>
#include <ostream>

namespace TowerDefense {

/**
 * @class Tower
 * @brief A class representing a tower that can take damage and be drawn in a game.
 *
 * The `Tower` class encapsulates the properties and behaviors of a tower in a game.
 * It includes attributes such as position and health, and provides methods to
 * manage its state and render it visually.
 */
class Tower {
      public:
	/**
	 * @brief Default constructor that initializes the tower at default values.
	 *
	 * The default values are:
	 * - gridPosition: (0, 0, 0)
	 * - health: 10
	 */
	Tower()
	    : gridPosition(0, 0, 0)
	    , health(10)
	{}

	/**
	 * @brief Parameterized constructor that initializes the tower with a specified position.
	 *
	 * @param position A `Vec3` object representing the position of the tower in the game world.
	 */
	Tower(const Vec3 &gridPosition)
	    : Tower()
	{
		this->gridPosition = gridPosition;
	}

	/**
	 * @brief Parameterized constructor that initializes the tower with a specified position and health.
	 *
	 * @param gridPosition A `Vec3` object representing the position of the tower.
	 * @param health The health of the tower.
	 */
	Tower(const Vec3 &gridPosition, const uint8_t health)
	    : gridPosition(gridPosition)
	    , health(health)
	{}

	Tower(Tower &&)                 = default;
	Tower(const Tower &)            = default;
	Tower &operator=(Tower &&)      = default;
	Tower &operator=(const Tower &) = default;
	~Tower()                        = default;

	/**
	 * @brief Overloads the output stream operator for easy printing of Tower details.
	 *
	 * @param os The output stream to write to.
	 * @param tower The Tower object to retrieve details from.
	 * @return The output stream after writing the tower details.
	 */
	friend std::ostream &operator<<(std::ostream &os, const Tower &tower)
	{
		os << "Tower: " << tower.getGridPosition()
		   << ", health: " << static_cast<int>(tower.getHealth());
		return os;
	}

	/**
	 * @brief Gets the position of the tower in the grid.
	 *
	 * @return The position of the tower as a `Vec3` object.
	 */
	[[nodiscard]] Vec3 getGridPosition() const;

	/**
	 * @brief Gets the current health of the tower.
	 *
	 * @return The health of the tower as an 8-bit unsigned integer.
	 */
	[[nodiscard]] uint8_t getHealth() const;

	/**
	 * @brief Applies damage to the tower and reduces its health.
	 *
	 * @param damage The amount of damage to apply to the tower.
	 * @return The health of the tower after taking damage.
	 */
	uint8_t damage(const uint8_t damage);

	/**
	 * @brief Draws the tower representation on a grid.
	 */
	void draw() const;

      private:
	Vec3 gridPosition; ///< The position of the tower in the grid.
	uint8_t health;    ///< The health of the tower.

	/**
	 * @brief Draws the tower health representation on a grid.
	 */
	void drawHealth() const;
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_TOWER_HPP_
