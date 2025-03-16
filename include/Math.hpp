#ifndef INCLUDE_MATH_CONSTANTS_HPP_
#define INCLUDE_MATH_CONSTANTS_HPP_

#include "types.hpp"

#ifndef NOOF
	#include "ofMain.h"
#endif

#include <GL/gl.h>

namespace Math {

constexpr f64 PId = 3.14159265358979323846;
constexpr f32 PIf = f32(PId);

f64 R2D(const f64 radians);
f64 D2R(const f64 degrees);

} // namespace Math

#endif // INCLUDE_MATH_CONSTANTS_HPP_
