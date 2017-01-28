#include "template.h"
#include "triangle.h"



Triangle::~Triangle()
{
}

inline void Triangle::Intersect(Ray & ray)
{
	vec3 e1, e2;  //Edge1, Edge2
	vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;
	if (glm::dot(ray.direction, normal) > 0)
		return;
	//Find vectors for two edges sharing V1
	e1 = location2 - location;
	e2 = location3 -  location;
	//Begin calculating determinant - also used to calculate u parameter
	P = glm::cross(ray.direction, e2);
	//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	det = glm::dot(e1, P);
	//NOT CULLING
	if (det > -EPSILON && det < EPSILON) return;
	inv_det = 1.f / det;

	//calculate distance from V1 to ray origin
	T = ray.origin - location;

	//Calculate u parameter and test bound
	u = glm::dot(T, P) * inv_det;
	//The intersection lies outside of the triangle
	if ((u < 0.f) || (u > 1.f)) return;

	//Prepare to test v parameter
	Q = glm::cross(T, e1);

	//Calculate V parameter and test bound
	v = glm::dot(ray.direction, Q) * inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return;

	t = glm::dot(e2, Q) * inv_det;

	if ((t > EPSILON) && (t < ray.length)) {
		//ray intersection
		ray.length = t;
		ray.hit = this;
	}

}



glm::vec3 Triangle::Sample(Ray & ray, Ray & lightRay)
{
	float intencity = (glm::dot(normal, lightRay.direction));
	vec3 matcol = Color(lightRay.origin);

	return matcol*lightRay.color * intencity / (lightRay.length*lightRay.length);
}

glm::vec3 Triangle::Color(const vec3& position)
{
	if (uv == NULL)
		return material->texture[0];
	else
	{
		glm::vec3 v0 = location2 - location, v1 = location3 - location, v2 = position - location;
		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		float denom = d00 * d11 - d01 * d01;
		float v = (d11 * d20 - d01 * d21) / denom;
		float w = max(0.f,d00 * d21 - d01 * d20) / denom;
		float u = 1.0f - v - w;
		return material->Color(uv[0] * u + uv[1] * v + uv[2] * w);
	}
}

AABB Triangle::CalculateBounds()
{
	vec3 max = glm::max(glm::max(location, location2), location3);
	vec3 min = glm::min(glm::min(location, location2), location3);
	return AABB(min, max);
}

glm::vec3 Triangle::Normal(glm::vec3 loc)
{
	return normal;
}

glm::vec3 Triangle::Centroid()
{
	return (location + location2 + location3) / 3.f;
}

glm::vec3 Triangle::RandomPointOn()
{
	float u = 0;
	float v = 0;
	do
	{
		u = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		v = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
	while (u + v >= 1);
	vec3 uVec = (location2 - location) * u;
	vec3 vVec = (location2 - location) * v;
	return uVec + vVec;
}
