#ifndef INCLUDE_ENEMY_BASE_HPP_
#define INCLUDE_ENEMY_BASE_HPP_

#include "Color.hpp"
#include "Vec3.hpp"

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
	enum class Type { TierA, TierB, TierC };

	/**
	 * @brief Default constructs an Enemy with initial position, speed, health, and points.
	 */
	Enemy(const Type &type, const std::vector<Vec3> &gridPath)
	    : position(0)
	    , gridPath(gridPath)
	    , color(Colors::RED)
	{
		static constexpr double baseSpeed    = (double) 1 / 1000;
		static constexpr uint32_t baseHealth = 10;

		switch (type) {
		case Type::TierA: {
			speed  = 3 * baseSpeed;
			health = 1 * baseHealth;
			points = 1 * baseHealth;
			color  = Colors::RED;
		} break;
		case Type::TierB: {
			speed  = 1.5 * baseSpeed;
			health = 3 * baseHealth;
			points = 3 * baseHealth;
			color  = Colors::ORANGE;
		} break;
		case Type::TierC: {
			speed  = 1 * baseSpeed;
			health = 5 * baseHealth;
			points = 5 * baseHealth;
			color  = Colors::PURPLE;
		} break;
		}
	}

	Enemy(const Type &type,
	      const std::vector<Vec3> &gridPath,
	      const double position)
	    : Enemy(type, gridPath)
	{
		this->position = position;
	}

	static Enemy Random(const std::vector<Vec3> &path,
	                    const double position = 0);

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
	 * @brief Draws the enemy on the screen.
	 */
	void draw() const;

	/**
	 * @brief Updates the enemy along the specified path.
	 */
	void update();

	/**
	 * @brief Decreases the enemy's health by a specified amount.
	 *
	 * @param healthPoints The amount of health points to lose.
	 * @return The remaining health points after loss.
	 */
	uint8_t loseHP(const uint8_t healthPoints);

	/**
	 * @brief Gets position on the grid based on unidimensional position on the path.
	 *
	 * @return The converted Vec3 position.
	 */
	[[nodiscard]] Vec3 getGridPosition() const;

      private:
	double position; ///< The enemy's current unidimensional position.
	double speed;    ///< The speed at which the enemy moves.
	uint32_t health; ///< The current health of the enemy.
	uint8_t points;  ///< The points awarded for defeating this enemy.
	std::vector<Vec3> gridPath; ///< The path to follow
	Color color;                ///< The color of the enemy

	/**
	 * @brief Draws the enemy health.
	 */
	void drawHealth() const;
};

} // namespace TowerDefense

#endif // INCLUDE_ENEMY_BASE_HPP_
