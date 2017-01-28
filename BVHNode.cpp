#include "template.h"
#include "BVHNode.h"

float BVHNode::Area()
{
	glm::vec3 difference = corner2 - corner1;

	float l = difference.x;
	float h = difference.y;
	float d = difference.z;

	return 2 * l*h + 2 * l*d + 2 * h*d;
}

float BVHNode::Cost()
{
	return Area() * count;
}

bool BVHNode::Intersect(Ray & ray, float &distance)
{
	const __m128
		inv_dir = _mm_rcp_ps(ray.quadDirection);

	// use a div if inverted directions aren't available
	const __m128 l1 = mulps(subps(c1, ray.quadOrigin), inv_dir);
	const __m128 l2 = mulps(subps(c2, ray.quadOrigin), inv_dir);

	// the order we use for those min/max is vital to filter out
	// NaNs that happens when an inv_dir is +/- inf and
	// (box_min - pos) is 0. inf * 0 = NaN
	const __m128 filtered_l1a = minps(l1, q_plus_inf);
	const __m128 filtered_l2a = minps(l2, q_plus_inf);

	const __m128 filtered_l1b = maxps(l1, q_minus_inf);
	const __m128 filtered_l2b = maxps(l2, q_minus_inf);

	// now that we're back on our feet, test those slabs.
	__m128 lmax = maxps(filtered_l1a, filtered_l2a);
	__m128 lmin = minps(filtered_l1b, filtered_l2b);

	// unfold back. try to hide the latency of the shufps & co.
	const __m128 lmax0 = rotatelps(lmax);
	const __m128 lmin0 = rotatelps(lmin);
	lmax = minss(lmax, lmax0);
	lmin = maxss(lmin, lmin0);

	const __m128 lmax1 = muxhps(lmax, lmax);
	const __m128 lmin1 = muxhps(lmin, lmin);
	lmax = minss(lmax, lmax1);
	lmin = maxss(lmin, lmin1);
	vec4 lminv;
	storess(lmin, &lminv);

	distance = lminv.x;

	return _mm_comige_ss(lmax, _mm_setzero_ps()) & _mm_comige_ss(lmax, lmin) && distance < ray.length;
}

