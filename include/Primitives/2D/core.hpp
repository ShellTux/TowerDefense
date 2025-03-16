#ifndef INCLUDE_2D_CORE_HPP_
#define INCLUDE_2D_CORE_HPP_

#include "types.hpp"

namespace Primitives2D {

static constexpr f64 p          = .5;
static constexpr u64 resolution = 50;

namespace Unit {

void Circle();
void Square();
void Grid(const u32 rows, const u32 cols);

} // namespace Unit

} // namespace Primitives2D

#endif // INCLUDE_2D_CORE_HPP_
