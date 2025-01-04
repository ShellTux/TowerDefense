#include "OpenGL/Camera.hpp"

#include "Math.hpp"
#include "Vec3.hpp"
#include "types.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <ios>
#include <ostream>
#include <sstream>
#include <string>

namespace OpenGL {

Camera::Camera(const Vec3 &target,
               const f64 defaultRadius,
               const u32 width,
               const u32 height,
               const bool invertX,
               const bool invertY)
    : mTarget(target)
    , view(Front3rd)
    , up({0, 0, 1})
    , radius(defaultRadius)
    , azimuthalAngle(0)
    , polarAngle(0)
    , orbitAngle(0)
    , xmax(0)
    , ymax(0)
    , nearClip(-1)
    , farClip(1)
    , width(width)
    , height(height)
    , invertX(invertX)
    , invertY(invertY)
    , defaults({.target = target, .radius = radius, .view = view})
{
	// TODO: Update perspective parameters
	update();
}

Camera &Camera::operator+=(const usize i)
{
	for (usize j = 0; j < i; ++j) {
		switch (view) {
		case Front3rd: {
			view = Left3rd;
		} break;
		case Left3rd: {
			view = Back3rd;
		} break;
		case Back3rd: {
			view = Right3rd;
		} break;
		case Right3rd: {
			view = Above3rd;
		} break;
		case Above3rd: {
			view = Below3rd;
		} break;
		case Below3rd: {
			view = OrbitHorizontal3rd;
		} break;
		case OrbitHorizontal3rd: {
			view = OrbitVertical3rd;
		} break;
		case OrbitVertical3rd: {
			view = FreeView3rd;
		} break;
		case FreeView3rd: {
			view = FirstView;
		} break;
		case FirstView: {
			view = Front3rd;
		} break;
		}
	}

	update();

	return *this;
}

std::ostream &operator<<(std::ostream &os, const Camera &camera)
{
	os.precision(2);

	os << std::fixed;
	os << "󰓾 " << camera.mTarget << " ; ";

	os << "r: " << camera.radius << " (";
	os << "φ: " << Math::R2D(camera.azimuthalAngle) << ", ";
	os << "ϑ: " << Math::R2D(camera.polarAngle) << ")" << " ; ";
	os << "↑ " << camera.up;
#ifdef DEBUG
	os << "; xmax: " << camera.xmax << ", ymax: " << camera.ymax;
#endif
	return os;
}

std::string Camera::toString() const {
    std::stringstream string;
    string << *this;
    return string.str();
}

void Camera::apply()
{
	perspective();
	lookAt();

	switch (view) {
	case Front3rd:
	case Left3rd:
	case Back3rd:
	case Right3rd:
	case Above3rd:
	case Below3rd:
	case FreeView3rd:
	case FirstView:
		break;
	case OrbitHorizontal3rd:
	case OrbitVertical3rd: {
		orbitAngle += .01;
		while (orbitAngle > 2 * Math::PId) {
			orbitAngle -= 2 * Math::PId;
		}
		while (orbitAngle < 0) {
			orbitAngle += 2 * Math::PId;
		}
		update();
	} break;
	}
}

void Camera::lookAt() const
{
	const Vec3 N = Vec3::Normalize(camera - mTarget);
	const Vec3 U = Vec3::Normalize(up.cross(N));
	const Vec3 V = Vec3::Normalize(N.cross(U));

	const auto &[Ux, Uy, Uz] = U.getCoordinates();
	const auto &[Vx, Vy, Vz] = V.getCoordinates();
	const auto &[Nx, Ny, Nz] = N.getCoordinates();

	const f64 camTransformMatrix[4][4] = {
	    {            Ux,             Vx,             Nx, 0},
	    {            Uy,             Vy,             Ny, 0},
	    {            Uz,             Vz,             Nz, 0},
	    {-U.dot(camera), -V.dot(camera), -N.dot(camera), 1}
        };


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(&camTransformMatrix[0][0]);
}

void Camera::perspective() const
{
	glPushAttrib(GL_TRANSFORM_BIT);
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		// WARN: ymax can be swapped
		glFrustum(-xmax, xmax, -ymax, ymax, nearClip, farClip);
	}
	glPopAttrib();
}

Camera &Camera::orbit(const f64 azimuthalAngleDelta, const f64 polarAngleDelta)
{
	azimuthalAngle += azimuthalAngleDelta;
	polarAngle     += polarAngleDelta;

	update();

	return *this;
}

Camera &Camera::perspective(const f32 theta, const f32 alpha, const f32 beta)
{
	using std::tan, Math::D2R;

#if 0
	const f64 tg = tan(D2R(theta * .5));
	const f64 d  = (height / 2.) / tg;

	nearClip = d / alpha;
	farClip  = d * beta;
	xmax     = ymax * f64(width) / f64(height);
	ymax     = nearClip * tg;

	if (invertX) {
		xmax *= -1;
	}
	if (invertY) {
		ymax *= -1;
	}
#else
	(void) width;
	(void) height;
	(void) invertX;
	(void) invertY;

	const f64 tg = tan(D2R(theta));
	const f64 d  = (1 / 2.) / tg;
	nearClip     = d / alpha;
	farClip      = d * beta;
	ymax         = nearClip * tg;
	xmax         = -ymax;
#endif


	return *this;
}

Camera &Camera::setRadius()
{
	return setRadius(defaults.radius);
}

Camera &Camera::setRadius(const f64 radius)
{
	this->radius = radius;

	update();

	return *this;
}

Camera &Camera::reset()
{
	view    = defaults.view;
	mTarget = defaults.target;
	radius  = defaults.radius;

	update();

	return *this;
}

Camera &Camera::setTarget()
{
	return setTarget(defaults.target);
}

Camera &Camera::setTarget(const Vec3 &target)
{
	mTarget = target;

	update();

	return *this;
}

Camera &Camera::setWidthHeight(const u32 width, const u32 height)
{
	this->width  = width;
	this->height = height;

	return *this;
}

Camera &Camera::zoomIn(const f64 radiusDelta)
{
	if (radius > radiusDelta) {
		radius -= radiusDelta;
	}

	update();

	return *this;
}

void Camera::update()
{
	static constexpr f64 quart = Math::PId * .5;

	switch (view) {
	case Front3rd: {
		azimuthalAngle = quart;
		polarAngle     = .5 * quart;
	} break;
	case Left3rd: {
		azimuthalAngle = 2 * quart;
		polarAngle     = .5 * quart;
	} break;
	case Back3rd: {
		azimuthalAngle = 3 * quart;
		polarAngle     = .5 * quart;
	} break;
	case Right3rd: {
		azimuthalAngle = 0;
		polarAngle     = .5 * quart;
	} break;
	case Above3rd: {
		azimuthalAngle = 0;
		polarAngle     = quart;
	} break;
	case Below3rd: {
		azimuthalAngle = 3 * quart;
		polarAngle     = .5 * quart;
	} break;
	case OrbitHorizontal3rd: {
		azimuthalAngle = orbitAngle;
		polarAngle     = .5 * quart;
	} break;
	case OrbitVertical3rd: {
		azimuthalAngle = 0;
		polarAngle     = orbitAngle;
	} break;
	case FreeView3rd:
	case FirstView: {
	} break;
	}

	switch (view) {
	case Front3rd:
	case Left3rd:
	case Back3rd:
	case Right3rd:
	case Above3rd:
	case Below3rd:
	case OrbitHorizontal3rd:
	case OrbitVertical3rd:
	case FreeView3rd: {
		camera = mTarget
		         + Vec3::Polar3D(azimuthalAngle, polarAngle) * radius;
	} break;
	case FirstView:
		break;
	}
}

} // namespace OpenGL
