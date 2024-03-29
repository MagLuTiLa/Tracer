#include "template.h"
#include "plane.h"

void Plane::Intersect(Ray& ray)
{
	float dP = glm::dot(normal, ray.direction);
	if (dP < 0)
	{
		glm::vec3 p = (location - ray.origin);

		float dist = glm::dot(p, normal) / dP;
		if (dist < ray.length)
		{
			ray.length = dist;
			ray.hit = this;
		}
	}
}

glm::vec3 Plane::Sample(Ray & ray, Ray & lightRay)
{
	float intencity = glm::dot(normal, lightRay.direction);
	vec2 uv = vec2(lightRay.origin);
	uv.x = abs(uv.x-(long)uv.x);
	uv.y = abs(uv.y-(long)uv.y);

	return material->Color(uv)*lightRay.color * intencity / (lightRay.length*lightRay.length);
}

glm::vec3 Plane::Normal(glm::vec3 loc)
{
	return normal;
}

AABB Plane::CalculateBounds()
{
	return AABB();
}

glm::vec3 Plane::Centroid()
{
	return location;
}
