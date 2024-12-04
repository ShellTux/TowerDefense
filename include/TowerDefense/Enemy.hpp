#ifndef INCLUDE_ENEMY_BASE_HPP_
#define INCLUDE_ENEMY_BASE_HPP_

#include "Color.hpp"
#include "OpenGL/Material.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <optional>
#include <ostream>
#include <vector>

namespace TowerDefense {

class Enemy {
      public:
	struct PathInfo {
		Vec3 start;
		Vec3 pos;
		Vec3 next;
		f64 ratio;

		Vec3 direction;
		f64 angle;
	};

	struct LookAt {
		Vec3 camera;
		Vec3 target;
		Vec3 up;
	};

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

	friend std::ostream &operator<<(std::ostream &os, const Enemy &enemy);
	friend std::ostream &operator<<(std::ostream &os,
	                                const std::optional<Enemy> &enemy);

	void draw() const;
	void update(const Stats::TimeMs deltaTimeMs);

	Stats::HealthPoints loseHP(const Stats::HealthPoints damagePoints);

	[[nodiscard]] f64 getHealthRatio() const;
	[[nodiscard]] f64 getPosition() const;
	[[nodiscard]] Stats::HealthPoints getHealth() const;
	[[nodiscard]] Stats::HealthPoints getPoints() const;
	[[nodiscard]] Stats::Speed getSpeedUpMs() const;
	[[nodiscard]] struct LookAt getLookAt() const;
	[[nodiscard]] struct PathInfo getPathInfo() const;

      private:
	Color color{Colors::BLACK};
	f64 position{};
	Stats::HealthPoints fullHealth{10};
	Stats::HealthPoints health{10};
	Stats::Speed speedUpMs{};
	std::vector<Vec3> gridPath;
	struct PathInfo pathInfo {};
	struct LookAt lookAt {};

	OpenGL::Material::Type material{};

	static constexpr GLbitfield drawGlMask
	    = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT
	      | GL_POLYGON_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	      | GL_VIEWPORT_BIT;

	void drawHealth() const;
	void updateStats(const Stats::Tier &tier);

	Enemy &updatePathAndLookAt();
};

} // namespace TowerDefense

#endif // INCLUDE_ENEMY_BASE_HPP_
