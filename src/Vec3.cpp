#include "Vec3.hpp"

#include "Math.hpp"
#include "types.hpp"

#include <cmath>
#include <random>
#include <stdexcept>
#include <tuple>

namespace TowerDefense {

Vec3::Vec3()
    : x(0)
    , y(0)
    , z(0)
{}

Vec3::Vec3(const f64 x, const f64 y)
    : x(x)
    , y(y)
    , z(0)
{}

Vec3::Vec3(const f64 x, const f64 y, const f64 z)
    : x(x)
    , y(y)
    , z(z)
{}

Vec3 Vec3::RandomUnitVec3()
{
	using std::sin, std::cos, std::acos;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);


	const f64 u = dis(gen);
	const f64 v = dis(gen);


	const f64 theta = 2 * Math::PId * u;
	const f64 phi   = acos(2 * v - 1);


	const f64 x = sin(phi) * cos(theta);
	const f64 y = sin(phi) * sin(theta);
	const f64 z = cos(phi);

	return {x, y, z};
}

Vec3 Vec3::Polar2D(const f64 angle)
{
	using std::cos, std::sin;

	return {cos(angle), sin(angle)};
}

Vec3 Vec3::Polar2D(const Vec3 &center, const f64 radius, const f64 angle)
{
	return center + Polar2D(angle) * radius;
}

f64 Vec3::AngleBetween(const Vec3 &a, const Vec3 &b)
{
	const f64 dotProduct = Dot(a, b);
	const f64 magA       = a.magnitude();
	const f64 magB       = b.magnitude();

	if (magA == 0 || magB == 0) {
		throw std::runtime_error(
		    "Cannot compute angle with a zero-length vector");
	}

	// Clamp value to avoid domain errors due to potential precision issues
	const f64 cosAngle
	    = std::fmax(-1.0, std::fmin(1.0, dotProduct / (magA * magB)));

	return std::acos(cosAngle);
}

f64 Vec3::Dot(const Vec3 &a, const Vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3::Cross(const Vec3 &a, const Vec3 &b)
{
	return a.cross(b);
}

Vec3 Vec3::Normalize(const Vec3 &a)
{
	return a / a.magnitude();
}

Vec3 Vec3::operator+(const Vec3 &other) const
{
	return {x + other.x, y + other.y, z + other.z};
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
	return {x - other.x, y - other.y, z - other.z};
}

Vec3 Vec3::operator*(f64 scalar) const
{
	return {x * scalar, y * scalar, z * scalar};
}

Vec3 Vec3::operator/(f64 scalar) const
{
	if (scalar == 0) {
		throw std::runtime_error("Division by zero");
	}

	return {x / scalar, y / scalar, z / scalar};
}

f64 Vec3::operator*(const Vec3 &other) const
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vec3 Vec3::operator^(const Vec3 &other) const
{
	return {y * other.z - z * other.y,
	        z * other.x - x * other.z,
	        x * other.y - y * other.x};
}

Vec3 Vec3::operator%(const Vec3 &other) const
{
	using std::fmod;

	return {fmod(x, other.x), fmod(y, other.y), fmod(z, other.z)};
}

bool Vec3::operator==(const Vec3 &other) const
{
	return (x == other.x) && (y == other.y) && (z == other.z);
}

bool Vec3::operator!=(const Vec3 &other) const
{
	return !(*this == other);
}

Vec3 &Vec3::operator*=(const f64 scalar)
{
	*this = *this * scalar;
	return *this;
}

Vec3 &Vec3::operator+=(const Vec3 &other)
{
	*this = *this + other;
	return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &other)
{
	*this = *this - other;
	return *this;
}

Vec3 &Vec3::operator/=(const f64 scalar)
{
	*this = *this / scalar;
	return *this;
}

Vec3 &Vec3::operator%=(const Vec3 &other)
{
	*this = *this % other;
	return *this;
}

f64 Vec3::dot(const Vec3 &other) const
{
	return Dot(*this, other);
}

f64 Vec3::magnitude() const
{
	return std::sqrt(magnitudeSq());
}

f64 Vec3::magnitudeSq() const
{
	return dot(*this);
}

std::tuple<f64, f64, f64> Vec3::getCoordinates() const
{
	return {x, y, z};
}

Vec3 Vec3::cross(const Vec3 &other) const
{
	return {y * other.z - z * other.y,
	        z * other.x - x * other.z,
	        x * other.y - y * other.x};
}

Vec3 Vec3::normalize() const
{
	return Normalize(*this);
}

Vec3 Vec3::transpose2D() const
{
	return {y, x, z};
}

} // namespace TowerDefense
