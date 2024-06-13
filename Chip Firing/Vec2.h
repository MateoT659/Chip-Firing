#pragma once
#include <string>

class Vec2
{
public:
	int x, y;

	Vec2(int x, int y);
	Vec2();

	void set(int x, int y);

	void add(Vec2); // essentially +=
	void sub(Vec2); // -=
	void mul(int); // *=
	void mul(double); // *= f
	double dot(Vec2);
	
	
	double mag();
	double mag2();
	Vec2 getPerp();
	Vec2 getUnit();
	Vec2 getProjectionOn(Vec2);

	void operator+=(Vec2);
	void operator-=(Vec2);
	void operator*=(int);
	void operator*=(double);
	
	double operator*(Vec2); //dot product operator

	Vec2 operator+(Vec2); //addition operator
	Vec2 operator-(Vec2); //subtraction operator
	Vec2 operator*(double); //float multiplication
	Vec2 operator/(double); //float div
	Vec2 operator*(int); //int multiplication
	bool operator==(const Vec2& other)const {
		return x == other.x && y == other.y;
	}

	std::string toString();
};

namespace std {
	template<>
	struct hash<Vec2> {
		size_t operator()(const Vec2& vec) const {
			return vec.x >= vec.y ? vec.x * vec.x + vec.x + vec.y : vec.x + vec.y * vec.y; 
		}
	};
}

