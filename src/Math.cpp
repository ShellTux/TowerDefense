#include "Math.hpp"

#include "types.hpp"

namespace Math {
f64 radiansToDegrees(const f64 radians)
{
	return radians * (180.0 / PI);
}

f64 degreesToRadians(const f64 degrees)
{
	return degrees * (PI / 180.0);
}

} // namespace Math
