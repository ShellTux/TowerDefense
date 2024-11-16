#ifndef INCLUDE_CANNON_BASE_HPP_
#define INCLUDE_CANNON_BASE_HPP_

#include "TowerDefense/Enemy/Base.hpp"
#include "TowerDefense/Vec3.hpp"

#include <cstdint>
#include <optional>
#include <ostream>
#include <vector>

namespace TowerDefense {

/**
 * @class Cannon
 * @brief A class representing a cannon that can target and shoot enemies.
 *
 * The `Cannon` class encapsulates the properties and behaviors of a cannon in a game.
 * It includes attributes such as angle, range, cooldown time, shot damage, and position.
 */
class Cannon {
      public:
	/**
	 * @brief Default constructor that initializes the cannon with default values.
	 *
	 * The default values are:
	 * - angle: 0
	 * - range: 3
	 * - cooldown: 10
	 * - shotDamage: 2
	 * - position: (0, 0, 0)
	 */
	Cannon()
	    : angle(0)
	    , range(3)
	    , defaultCooldown(10)
	    , cooldown(defaultCooldown)
	    , shotDamage(2)
	    , position(0, 0, 0)
	{}

	Cannon(const Vec3 &position)
	    : Cannon()
	{
		this->position = position;
	}

	/**
	 * @brief Parameterized constructor that initializes the cannon with specified values.
	 *
	 * @param angle The angle of the cannon in degrees.
	 * @param range The effective range of the cannon.
	 * @param cooldown The cooldown time between shots in arbitrary time units.
	 * @param shotDamage The amount of damage dealt by each shot.
	 * @param position A `Vec3` object representing the position of the cannon grid space.
	 */
	Cannon(const double angle,
	       const double range,
	       const uint8_t cooldown,
	       const uint8_t shotDamage,
	       const Vec3 position)
	    : angle(angle)
	    , range(range)
	    , defaultCooldown(cooldown)
	    , cooldown(defaultCooldown)
	    , shotDamage(shotDamage)
	    , position(position)
	{}
	Cannon(Cannon &&)                 = default;
	Cannon(const Cannon &)            = default;
	Cannon &operator=(Cannon &&)      = default;
	Cannon &operator=(const Cannon &) = default;
	~Cannon()                         = default;

	/**
	 * @brief Overloads the output stream operator for easy printing of Cannon details.
	 *
	 * @param os The output stream to write to.
	 * @param cannon The Cannon object to retrieve details from.
	 * @return The output stream after writing the cannon details.
	 */
	friend std::ostream &operator<<(std::ostream &os, const Cannon &cannon)
	{
		os << "Cannon: " << cannon.position
		   << ", angle: " << cannon.angle
		   << ", cooldown: " << static_cast<int>(cannon.cooldown)
		   << ", shotDamage: " << static_cast<int>(cannon.shotDamage);
		return os;
	}

	/**
	 * @brief Draws the cannon at its position in a grid.
	 */
	void draw() const;

	/**
	 * @brief Updates the state of the cannon (e.g., cooldown, targeting, etc.).
	 */
	void update(const std::vector<Enemy> &enemies);

	/**
	 * @brief Shoots at a specified enemy.
	 *
	 * @param target A reference to the `Enemy` object that is the target.
	 */
	void shot(Enemy &target) const;

	/**
	 * @brief Upgrades the cannon attributes.
	 *
	 * This function modifies the cannon's attributes to enhance its capabilities.
	 */
	void upgrade();

      private:
	double angle;            ///< The angle of the cannon in degrees.
	double range;            ///< The effective range of the cannon.
	uint8_t defaultCooldown; ///< The default cooldown time between shots.
	uint8_t cooldown;        ///< The cooldown time between shots.
	uint8_t shotDamage;      ///< The damage dealt by each shot.
	Vec3 position;           ///< The position of the cannon in 3D space.

	/**
	 * @brief Update Angle based on target enemy position.
	 *
	 * @param target The Enemy to target.
	 */
	void updateAngle(const Enemy &target);

	/**
	 * @brief Targets an enemy based on the given list of enemies.
	 *
	 * @param enemies A vector of `Enemy` objects to target.
	 */
	[[nodiscard]] std::optional<Enemy *>
	targetEnemy(const std::vector<Enemy> &enemies) const;
};

} // namespace TowerDefense

#endif // INCLUDE_CANNON_BASE_HPP_
