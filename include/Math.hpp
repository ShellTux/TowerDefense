#ifndef INCLUDE_MATH_CONSTANTS_HPP_
#define INCLUDE_MATH_CONSTANTS_HPP_

#include "types.hpp"

#include <GL/gl.h>

namespace Math {

constexpr f64 PI  = 3.14159265358979323846;
constexpr f32 PIf = 3.14159265358979323846f;

f64 radiansToDegrees(const f64 radians);
f64 degreesToRadians(const f64 degrees);

} // namespace Math

#endif // INCLUDE_MATH_CONSTANTS_HPP_
