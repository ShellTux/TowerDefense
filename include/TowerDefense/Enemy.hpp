#ifndef INCLUDE_ENEMY_BASE_HPP_
#define INCLUDE_ENEMY_BASE_HPP_

#include "Color.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <ostream>
#include <tuple>
#include <vector>

namespace TowerDefense {

class Enemy {
      public:
	Enemy(const Stats::Tier &tier, const std::vector<Vec3> &gridPath)
	    : gridPath(gridPath)
	{
		updateStats(tier);
	}

	Enemy(const Stats::Tier &tier,
	      const std::vector<Vec3> &gridPath,
	      const f64 position)
	    : Enemy(tier, gridPath)
	{
		this->position = position;
	}

	static Enemy Random(const std::vector<Vec3> &path,
	                    const f64 position = 0);

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

	void draw() const;
	void update(const Stats::TimeMs deltaTimeMs);

	Stats::HealthPoints loseHP(const Stats::HealthPoints damagePoints);

	[[nodiscard]] f64 getPosition() const;
	[[nodiscard]] Stats::HealthPoints getHealth() const;
	[[nodiscard]] Stats::HealthPoints getPoints() const;
	[[nodiscard]] Stats::Speed getSpeedUpMs() const;
	[[nodiscard]] std::tuple<Vec3, Vec3, Vec3> getLookAt() const;
	[[nodiscard]] Vec3 getGridPosition() const;
	[[nodiscard]] Vec3 getNextGridPosition() const;

      private:
	Color color{Colors::BLACK};
	f64 position{};
	Stats::HealthPoints fullHealth{10};
	Stats::HealthPoints health{10};
	Stats::Speed speedUpMs{};
	std::vector<Vec3> gridPath;

	void drawHealth() const;
	void updateStats(const Stats::Tier &tier);

	[[nodiscard]] std::tuple<Vec3, Vec3, f64>
	getInterpolatingGridPositions() const;
};

} // namespace TowerDefense

#endif // INCLUDE_ENEMY_BASE_HPP_
