#ifndef INCLUDE_TOWERDEFENSE_TOWER_HPP_
#define INCLUDE_TOWERDEFENSE_TOWER_HPP_

#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
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

	friend std::ostream &operator<<(std::ostream &os, const Tower &tower);

	void draw() const;

	[[nodiscard]] f64 getHealthRatio() const;
	[[nodiscard]] Stats::HealthPoints getFullHealth() const;
	[[nodiscard]] Stats::HealthPoints getHealth() const;
	[[nodiscard]] Vec3 getGridPosition() const;

	Stats::HealthPoints damage(const Stats::HealthPoints damage);

      private:
	Vec3 gridPosition;
	Stats::HealthPoints health{100};
	Stats::HealthPoints fullHealth{100};

	static constexpr GLbitfield drawGlMask
	    = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT
	      | GL_POLYGON_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	      | GL_VIEWPORT_BIT;

	void drawHealth() const;
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_TOWER_HPP_
