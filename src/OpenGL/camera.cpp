#include "OpenGL/camera.hpp"

#include <GL/gl.h>
#include <cmath>

namespace OpenGL {

void Perspective(GLfloat theta, GLfloat alpha, GLfloat beta)
{
	(void) theta;
	(void) alpha;
	(void) beta;

	/*glMatrixMode(GL_PROJECTION);*/
	/*glLoadIdentity();*/
	/*GLfloat tg       = tan(theta * 0.5 * PI / 180.0);*/
	/*GLfloat d        = (gh() / 2.0) / tan;*/
	/*GLfloat nearClip = d / alpha;*/
	/*GLfloat farClip  = d * beta;*/
	/*GLfloat ymax     = nearClip * tan;*/
	/*GLfloat xmax     = (gw() / gh()) * ymax;*/
	/*glFrustum(-xmax, xmax, -ymax, ymax, nearClip, farClip);*/
}

namespace Camera {
void LookAt(const Vec3 &camera, const Vec3 &target, const Vec3 &up)
{
	const Vec3 N = Vec3::Normalize(camera - target);
	const Vec3 U = Vec3::Normalize(up.cross(N));
	const Vec3 V = Vec3::Normalize(N.cross(U));

	const auto [Ux, Uy, Uz] = U.getCoordinates();
	const auto [Vx, Vy, Vz] = V.getCoordinates();
	const auto [Nx, Ny, Nz] = N.getCoordinates();

	const double camTransformMatrix[4][4] = {
	    {            Ux,             Vx,             Nx, 0},
	    {            Uy,             Vy,             Ny, 0},
	    {            Uz,             Vz,             Nz, 0},
	    {-U.dot(camera), -V.dot(camera), -N.dot(camera), 1}
        };


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(&camTransformMatrix[0][0]);
}
} // namespace Camera

} // namespace OpenGL
