#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <math.h>
#include <iostream>
#include <vector>
using sf::Vector2f;
using sf::Vector3f;
using std::cout;
using std::vector;

Vector3f rotateVect(Vector3f current, float a, float b, float c) {
	float x, y, z;
	x = current.x * (cos(b) * cos(c)) - current.y * (sin(c) * cos(b)) + current.z * (sin(b));
	y = current.x * (sin(a) * sin(b) * cos(c) + sin(c) * cos(a)) + current.y * (-sin(a) * sin(b) * sin(c) + cos(a) * cos(c)) - current.z * (sin(a) * cos(b));
	z = current.x * (sin(a) * sin(c) - sin(b) * cos(a) * cos(c)) + current.y * (sin(a) * cos(c) + sin(b) * sin(c) * cos(a)) + current.z * (cos(a) * cos(b));




	return Vector3f{ x,y,z };
}



float dot(Vector3f a, Vector3f b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


void normalize(Vector3f* a) {
	float lenght = sqrtf((*a).x * (*a).x + (*a).y * (*a).y + (*a).z * (*a).z);
	if (lenght != 0) {
		(*a).x /= lenght;
		(*a).y /= lenght;
		(*a).z /= lenght;
	}
}
Vector3f norm(Vector3f a) {
	float lenght = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	if (lenght != 0) {
		a.x /= lenght;
		a.y /= lenght;
		a.z /= lenght;
	}
	return a;
}


Vector3f avarage(vector<Vector3f> p) {
	Vector3f res;
	for (int i = 0; i < p.size(); i++) {
		res += p[i];
	}
	return res / (float)p.size();
}

Vector3f refclected(Vector3f a, Vector3f b) {
	b = norm(b);
	a = norm(a);
	Vector3f fn = dot(a, b) * b;
	return a + 2.0f * (fn - a);
}