#ifndef INCLUDE_OPENGL_CAMERA_HPP_
#define INCLUDE_OPENGL_CAMERA_HPP_

#include "Vec3.hpp"

#include <GL/gl.h>

using TowerDefense::Vec3;

namespace OpenGL {

void Perspective(GLfloat theta, GLfloat alpha, GLfloat beta);

namespace Camera {
void LookAt(const Vec3 &camera, const Vec3 &target, const Vec3 &up);
}

} // namespace OpenGL

#endif // INCLUDE_OPENGL_CAMERA_HPP_
