#ifndef INCLUDE_INCLUDE_TYPES_HPP_
#define INCLUDE_INCLUDE_TYPES_HPP_

#include <cstddef>
#include <cstdint>

using f80 = long double;
using f32 = float;
using f64 = double;

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using usize = size_t;

namespace Random {

i32 i32(const i32 min, const i32 max);
i64 i64(const i64 min, const i64 max);

u32 u32(const u32 min, const u32 max);
u64 u64(const u64 min, const u64 max);

f32 f32(const f32 min, const f32 max);
f64 f64(const f64 min, const f64 max);

} // namespace Random

#endif // INCLUDE_INCLUDE_TYPES_HPP_
