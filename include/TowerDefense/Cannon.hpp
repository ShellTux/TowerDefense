#ifndef INCLUDE_CANNON_BASE_HPP_
#define INCLUDE_CANNON_BASE_HPP_

#include "Color.hpp"
#include "OpenGL/Material.hpp"
#include "Texture.hpp"
#include "TowerDefense/Enemy.hpp"
#include "TowerDefense/Stats.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <optional>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace TowerDefense {

class Cannon {
      public:
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

	std::unordered_map<Stats::Tier, Texture> textures{
	    {Stats::Tier::A,    Texture("assets/red-cannon.png")},
	    {Stats::Tier::B, Texture("assets/orange-cannon.png")},
	    {Stats::Tier::C, Texture("assets/purple-cannon.png")},
	};

	static constexpr GLbitfield drawGlMask
	    = GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT
	      | GL_POLYGON_BIT | GL_TEXTURE_BIT | GL_TRANSFORM_BIT
	      | GL_VIEWPORT_BIT;

	void drawRange(const Vec3 &selectedGridPosition) const;
	void drawShot() const;
	void updateAngle(const Enemy &target);
	void updateStats();

	[[nodiscard]] std::optional<Enemy *>
	targetEnemy(const std::vector<Enemy> &enemies) const;
};

} // namespace TowerDefense

#endif // INCLUDE_CANNON_BASE_HPP_
