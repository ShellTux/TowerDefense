#ifndef INCLUDE_OPENGL_CAMERA_HPP_
#define INCLUDE_OPENGL_CAMERA_HPP_

#include "Vec3.hpp"
#include "types.hpp"

#include <ostream>
#include <string>

namespace OpenGL {
class Camera {
      public:
	enum View : u8 {
		Front3rd,
		Left3rd,
		Back3rd,
		Right3rd,
		Above3rd,
		Below3rd,
		OrbitHorizontal3rd,
		OrbitVertical3rd,
		FreeView3rd,
		FirstView,
	};

	Camera(const Vec3 &target,
	       const f64 defaultRadius,
	       const u32 width,
	       const u32 height,
	       const bool invertX,
	       const bool invertY);
	Camera(Camera &&)                 = default;
	Camera(const Camera &)            = default;
	Camera &operator=(Camera &&)      = default;
	Camera &operator=(const Camera &) = default;
	~Camera()                         = default;

	Camera &operator+=(const usize i);

	friend std::ostream &operator<<(std::ostream &os, const Camera &camera);

	void apply();

	Camera &orbit(const f64 azimuthalAngleDelta, const f64 polarAngleDelta);
	Camera &perspective(const f32 theta, const f32 alpha, const f32 beta);
	Camera &reset();
	Camera &setRadius();
	Camera &setRadius(const f64 radius);
	Camera &setTarget();
	Camera &setTarget(const Vec3 &target);
	Camera &setWidthHeight(const u32 width, const u32 height);
	Camera &zoomIn(const f64 radiusDelta);

        [[nodiscard]] std::string toString() const;

      private:
	struct Default {
		Vec3 target;
		f64 radius;
		View view;
	};

	Vec3 mTarget;

	View view;

	Vec3 camera;
	Vec3 up;

	f64 radius;
	f64 azimuthalAngle;
	f64 polarAngle;
	f64 orbitAngle;

	f64 xmax;
	f64 ymax;
	f64 nearClip;
	f64 farClip;

	u32 width;
	u32 height;

	bool invertX;
	bool invertY;

	struct Default defaults;

	void update();

	void lookAt() const;
	void perspective() const;
};
} // namespace OpenGL

#endif // INCLUDE_OPENGL_CAMERA_HPP_
