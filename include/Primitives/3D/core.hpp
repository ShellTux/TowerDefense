#ifndef INCLUDE_3D_CORE_HPP_
#define INCLUDE_3D_CORE_HPP_

#include "Primitives/2D/core.hpp"
#include "Vec3.hpp"
#include "types.hpp"

namespace Primitives3D {

namespace Unit {

static constexpr f64 p          = .5;
static constexpr f64 radius     = p;
static constexpr f64 resolution = Primitives2D::resolution;

void Cylinder();
void Cube();

} // namespace Unit

void Cube(const f32 x, const f32 y, const f32 z);
void Cube(const Vec3 &pos);

} // namespace Primitives3D

#endif // INCLUDE_3D_CORE_HPP_
