#include "template.h"
#include "sphere.h"
#include <algorithm>

Sphere::Sphere(glm::vec3 loc, float r):
	Primitive(loc),
	radius(r)
{
}

void Sphere::Intersect(Ray& ray)
{
	glm::vec3 c = location - ray.origin;
	float t = glm::dot(c, ray.direction);
	glm::vec3 q = c - t * ray.direction;
	float p2 = dot(q, q);
	float r2 = radius * radius;
	if (p2 > r2) return; // r2 = r * r
	t -= glm::sqrt(r2 - p2);
	if (t < ray.length && t > 0)
	{
		ray.length = t;
		ray.hit = this;
	}

	// or: ray.t = min( ray.t, max( 0, t ) );
}

glm::vec3 Sphere::Sample(Ray& ray, Ray& lightRay)
{
	glm::vec3 hitLoc = lightRay.origin;
	glm::vec3 normal = (hitLoc - location) / radius;
	return color*lightRay.color * std::max(0.f,glm::dot(normal, lightRay.direction)/(lightRay.length*lightRay.length));
	

	// or: ray.t = min( ray.t, max( 0, t ) );
}