#include "TowerDefense/Stats.hpp"

#include "Color.hpp"

#include <ostream>

namespace TowerDefense::Stats {

std::ostream &operator<<(std::ostream &os, const Tier &tier)
{
	switch (tier) {
	case Tier::A: {
		os << "Tier A";
	} break;
	case Tier::B: {
		os << "Tier B";
	} break;
	case Tier::C: {
		os << "Tier C";
	} break;
	}

	return os;
}

std::ostream &operator<<(std::ostream &os, const Level &level)
{
	switch (level) {
	case Level::L1: {
		os << "Level 1";
	} break;
	case Level::L2: {
		os << "Level 2";
	} break;
	case Level::L3: {
		os << "Level 3";
	} break;
	}

	return os;
}

CannonStats Cannon::Get(const Tier &tier, const Level &level)
{
	switch (tier) {
	case Tier::A: {
		switch (level) {
		case Level::L1: {
			return {
			    .color      = Colors::RED,
			    .cooldownMs = 500,
			    .damage     = 1,
			    .range      = 1.5,
			    .cost       = 50,
			};
		} break;
		case Level::L2: {
			return {
			    .color      = Colors::RED,
			    .cooldownMs = 500,
			    .damage     = 4,
			    .range      = 1.75,
			    .cost       = 50,
			};
		} break;
		case Level::L3: {
			return {
			    .color      = Colors::RED,
			    .cooldownMs = 500,
			    .damage     = 8,
			    .range      = 2,
			    .cost       = 50,
			};
		} break;
		}
	} break;
	case Tier::B: {
		switch (level) {
		case Level::L1: {
			return {
			    .color      = Colors::ORANGE,
			    .cooldownMs = 2500,
			    .damage     = 8,
			    .range      = 3.75,
			    .cost       = 125,
			};
		} break;
		case Level::L2: {
			return {
			    .color      = Colors::ORANGE,
			    .cooldownMs = 1750,
			    .damage     = 10,
			    .range      = 4.16,
			    .cost       = 125,
			};
		} break;
		case Level::L3: {
			return {
			    .color      = Colors::ORANGE,
			    .cooldownMs = 1000,
			    .damage     = 12,
			    .range      = 4.58,
			    .cost       = 125,
			};
		} break;
		}
	} break;
	case Tier::C: {
		switch (level) {
		case Level::L1: {
			return {
			    .color      = Colors::PURPLE,
			    .cooldownMs = 4000,
			    .damage     = 40,
			    .range      = 1.5,
			    .cost       = 250,
			};
		} break;
		case Level::L2: {
			return {
			    .color      = Colors::PURPLE,
			    .cooldownMs = 3500,
			    .damage     = 70,
			    .range      = 1.5,
			    .cost       = 250,
			};
		} break;
		case Level::L3: {
			return {
			    .color      = Colors::PURPLE,
			    .cooldownMs = 3000,
			    .damage     = 90,
			    .range      = 1.5,
			    .cost       = 250,
			};
		} break;
		}
	} break;
	}

	return {};
}

EnemyStats Enemy::Get(const Tier &tier)
{
	const CannonStats equivalentCannonStats = Cannon::Get(tier, Level::L1);

	switch (tier) {
	case Tier::A: {
		return {
		    .color  = Colors::RED,
		    .health = static_cast<HealthPoints>(
		        equivalentCannonStats.damage * 1.5),
		    .speedUpMs = 1e-5,
		};
	} break;
	case Tier::B: {
		return {
		    .color  = Colors::ORANGE,
		    .health = static_cast<HealthPoints>(
		        equivalentCannonStats.damage * 1.5),
		    .speedUpMs = 1e-5,
		};
	} break;
	case Tier::C: {
		return {
		    .color  = Colors::PURPLE,
		    .health = static_cast<HealthPoints>(
		        equivalentCannonStats.damage * 1.5),
		    .speedUpMs = 1e-5,
		};
	} break;
	}

	return {};
}

} // namespace TowerDefense::Stats
