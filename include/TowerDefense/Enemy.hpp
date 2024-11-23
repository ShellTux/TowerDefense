#ifndef INCLUDE_ENEMY_BASE_HPP_
#define INCLUDE_ENEMY_BASE_HPP_

#include "Color.hpp"
#include "TowerDefense/Stats.hpp"
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
	/**
	 * @brief Default constructs an Enemy with initial position, speed, health, and points.
	 */
	Enemy(const Stats::Tier &tier, const std::vector<Vec3> &gridPath)
	    : position(0)
	    , gridPath(gridPath)
	    , color(Colors::RED)
	{
		updateStats(tier);
	}

	Enemy(const Stats::Tier &tier,
	      const std::vector<Vec3> &gridPath,
	      const double position)
	    : Enemy(tier, gridPath)
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
		   << ", speed: " << enemy.getSpeedUpMs()
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
	[[nodiscard]] double getSpeedUpMs() const;

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
	void update(const Stats::CooldownMs deltaTimeMs);

	/**
	 * @brief Decreases the enemy's health by a specified amount.
	 *
	 * @param healthPoints The amount of health points to lose.
	 * @return The remaining health points after loss.
	 */
	Stats::HealthPoints loseHP(const Stats::HealthPoints damagePoints);

	/**
	 * @brief Gets position on the grid based on unidimensional position on the path.
	 *
	 * @return The converted Vec3 position.
	 */
	[[nodiscard]] Vec3 getGridPosition() const;

      private:
	double position; ///< The enemy's current unidimensional position.
	Stats::Speed speedUpMs;     ///< The speed at which the enemy moves.
	Stats::HealthPoints health; ///< The current health of the enemy.
	Stats::HealthPoints fullHealth;
	std::vector<Vec3> gridPath; ///< The path to follow
	Color color;                ///< The color of the enemy

	/**
	 * @brief Draws the enemy health.
	 */
	void drawHealth() const;

	void updateStats(const Stats::Tier &tier);
};

} // namespace TowerDefense

#endif // INCLUDE_ENEMY_BASE_HPP_
