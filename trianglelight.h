#pragma once
#include "light.h"
#include "triangle.h"
class TriangleLight : public Light
{
public:
	TriangleLight::TriangleLight(Triangle * t, glm::vec3 c) :
		Light(t->Centroid(), c)
	{
		triangle = t;
		t->light = this;
	}

	// TODO might have to account for radius if using sphere
	inline virtual void getIllumination(const Ray& ray, Ray& outRay)
	{
		glm::vec3 point = ray.origin + ray.direction * (ray.length - EPSILON);
		glm::vec3 N = ray.hit->Normal(point);
		vec3 rLoc = triangle->RandomPointOn();
		float len = glm::length(rLoc - point);

		outRay.origin = point;
		outRay.direction = (rLoc - point) / len;
		outRay.color = color;
		outRay.length = len - EPSILON;

		float cosO = dot(-outRay.direction, triangle->Normal(rLoc));
		float solidAngle = (cosO * triangle->Area()) / (len * len);
		float cosI = dot(outRay.direction, N);
		cosI = max(cosI, 0.f);
		outRay.color *= solidAngle * cosI;
	}

	Triangle* triangle;
};