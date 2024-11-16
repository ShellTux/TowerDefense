#ifndef INCLUDE_MATH_CONSTANTS_HPP_
#define INCLUDE_MATH_CONSTANTS_HPP_

#include <GL/gl.h>

namespace Math {

constexpr double PI   = 3.14159265358979323846;
constexpr GLfloat PIf = 3.14159265358979323846f;

double radiansToDegrees(const double radians);
double degreesToRadians(const double degrees);

} // namespace Math

#endif // INCLUDE_MATH_CONSTANTS_HPP_
