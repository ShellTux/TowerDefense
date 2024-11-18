#ifndef INCLUDE_CANNON_BASE_HPP_
#define INCLUDE_CANNON_BASE_HPP_

#include "Color.hpp"
#include "TowerDefense/Enemy/Base.hpp"
#include "Vec3.hpp"

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
	enum class Type { TierA, TierB, TierC };

	Cannon(const Type &type, const Vec3 &gridPosition)
	    : angle(0)
	    , cooldown(0)
	    , color(Colors::BLACK)
	{
		switch (type) {
		case Type::TierA: {
			color           = Colors::RED;
			range           = 2;
			shotDamage      = 1;
			defaultCooldown = 1 * 60;
		} break;
		case Type::TierB: {
			color           = Colors::ORANGE;
			range           = 4;
			shotDamage      = 2;
			defaultCooldown = 5 * 60;
		} break;
		case Type::TierC: {
			color           = Colors::PURPLE;
			range           = 3;
			shotDamage      = 3;
			defaultCooldown = 10 * 60;
		} break;
		}

		this->gridPosition = gridPosition;
	}

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
		os << "Cannon: " << cannon.gridPosition
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
	double angle;             ///< The angle of the cannon in degrees.
	double range;             ///< The effective range of the cannon.
	uint32_t defaultCooldown; ///< The default cooldown time between shots.
	uint32_t cooldown;        ///< The cooldown time between shots.
	uint8_t shotDamage;       ///< The damage dealt by each shot.
	Vec3 gridPosition;        ///< The position of the cannon in 3D space.
	Color color;

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
