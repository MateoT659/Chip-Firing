#include "Vec2.h"

Vec2::Vec2(int x, int y) {
	this->x = x;
	this->y = y;
}

Vec2::Vec2() {
	x = 0;
	y = 0;
}

void Vec2::set(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Vec2::add(Vec2 other) {
	x += other.x;
	y += other.y;
}

void Vec2::sub(Vec2 other) {
	x -= other.x;
	y -= other.y;
}

void Vec2::mul(int c) {
	x *= c;
	y *= c;
}

void Vec2::mul(double c) {
	x *= c;
	y *= c;
}

double Vec2::dot(Vec2 other) {
	return x * other.x + y * other.y;
}

double Vec2::mag() {
	return sqrt(x * x + y * y);
}

double Vec2::mag2() {
	return x * x + y * y;
}

Vec2 Vec2::getPerp() {
	return Vec2(-y, x);
}
Vec2 Vec2::getUnit()
{
	double magn = mag();

	return magn == 0 ? Vec2() : *this / mag();
}
Vec2 Vec2::getProjectionOn(Vec2 other) {
	double mag2n = other.mag2();
	if (mag2n == 0) return Vec2();
	return other * ((*this * other) / other.mag2());
}

Vec2 Vec2::operator+(Vec2 other) {
	return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::operator-(Vec2 other) {
	return *this + (other * -1);
}
Vec2 Vec2::operator*(double c) {
	return Vec2(x * c, y * c);
}
Vec2 Vec2::operator*(int c) {
	return Vec2(x * c, y * c);
}
double Vec2::operator*(Vec2 other) {
	return x * other.x + y * other.y;;
}
Vec2 Vec2::operator/(double c) {
	return Vec2(x / c, y / c);
}
void Vec2::operator+=(Vec2 other) {
	add(other);
}
void Vec2::operator-=(Vec2 other) {
	sub(other);
}
void Vec2::operator*=(int c) {
	mul(c);
}
void Vec2::operator*=(double c) {
	mul(c);
}

std::string Vec2::toString() {
	return "[" + std::to_string(x) + ", " + std::to_string(y) + "]";
}