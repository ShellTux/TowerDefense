#ifndef INCLUDE_CANNON_BASE_HPP_
#define INCLUDE_CANNON_BASE_HPP_

#include "Color.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"

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
	Cannon(const Stats::Tier &tier, const Vec3 &gridPosition)
	    : angle(0)
	    , cooldownMs(0)
	    , gridPosition(gridPosition)
	    , color(Colors::BLACK)
	    , currentLevel(Stats::Level::L1)
	    , currentTier(tier)
	{
		updateStats();
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
		   << ", cooldown: " << static_cast<int>(cannon.cooldownMs)
		   << ", shotDamage: " << static_cast<int>(cannon.shotDamage)
		   << ", " << cannon.currentTier << ", " << cannon.currentLevel;
		return os;
	}

	friend std::ostream &operator<<(std::ostream &os,
	                                const std::optional<Cannon> &cannonOpt)
	{
		if (!cannonOpt.has_value()) {
			os << "No Cannon";
			return os;
		}

		os << cannonOpt.value();
		return os;
	}

	/**
	 * @brief Draws the cannon at its position in a grid.
	 */
	void draw(const Vec3 &selectedGridPosition) const;

	/**
	 * @brief Updates the state of the cannon (e.g., cooldown, targeting, etc.).
	 */
	void update(const Stats::CooldownMs deltaTimeMs,
	            const std::vector<Enemy> &enemies);

	/**
	 * @brief Shoots at a specified enemy.
	 *
	 * @param target A reference to the `Enemy` object that is the target.
	 */
	void shot(Enemy &target);

	/**
	 * @brief Upgrades the cannon attributes.
	 *
	 * This function modifies the cannon's attributes to enhance its capabilities.
	 */
	void upgrade();

	[[nodiscard]] Vec3 getGridPosition() const;

      private:
	double angle;       ///< The angle of the cannon in degrees.
	Stats::Range range; ///< The effective range of the cannon.
	Stats::CooldownMs defaultCooldownMs; ///< The default cooldown time.
	Stats::CooldownMs cooldownMs;   ///< The cooldown time between shots.
	Stats::HealthPoints shotDamage; ///< The damage dealt by each shot.
	Vec3 gridPosition; ///< The position of the cannon in 3D space.
	Color color;
	Stats::Level currentLevel;
	Stats::Tier currentTier;

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

	void drawRange(const Vec3 &selectedGridPosition) const;
	void drawShot() const;
	void updateStats();
};

} // namespace TowerDefense

#endif // INCLUDE_CANNON_BASE_HPP_
