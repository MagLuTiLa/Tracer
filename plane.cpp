#include "template.h"
#include "plane.h"

Plane::Plane(glm::vec3 loc, glm::vec3 nor):
	Primitive(loc),
	normal(nor)
{
}

//http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
void Plane::Intersect(Ray& ray)
{
	float dP = glm::dot(normal, ray.direction);
	if (dP > 0)
	{
		glm::vec3 p = (location - ray.origin);
		float angle = glm::dot(normal, ray.direction);
		if (angle > 0)
		{
			float dist = glm::dot(p, normal) / angle;
			if (dist < ray.length)
			{
				ray.length = dist;
				ray.hit = this;
			}
		}
	}
}

glm::vec3 Plane::Sample(Ray & ray, Ray & lightRay)
{
	float intencity = -glm::dot(normal, lightRay.direction);
	return color*lightRay.color * intencity / (lightRay.length*lightRay.length);
}
