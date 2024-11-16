#include "Math.hpp"

namespace Math {
double radiansToDegrees(const double radians)
{
	return radians * (180.0 / PI);
}

double degreesToRadians(const double degrees)
{
	return degrees * (PI / 180.0);
}

} // namespace Math
