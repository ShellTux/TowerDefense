#ifndef INCLUDE_INCLUDE_COLOR_HPP_
#define INCLUDE_INCLUDE_COLOR_HPP_

#include "types.hpp"

#include <array>
#include <ostream>

using Color = std::array<u8, 4>;

namespace Colors {

constexpr Color WHITE      = {255, 255, 255, 255};
constexpr Color BLACK      = {0, 0, 0, 255};
constexpr Color RED        = {255, 0, 0, 255};
constexpr Color GREEN      = {0, 255, 0, 255};
constexpr Color BLUE       = {0, 0, 255, 255};
constexpr Color YELLOW     = {255, 255, 0, 255};
constexpr Color CYAN       = {0, 255, 255, 255};
constexpr Color MAGENTA    = {255, 0, 255, 255};
constexpr Color GRAY       = {128, 128, 128, 255};
constexpr Color DARK_GRAY  = {64, 64, 64, 255};
constexpr Color LIGHT_GRAY = {192, 192, 192, 255};
constexpr Color ORANGE     = {255, 165, 0, 255};
constexpr Color PURPLE     = {128, 0, 128, 255};
constexpr Color PINK       = {255, 192, 203, 255};
constexpr Color BROWN      = {165, 42, 42, 255};

constexpr Color GOLD      = {255, 215, 0, 255};
constexpr Color SILVER    = {192, 192, 192, 255};
constexpr Color BRONZE    = {205, 127, 50, 255};
constexpr Color NAVY      = {0, 0, 128, 255};
constexpr Color TEAL      = {0, 128, 128, 255};
constexpr Color OLIVE     = {128, 128, 0, 255};
constexpr Color MAROON    = {128, 0, 0, 255};
constexpr Color CORAL     = {255, 127, 80, 255};
constexpr Color SALMON    = {250, 128, 114, 255};
constexpr Color LAVENDER  = {230, 230, 250, 255};
constexpr Color MINT      = {189, 252, 201, 255};
constexpr Color PEACH     = {255, 218, 185, 255};
constexpr Color AQUA      = {0, 255, 255, 255};
constexpr Color CHOCOLATE = {210, 105, 30, 255};
constexpr Color KHAKI     = {240, 230, 140, 255};
constexpr Color LIME      = {0, 255, 0, 255};
constexpr Color ENDER     = {255, 0, 255, 255};

std::ostream &operator<<(std::ostream &os, const Color &color);

} // namespace Colors

#endif // INCLUDE_INCLUDE_COLOR_HPP_
