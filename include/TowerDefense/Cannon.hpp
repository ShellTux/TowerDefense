#ifndef INCLUDE_CANNON_BASE_HPP_
#define INCLUDE_CANNON_BASE_HPP_

#include "Color.hpp"
#include "OpenGL/Material.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>
#include <functional>
#include <optional>
#include <ostream>
#include <vector>

namespace TowerDefense {

class Cannon {
      public:
	using EnemyR = std::optional<std::reference_wrapper<Enemy>>;

	Cannon(const Stats::Tier &tier, const Vec3 &gridPosition)
	    : gridPosition(gridPosition)
	    , currentTier(tier)
	{
		updateStats();
	}

	Cannon(Cannon &&)                 = default;
	Cannon(const Cannon &)            = default;
	Cannon &operator=(Cannon &&)      = default;
	Cannon &operator=(const Cannon &) = default;
	~Cannon()                         = default;

	friend std::ostream &operator<<(std::ostream &os, const Cannon &cannon);
	friend std::ostream &operator<<(std::ostream &os,
	                                const std::optional<Cannon> &cannonOpt);

	void draw(const Vec3 &selectedGridPosition) const;
	void update(const Stats::TimeMs deltaTimeMs,
	            const std::vector<Enemy> &enemies);
	void shot(Enemy &target);
	bool upgrade();

	[[nodiscard]] Vec3 getGridPosition() const;

      private:
	Color color{Colors::BLACK};
	f64 angle{};
	Vec3 gridPosition;
	OpenGL::Material::Type material{};

	Stats::TimeMs cooldownMs{};
	Stats::TimeMs defaultCooldownMs{};
	Stats::HealthPoints shotDamage{};
	Stats::Range range{};

	Stats::Level currentLevel{Stats::Level::L1};
	Stats::Tier currentTier{Stats::Tier::A};

	static constexpr GLbitfield drawGlMask
	    = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT
	      | GL_POLYGON_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	      | GL_VIEWPORT_BIT;

	void drawRange(const Vec3 &selectedGridPosition) const;
	void drawShot() const;
	void updateAngle(const Enemy &target);
	void updateStats();

	[[nodiscard]] EnemyR
	targetEnemy(const std::vector<Enemy> &enemies) const;
};

} // namespace TowerDefense

#endif // INCLUDE_CANNON_BASE_HPP_
