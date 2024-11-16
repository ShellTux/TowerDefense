#ifndef INCLUDE_3D_CORE_HPP_
#define INCLUDE_3D_CORE_HPP_

#include <cstdint>

namespace Primitives3D {

namespace Unit {

void Cube();
void Grid(const uint32_t rows, const uint32_t cols);

} // namespace Unit

inline void Cube(/* TODO: Args */);

} // namespace Primitives3D

#endif // INCLUDE_3D_CORE_HPP_
