#ifndef INCLUDE_TOWERDEFENSE_TOWER_HPP_
#define INCLUDE_TOWERDEFENSE_TOWER_HPP_

#include "Vec3.hpp"

#include <cstdint>
#include <ostream>

namespace TowerDefense {

class Tower {
      public:
	Tower()
	    : gridPosition(0, 0, 0)
	    , health(10)
	{}

	Tower(const Vec3 &gridPosition)
	    : Tower()
	{
		this->gridPosition = gridPosition;
	}

	Tower(const Vec3 &gridPosition, const uint8_t health)
	    : gridPosition(gridPosition)
	    , health(health)
	{}

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

	[[nodiscard]] Vec3 getGridPosition() const;
	[[nodiscard]] uint8_t getHealth() const;

	uint8_t damage(const uint8_t damage);

	void draw() const;

      private:
	Vec3 gridPosition; ///< The position of the tower in the grid.
	uint8_t health;    ///< The health of the tower.

	void drawHealth() const;
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_TOWER_HPP_
