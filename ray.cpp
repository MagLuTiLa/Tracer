#include "template.h"
#include "ray.h"

Ray::Ray(glm::vec3 o, glm::vec3 d, float l)
{
	origin = o;
	direction = d;
	length = l;
}