#include "types.hpp"

#include <random>

namespace Random {

std::random_device rd;
std::mt19937 engine(rd());

::i32 i32(const ::i32 min, const ::i32 max)
{
	std::uniform_int_distribution<::i32> distribution(min, max);
	return distribution(engine);
}

::i64 i64(const ::i64 min, const ::i64 max)
{
	std::uniform_int_distribution<::i64> distribution(min, max);
	return distribution(engine);
}

::u32 u32(const ::u32 min, const ::u32 max)
{
	std::uniform_int_distribution<::u32> distribution(min, max);
	return distribution(engine);
}

::u64 u64(const ::u64 min, const ::u64 max)
{
	std::uniform_int_distribution<::u64> distribution(min, max);
	return distribution(engine);
}

::f32 f32(const ::f32 min, const ::f32 max)
{
	std::uniform_real_distribution<::f32> distribution(min, max);
	return distribution(engine);
}

::f64 f64(const ::f64 min, const ::f64 max)
{
	std::uniform_real_distribution<::f64> distribution(min, max);
	return distribution(engine);
}

} // namespace Random
