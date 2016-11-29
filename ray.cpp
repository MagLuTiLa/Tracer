#include "template.h"
#include "ray.h"

Ray::Ray(glm::vec3 o, glm::vec3 d) :
	length(std::numeric_limits<float>::max()), 
	origin(o),
	direction(d),
	color(glm::vec3(0,0,0))
{
}