#ifndef INCLUDE_TOWERDEFENSE_VEC3_HPP_
#define INCLUDE_TOWERDEFENSE_VEC3_HPP_

#include <iostream>
#include <tuple>

namespace TowerDefense {

class Vec3 {
      public:
	Vec3();
	Vec3(const double x, const double y);
	Vec3(const double x, const double y, const double z);
	Vec3(Vec3 &&)                 = default;
	Vec3(const Vec3 &)            = default;
	Vec3 &operator=(Vec3 &&)      = default;
	Vec3 &operator=(const Vec3 &) = default;
	~Vec3()                       = default;

	static double Dot(const Vec3 &a, const Vec3 &b);
	static Vec3 Cross(const Vec3 &a, const Vec3 &b);
	static Vec3 Normalize(const Vec3 &a);
	static Vec3 RandomUnitVec3();

	bool operator!=(const Vec3 &other) const;
	bool operator==(const Vec3 &other) const;
	double operator*(const Vec3 &other) const;
	Vec3 &operator*=(const double scalar);
	Vec3 &operator/=(const double scalar);
	Vec3 &operator%=(const Vec3 &other);
	Vec3 &operator+=(const Vec3 &other);
	Vec3 &operator-=(const Vec3 &other);
	Vec3 operator%(const Vec3 &other) const;
	Vec3 operator+(const Vec3 &other) const;
	Vec3 operator-(const Vec3 &other) const;
	Vec3 operator^(const Vec3 &other) const;
	Vec3 operator*(double scalar) const;
	Vec3 operator/(double scalar) const;

	friend std::ostream &operator<<(std::ostream &os, const Vec3 &vec)
	{
		os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	[[nodiscard]] double dot(const Vec3 &other) const;
	[[nodiscard]] double magnitude() const;
	[[nodiscard]] double magnitudeSq() const;
	[[nodiscard]] std::tuple<double, double, double> getCoordinates() const;
	[[nodiscard]] Vec3 cross(const Vec3 &other) const;
	[[nodiscard]] Vec3 normalize() const;

      private:
	double x;
	double y;
	double z;
};

} // namespace TowerDefense

#endif // INCLUDE_TOWERDEFENSE_VEC3_HPP_
