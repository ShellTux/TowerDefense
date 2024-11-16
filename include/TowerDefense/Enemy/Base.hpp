#ifndef INCLUDE_ENEMY_BASE_HPP_
#define INCLUDE_ENEMY_BASE_HPP_

#include "TowerDefense/Vec3.hpp"

#include <cassert>
#include <cstdint>
#include <ostream>
#include <vector>

namespace TowerDefense {

/**
 * @class Enemy
 * @brief Represents an enemy in the game with attributes such as position, speed, health, and points.
 */
class Enemy {
      public:
	/**
	 * @brief Default constructs an Enemy with initial position, speed, health, and points.
	 */
	Enemy()
	    : position(0)
	    , speed(0.1)
	    , health(10)
	    , points(10)
	    , realPosition(0, 0, 0)
	{}

	/**
	 * @brief Constructs an Enemy with specified parameters.
	 *
	 * @param position The initial unidimensional position of the enemy. [0-1]
	 * @param speed The speed of the enemy. [0-1]
	 * @param health The initial health points of the enemy.
	 * @param points The points awarded for defeating the enemy.
	 * @param realPosition The grid position of the enemy in the game.
	 */
	Enemy(double position,
	      double speed,
	      uint8_t health,
	      uint8_t points,
	      Vec3 realPosition)
	    : position(position)
	    , speed(speed)
	    , health(health)
	    , points(points)
	    , realPosition(realPosition)
	{
		assert((0 <= position && position <= 1)
		       && "Position needs to be between [0-1]");
		assert((0 <= speed && speed <= 1)
		       && "speed needs to be between [0-1]");
	}
	Enemy(Enemy &&)                 = default;
	Enemy(const Enemy &)            = default;
	Enemy &operator=(Enemy &&)      = default;
	Enemy &operator=(const Enemy &) = default;
	~Enemy()                        = default;

	/**
	 * @brief Outputs a string representation of the Enemy.
	 *
	 * @param os The output stream to write to.
	 * @param enemy The Enemy object to output.
	 * @return The modified output stream.
	 */
	friend std::ostream &operator<<(std::ostream &os, const Enemy &enemy)
	{
		os << "Enemy: " << enemy.getPosition()
		   << " ~= " << enemy.getRealPosition()
		   << ", speed: " << enemy.getSpeed()
		   << ", health: " << static_cast<int>(enemy.getHealth())
		   << ", points: " << static_cast<int>(enemy.getPoints());
		return os;
	}

	/**
	 * @brief Gets the enemy's unidimensional position.
	 *
	 * @return The current position of the enemy.
	 */
	[[nodiscard]] double getPosition() const;

	/**
	 * @brief Gets the speed of the enemy.
	 *
	 * @return The speed of the enemy.
	 */
	[[nodiscard]] double getSpeed() const;

	/**
	 * @brief Gets the current health of the enemy.
	 *
	 * @return The current health points of the enemy.
	 */
	[[nodiscard]] uint8_t getHealth() const;

	/**
	 * @brief Gets the points awarded for defeating the enemy.
	 *
	 * @return The points associated with the enemy.
	 */
	[[nodiscard]] uint8_t getPoints() const;

	/**
	 * @brief Gets the real grid position of the enemy.
	 *
	 * @return The real position of the enemy as a Vec3 object.
	 */
	[[nodiscard]] Vec3 getRealPosition() const;

	/**
	 * @brief Draws the enemy on the screen.
	 *
	 * @param rows The number of rows in the grid.
	 * @param cols The number of columns in the grid.
	 */
	void draw(const uint8_t rows, const uint8_t cols) const;

	/**
	 * @brief Updates the enemy along the specified path.
	 *
	 * @param enemyPath A vector of Vec3 representing the path for the enemy to follow,
	 *                  where the enemy starts at the first position and moves towards the last.
	 *                  Positions are normalized such that the first position is 0 and the last is 1,
	 *                  allowing for linear interpolation between grid positions.
	 */
	void update(const std::vector<Vec3> &enemyPath);

	/**
	 * @brief Decreases the enemy's health by a specified amount.
	 *
	 * @param healthPoints The amount of health points to lose.
	 * @return The remaining health points after loss.
	 */
	uint8_t loseHP(const uint8_t healthPoints);

      private:
	double position;   ///< The enemy's current unidimensional position.
	double speed;      ///< The speed at which the enemy moves.
	uint8_t health;    ///< The current health of the enemy.
	uint8_t points;    ///< The points awarded for defeating this enemy.
	Vec3 realPosition; ///< The grid position of the enemy.

	/**
	 * @brief Converts the map coordinates to path coordinates.
	 *
	 * @param enemyPath The enemy path to convert.
	 * @return The converted Vec3 position.
	 */
	[[nodiscard]] Vec3 map2path(const std::vector<Vec3> &enemyPath) const;

	/**
	 * @brief Updates the enemy's position based on the provided path.
	 *
	 * @param enemyPath The path to follow for position updates.
	 */
	void updatePosition(const std::vector<Vec3> &enemyPath);
};

} // namespace TowerDefense

#endif // INCLUDE_ENEMY_BASE_HPP_
