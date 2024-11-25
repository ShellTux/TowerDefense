#ifndef INCLUDE_TOWERDEFENSE_TOWER_HPP_
#define INCLUDE_TOWERDEFENSE_TOWER_HPP_

#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"

#include <ostream>

namespace TowerDefense {

class Tower {
      public:
	Tower() = default;
	Tower(const Vec3 &gridPosition)
	    : gridPosition(gridPosition)
	{}

	Tower(const Vec3 &gridPosition, const Stats::HealthPoints health)
	    : Tower(gridPosition)
	{
		this->health = health;
	}

	Tower(Tower &&)                 = default;
	Tower(const Tower &)            = default;
	Tower &operator=(Tower &&)      = default;
	Tower &operator=(const Tower &) = default;
	~Tower()                        = default;

	friend std::ostream &operator<<(std::ostream &os, const Tower &tower)
	{
		os << "Tower: " << tower.getGridPosition()
		   << ", health: " << static_cast<int>(tower.getHealth());
		return os;
	}

	void draw() const;

	[[nodiscard]] Stats::HealthPoints getHealth() const;
	[[nodiscard]] Vec3 getGridPosition() const;

	Stats::HealthPoints damage(const Stats::HealthPoints damage);

      private:
	Vec3 gridPosition;
	Stats::HealthPoints health{100};

	void drawHealth() const;
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_TOWER_HPP_
