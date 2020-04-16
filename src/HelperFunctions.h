#pragma once
#include <vector>
#include <glm/glm.hpp>

using namespace glm;

#define PI 3.14159265

template<typename T>
std::vector<T> sub_vector(std::vector<T> const& v, int m, int n) {
    auto first = v.begin() + m;
    auto last = v.begin() + n + 1;
    std::vector<T> vector(first, last);
    return vector;
}

vec3 points_to_vector(vec3 v1, vec3 v2) {
	return vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
}

vec2 coord_after_rotation(vec2 old_position, vec2 rotating_point, float rotation_angle, float new_angle) {
	float x_prime = 0;
	float y_prime = 0;
	if (new_angle > 90 && new_angle <= 180)
	{
		//rotation_angle = rotation_angle - 90;
		x_prime = -1 * ((old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle)) + rotating_point.x;
		y_prime = (old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle) + rotating_point.y;
	}
	else if (new_angle > 180 && new_angle <= 270)
	{
		//rotation_angle = rotation_angle - 180;
		x_prime = -1 * ((old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle)) + rotating_point.x;
		y_prime = -1 * ((old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle)) + rotating_point.y;
	}
	else if (new_angle > 270 && new_angle <= 360)
	{
		//rotation_angle = rotation_angle - 270;
		x_prime = (old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle) + rotating_point.x;
		y_prime = -1 * ((old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle)) + rotating_point.y;
	}
	else
	{
		x_prime = (old_position.x - rotating_point.x) * cos(rotation_angle) - (old_position.y - rotating_point.y) * sin(rotation_angle) + rotating_point.x;
		y_prime = (old_position.x - rotating_point.x) * sin(rotation_angle) + (old_position.y - rotating_point.y) * cos(rotation_angle) + rotating_point.y;
	}
	return vec2(x_prime, y_prime);
}

long double calc_angle(vec3 v1, vec3 v2)
{
	v1 = normalize(v1);
	v2 = normalize(v2);
	long double numerator = dot(v1, v2);
	long double denominator = (double)length(v1) * length(v2);

	long double f = acos(numerator / denominator);
	//convert radians to degrees

	return f * 180.0 / PI;
}


float frand()
{
	return (float)rand() / 32768.0;
}




float sign(float f)
{
	if (f < 0)return -1;
	else return 1;
}