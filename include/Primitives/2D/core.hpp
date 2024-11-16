#ifndef INCLUDE_2D_CORE_HPP_
#define INCLUDE_2D_CORE_HPP_

#include <cstdint>

namespace Primitives2D {
namespace Unit {

void Circle(const uint32_t resolution);
void Square();
void Grid(const uint32_t rows, const uint32_t cols);

} // namespace Unit

} // namespace Primitives2D

#endif // INCLUDE_2D_CORE_HPP_
