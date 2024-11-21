#ifndef INCLUDE_TOWERDEFENSE_STATS_HPP_
#define INCLUDE_TOWERDEFENSE_STATS_HPP_

#include "Color.hpp"
#include "types.hpp"

#include <ostream>

namespace TowerDefense::Stats {

enum class Tier { A, B, C };
enum class Level { L1, L2, L3 };

std::ostream &operator<<(std::ostream &os, const Tier &tier);
std::ostream &operator<<(std::ostream &os, const Level &level);

using CooldownMs   = u32;
using HealthPoints = u32;
using Range        = f32;
using Speed        = f32;

struct CannonStats {
	Color color;
	CooldownMs cooldownMs;
	HealthPoints damage;
	Range range;
	HealthPoints cost;
};

struct EnemyStats {
	Color color;
	HealthPoints health;
	Speed speedUpMs;
};

namespace Cannon {

CannonStats Get(const Tier &tier, const Level &level);

} // namespace Cannon

namespace Enemy {

EnemyStats Get(const Tier &tier);

} // namespace Enemy

} // namespace TowerDefense::Stats

#endif // INCLUDE_TOWERDEFENSE_STATS_HPP_
