#include "Math.hpp"

#include "types.hpp"

namespace Math {
f64 radiansToDegrees(const f64 radians)
{
	return radians * (180.0 / PId);
}

f64 degreesToRadians(const f64 degrees)
{
	return degrees * (PId / 180.0);
}

} // namespace Math
