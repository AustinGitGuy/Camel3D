#include "Vector3.h"
#include <cmath>

Vector3::Vector3(float nX, float nY, float nZ):x(nX),y(nY),z(nZ){}

Vector3& Vector3::operator += (const Vector3& other){
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator -= (const Vector3& other){
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator = (const Vector3& other){
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

Vector3& Vector3::operator *= (float multiplier){
	x *= multiplier;
	y *= multiplier;
	z *= multiplier;
	return *this;
}

Vector3& Vector3::operator /= (float divisor){
	x /= divisor;
	y /= divisor;
	z /= divisor;
	return *this;
}

Vector3 Vector3::operator+(const Vector3 &other) const {
	Vector3 result = *this;
	result += other;
	return result;
}

Vector3 Vector3::operator-(const Vector3 &other) const {
	Vector3 result = *this;
	result -= other;
	return result;
}

Vector3 Vector3::operator*(float multiplier) const {
	Vector3 result = *this;     
	result.x *= multiplier;
	result.y *= multiplier;
	result.z *= multiplier;

	return result;
}

Vector3 Vector3::operator/(float divisor) const {
	Vector3 result = *this;
	result.x /= divisor;
	result.y /= divisor;
	result.z /= divisor;

	return result;
}

bool Vector3::operator==(const Vector3& other){
	if((x == other.y) && (y == other.y) && (z == other.z)){
		return true;
	}
	else return false;
}

bool Vector3::operator!=(const Vector3& other){
	if((x == other.x) && (y == other.y) && (z == other.z)){
		return false;
	}
	else return true;
}

void CrossProduct(Vector3 one, Vector3 two, Vector3* product){
	product->x = one.y * two.z - one.z * two.y; 
	product->y = one.x * two.z - one.z * two.x; 
	product->z = one.x * two.y - one.y * two.x; 
}

Vector3 Lerp(Vector3 one, Vector3 two, float time){
	Vector3 result(0, 0, 0);
	result.x = (1 - time) * one.x + time * two.x;
	result.y = (1 - time) * one.y + time * two.y;
	result.z = (1 - time) * one.z + time * two.z;
	return result;
}
