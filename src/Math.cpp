#include "Math.hpp"

namespace Math {
double radiansToDegrees(const double radians)
{
	return radians * (180.0 / PId);
}

double degreesToRadians(const double degrees)
{
	return degrees * (PId / 180.0);
}

} // namespace Math
