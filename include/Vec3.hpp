#ifndef INCLUDE_TOWERDEFENSE_VEC3_HPP_
#define INCLUDE_TOWERDEFENSE_VEC3_HPP_

#include "types.hpp"

#include <cmath>
#include <iostream>
#include <optional>
#include <tuple>

class Vec3 {
      public:
	Vec3();
	Vec3(const f64 x, const f64 y);
	Vec3(const f64 x, const f64 y, const f64 z);
	Vec3(Vec3 &&)                 = default;
	Vec3(const Vec3 &)            = default;
	Vec3 &operator=(Vec3 &&)      = default;
	Vec3 &operator=(const Vec3 &) = default;
	~Vec3()                       = default;

	static f64 Dot(const Vec3 &a, const Vec3 &b);
	static Vec3 Cross(const Vec3 &a, const Vec3 &b);
	static Vec3 Normalize(const Vec3 &a);
	static Vec3 RandomUnitVec3();
	static Vec3 Polar2D(const f64 angle);
	static Vec3 Polar3D(const f64 azimuthalAngle, const f64 polarAngle);
	static f64 AngleBetween(const Vec3 &a, const Vec3 &b);

	bool operator!=(const Vec3 &other) const;
	bool operator==(const Vec3 &other) const;
	f64 operator*(const Vec3 &other) const;

	Vec3 &operator*=(const f64 scalar);
	Vec3 &operator/=(const f64 scalar);
	Vec3 &operator%=(const Vec3 &other);
	Vec3 &operator+=(const Vec3 &other);
	Vec3 &operator-=(const Vec3 &other);

	Vec3 operator%(const Vec3 &other) const;
	Vec3 operator+(const Vec3 &other) const;
	Vec3 operator-(const Vec3 &other) const;
	Vec3 operator^(const Vec3 &other) const;
	Vec3 operator*(f64 scalar) const;
	Vec3 operator/(f64 scalar) const;

	friend std::ostream &operator<<(std::ostream &os, const Vec3 &vec)
	{
		os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	friend std::ostream &operator<<(std::ostream &os,
	                                const std::optional<Vec3> &vec)
	{
		if (vec.has_value()) {
			os << vec.value();
		} else {
			os << "Vec3(None)";
		}

		return os;
	}

	[[nodiscard]] f64 dot(const Vec3 &other) const;
	[[nodiscard]] f64 magnitude() const;
	[[nodiscard]] f64 magnitudeSq() const;
	[[nodiscard]] std::tuple<f64, f64, f64> getCoordinates() const;
	[[nodiscard]] Vec3 cross(const Vec3 &other) const;
	[[nodiscard]] Vec3 normalize() const;
	[[nodiscard]] Vec3 transpose2D() const;

      private:
	f64 x;
	f64 y;
	f64 z;
};

#endif // INCLUDE_TOWERDEFENSE_VEC3_HPP_
