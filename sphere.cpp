#include "template.h"
#include "sphere.h"
#include <algorithm>

void Sphere::Intersect(Ray& ray)
{
	
	if (!ray.inside)
	{
		glm::vec3 c = location - ray.origin;

		float t = glm::dot(c, ray.direction);
		glm::vec3 q = c - t * ray.direction;

		float p2 = dot(q, q);
		float r2 = radius * radius;

		if (p2 > r2) 
			return; // r2 = r * r

		t -= glm::sqrt(r2 - p2);

		if (t < ray.length && t > 0)
		{
			ray.length = t;
			ray.hit = this;
		}
		// or: ray.t = min( ray.t, max( 0, t ) );
	}
	
	else
	{
		vec3 D = ray.direction;
		vec3 O = ray.origin;
		vec3 C = location;
		float r2 = radius * radius;

		float a = glm::dot(D, D);
		float b = glm::dot(2.f * D, (O - C));
		float c = glm::dot((O - C), (O - C)) - r2;

		float det = b * b - 4 * a* c;
		// No intersection
		if (det < 0)
			return;

		float sd = glm::sqrt(det);
		float t1 = (-b + sd) / 2 * a;
		float t2 = (-b - sd) / 2 * a;
		float t;
		if (t1 < 0 && t2 >= 0)
			t = t2;
		else if (t2 < 0 && t1 >= 0)
			t = t1;
		else if (t2 < 0 && t1 < 0)
			return;
		else
			t = glm::min(t1, t2);
		if (t < ray.length)
		{
			ray.length = t;
			ray.hit = this;
		}
	}
}

glm::vec3 Sphere::Sample(Ray& ray, Ray& lightRay)
{
	glm::vec3 hitLoc = lightRay.origin;
	glm::vec3 normal = Normal(hitLoc);
	return material->texture[0]*lightRay.color * std::max(0.f,glm::dot(normal, lightRay.direction)/(lightRay.length*lightRay.length));
	

	// or: ray.t = min( ray.t, max( 0, t ) );
}

glm::vec3 Sphere::Normal(glm::vec3 loc)
{
	return (loc - location) / radius;
}

AABB Sphere::CalculateBounds()
{
	return AABB(location - radius, location + radius);
}