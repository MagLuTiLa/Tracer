#include "template.h"
#include "triangle.h"




Triangle::~Triangle()
{
}

void Triangle::Intersect(Ray & ray)
{
	vec3 e1, e2;  //Edge1, Edge2
	vec3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	//Find vectors for two edges sharing V1
	e1 = location2 - location;
	e2 = location3 -  location;
	//Begin calculating determinant - also used to calculate u parameter
	P = glm::cross(ray.direction, e2);
	//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
	det = glm::dot(e1, P);
	//NOT CULLING
	if (det > -0.0001 && det < 0.0001) return;
	inv_det = 1.f / det;

	//calculate distance from V1 to ray origin
	T = ray.origin - location;

	//Calculate u parameter and test bound
	u = glm::dot(T, P) * inv_det;
	//The intersection lies outside of the triangle
	if (u < 0.f || u > 1.f) return;

	//Prepare to test v parameter
	Q = glm::cross(T, e1);

	//Calculate V parameter and test bound
	v = glm::dot(ray.direction, Q) * inv_det;
	//The intersection lies outside of the triangle
	if (v < 0.f || u + v  > 1.f) return;

	t = glm::dot(e2, Q) * inv_det;

	if ((t > 0.0001) & (t < ray.length)) {
		//ray intersection
		ray.length = t;
		ray.hit = this;
	}

}

glm::vec3 Triangle::Sample(Ray & ray, Ray & lightRay)
{
	float intencity = (glm::dot(normal, lightRay.direction));
	return Color()*lightRay.color * intencity / (lightRay.length*lightRay.length);
}

glm::vec3 Triangle::Normal(glm::vec3 loc)
{
	return normal;
}
