#ifndef INCLUDE_ENEMY_BASE_HPP_
#define INCLUDE_ENEMY_BASE_HPP_

#include "Color.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <cassert>
#include <cstdint>
#include <ostream>
#include <tuple>
#include <vector>

namespace TowerDefense {

class Enemy {
      public:
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

	friend std::ostream &operator<<(std::ostream &os, const Enemy &enemy)
	{
		os << "Enemy: " << enemy.getPosition()
		   << ", speed: " << enemy.getSpeedUpMs()
		   << ", health: " << static_cast<int>(enemy.getHealth())
		   << ", points: " << static_cast<int>(enemy.getPoints());
		return os;
	}

	[[nodiscard]] double getPosition() const;
	[[nodiscard]] double getSpeedUpMs() const;
	[[nodiscard]] uint8_t getHealth() const;
	[[nodiscard]] uint8_t getPoints() const;
	void draw() const;

	void update(const Stats::CooldownMs deltaTimeMs);
	Stats::HealthPoints loseHP(const Stats::HealthPoints damagePoints);

	[[nodiscard]] Vec3 getGridPosition() const;
	[[nodiscard]] Vec3 getNextGridPosition() const;

	[[nodiscard]] std::tuple<Vec3, Vec3, Vec3> getLookAt() const;

      private:
	void drawHealth() const;

	void updateStats(const Stats::Tier &tier);

	[[nodiscard]] std::tuple<Vec3, Vec3, f64>
	getInterpolatingGridPositions() const;
};

} // namespace TowerDefense

#endif // INCLUDE_ENEMY_BASE_HPP_
