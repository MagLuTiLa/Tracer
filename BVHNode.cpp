#include "template.h"
#include "BVHNode.h"

float BVHNode::Area()
{
	glm::vec3 difference = corner2 - corner1;

	return difference.x * difference.y * difference.z;
}

float BVHNode::Cost()
{
	return Area() * count;
}

bool BVHNode::Intersect(Ray & ray)
{
	__m128 t1 = _mm_mul_ps(_mm_sub_ps(c1, ray.quadOrigin), ray.quadDirection);
	__m128 t2 = _mm_mul_ps(_mm_sub_ps(c2, ray.quadOrigin), ray.quadDirection);
	__m128 vmax4 = _mm_max_ps(t1, t2), vmin4 = _mm_min_ps(t1, t2);

	float* vmax = (float*)&vmax4, *vmin = (float*)&vmin4;
	float tmax = min(vmax[0], max(vmax[1], vmax[2]));
	float tmin = max(vmin[0], min(vmin[1], vmin[2]));

	return (tmax >= tmin && tmax >= 0);
}

