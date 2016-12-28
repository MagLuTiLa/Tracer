#pragma once
#include "light.h"
class SpotLight :
	public Light
{
public:
	SpotLight(vec3 location, vec3 direction, float radius, vec3 color) :
		Light(location, color)
	{
		Direction(direction);
		Radius(radius);
	};

	inline virtual void getIllumination(vec3 point, Ray& outRay)
	{
		float len = glm::length(location - point);
		vec3 dir = (location - point) / len;
		outRay.color = color;
		outRay.length = len;
		outRay.hit = reinterpret_cast<Primitive*>(glm::dot(dir, minusDirection) < cosa);
	}

	inline float Radius(){return radius;}
	inline float Radius(float r) {cosa = 1 / glm::sqrt(r*r + 1); return radius = r;}
	inline vec3 Direction() { return -minusDirection; }
	inline vec3 Direction(vec3 dir) { return -(minusDirection = -dir); }
private:
	float radius;
	float cosa;
	vec3 minusDirection;
};

