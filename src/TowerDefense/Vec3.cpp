#include "TowerDefense/Vec3.hpp"

#include <cmath>
#include <random>
#include <tuple>

constexpr double PI = 3.14159265358979323846;

namespace TowerDefense {

Vec3::Vec3()
    : x(0)
    , y(0)
    , z(0)
{}

Vec3::Vec3(const double x, const double y)
    : x(x)
    , y(y)
    , z(0)
{}

Vec3::Vec3(const double x, const double y, const double z)
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


	const double u = dis(gen);
	const double v = dis(gen);


	const double theta = 2 * PI * u;
	const double phi   = acos(2 * v - 1);


	const double x = sin(phi) * cos(theta);
	const double y = sin(phi) * sin(theta);
	const double z = cos(phi);

	return {x, y, z};
}

double Vec3::Dot(const Vec3 &a, const Vec3 &b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3::operator+(const Vec3 &other) const
{
	return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3 &other) const
{
	return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator*(double scalar) const
{
	return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 Vec3::operator/(double scalar) const
{
	if (scalar != 0) {
		return Vec3(x / scalar, y / scalar, z / scalar);
	} else {
		throw std::runtime_error("Division by zero");
	}
}

double Vec3::operator*(const Vec3 &other) const
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vec3 Vec3::operator^(const Vec3 &other) const
{
	return Vec3(y * other.z - z * other.y,
	            z * other.x - x * other.z,
	            x * other.y - y * other.x);
}

bool Vec3::operator==(const Vec3 &other) const
{
	return (x == other.x) && (y == other.y) && (z == other.z);
}

bool Vec3::operator!=(const Vec3 &other) const
{
	return !(*this == other);
}

Vec3 &Vec3::operator*=(const double scalar)
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

Vec3 &Vec3::operator/=(const double scalar)
{
	*this = *this / scalar;
	return *this;
}

double Vec3::dot(const Vec3 &other) const
{
	return Dot(*this, other);
}

double Vec3::magnitude() const
{
	return std::sqrt(magnitudeSq());
}

double Vec3::magnitudeSq() const
{
	return dot(*this);
}

std::tuple<double, double, double> Vec3::getCoordinates() const
{
	return {x, y, z};
}

} // namespace TowerDefense
