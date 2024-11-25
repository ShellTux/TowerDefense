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

	void draw(const Vec3 &selectedGridPosition) const;
	void update(const Stats::CooldownMs deltaTimeMs,
	            const std::vector<Enemy> &enemies);
	void shot(Enemy &target);
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

	void updateAngle(const Enemy &target);

	[[nodiscard]] std::optional<Enemy *>
	targetEnemy(const std::vector<Enemy> &enemies) const;

	void drawRange(const Vec3 &selectedGridPosition) const;
	void drawShot() const;
	void updateStats();
};

} // namespace TowerDefense

#endif // INCLUDE_CANNON_BASE_HPP_
