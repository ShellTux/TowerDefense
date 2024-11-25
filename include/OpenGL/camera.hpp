#ifndef INCLUDE_OPENGL_CAMERA_HPP_
#define INCLUDE_OPENGL_CAMERA_HPP_

#include "Vec3.hpp"
#include "types.hpp"

namespace OpenGL {

void Perspective(const f32 theta, const f32 alpha, const f32 beta);

namespace Camera {
void LookAt(const Vec3 &camera, const Vec3 &target, const Vec3 &up);
}

} // namespace OpenGL

#endif // INCLUDE_OPENGL_CAMERA_HPP_
