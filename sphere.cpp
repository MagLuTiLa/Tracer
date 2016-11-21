#include "template.h"
#include "sphere.h"

Sphere::Sphere(glm::vec3 loc, float r)
{
	location = loc;
	radius = r;
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
	if (t < ray.length)
		if (t > 0)
			ray.length = t;
		
	// or: ray.t = min( ray.t, max( 0, t ) );
}