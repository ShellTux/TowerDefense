#ifndef INCLUDE_INCLUDE_COLOR_HPP_
#define INCLUDE_INCLUDE_COLOR_HPP_

#include <array>
#include <cstdint>

using Color = std::array<uint8_t, 4>;

namespace Colors {

constexpr Color RED   = {255, 0, 0, 255};
constexpr Color GREEN = {0, 255, 0, 255};
constexpr Color BLUE  = {0, 0, 255, 255};

} // namespace Colors

#endif // INCLUDE_INCLUDE_COLOR_HPP_
