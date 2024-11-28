#ifndef INCLUDE_MATH_CONSTANTS_HPP_
#define INCLUDE_MATH_CONSTANTS_HPP_

#include "types.hpp"

#include <GL/gl.h>

namespace Math {

constexpr double PId  = 3.14159265358979323846;
constexpr GLfloat PIf = f32(PId);

double radiansToDegrees(const double radians);
double degreesToRadians(const double degrees);

} // namespace Math

#endif // INCLUDE_MATH_CONSTANTS_HPP_
