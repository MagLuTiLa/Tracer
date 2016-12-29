#include "template.h"
#include "renderer.h"

#include "obj.h"
#define USEBVH
#define USEBVHLh
#define DEPTHTRACER

Renderer::Renderer()
{
	Init();
}

void Renderer::Init()
{
	/*
	for (int i = -10; i < 10; i++)
	{
		AddPrimitive(new Triangle(vec3(2 * i + 0, -2, 13), vec3(2 * i + 0, 2, 12), vec3(2 * i + 2, -2, 12), new Material(vec3(1, 0, 0))));
		AddPrimitive(new Triangle(vec3(2 * i + 2, -2, 12), vec3(2 * i + 0, 2, 12), vec3(2 * i + 2, 2, 11)));
	}*/

	
	Material* texture = new Material(.5, "wood.bmp");
	
	LoadObj("box.obj", primitives, texture, mat4(1, 0, 0, 0,
		0, std::cos(2), -std::sin(2), 0,
		0, std::sin(2), std::cos(2), 0,
		0, 0, 0, 1)
		*
		mat4(std::cos(2), 0, -std::sin(2), 0,
			0, 1, 0, 0,
			std::sin(2), 0, std::cos(2), 0,
			0, 0, 0, 1)
		*
		mat4(.5, 0, 0, 0,
			0, .5, 0, 0,
			0, 0, .5, 4,
			0, 0, 0, 1));

	LoadObj("box.obj", primitives, texture, mat4(1, 0, 0, 0,
		0, std::cos(2), -std::sin(2), 0,
		0, std::sin(2), std::cos(2), 0,
		0, 0, 0, 1)
		*
		mat4(std::cos(2), 0, -std::sin(2), 0,
			0, 1, 0, 0,
			std::sin(2), 0, std::cos(2), 0,
			0, 0, 0, 1)
		*
		mat4(.5, 0, 0, 5,
			0, .5, 0, 0,
			0, 0, .5, 4,
			0, 0, 0, 1));
			
	LoadObj("box.obj", primitives, texture, mat4(1, 0, 0, 0,
		0, std::cos(2), -std::sin(2), 0,
		0, std::sin(2), std::cos(2), 0,
		0, 0, 0, 1)
		*
		mat4(std::cos(2), 0, -std::sin(2), 0,
			0, 1, 0, 0,
			std::sin(2), 0, std::cos(2), 0,
			0, 0, 0, 1)
		*
		mat4(.5, 0, 0, -5,
			0, .5, 0, 0,
			0, 0, .5, 4,
			0, 0, 0, 1));

	AddLight(new PointLight(vec3(0, 0, 0), vec3(50.f, 50.f, 50.f)));
	
	bvh = BVH();
	bvh.ConstructBVH(&primitives, primitives.size());
	
	//AddPrimitive(new Sphere(vec3(3, 3, 3), 1.5f, new Material(0.9f, vec3(1., 1., 1.))));
	/*
	AddPrimitive(new Triangle(vec3(-1000, 5, 10), vec3(-1000, 5, -10), vec3(1000, 5, -10)));
	AddPrimitive(new Triangle(vec3(-1000, 5, 10), vec3(1000, 5, 10), vec3(1000, 5, -10)));*/

	// Bye bye fancy scene, you will be remembered for your bravery.
	/*
	AddPrimitive(new Sphere(vec3(-3, -3, -3), 1.15f, new Material(1.5f, vec3(0., 1., 0.))));
	AddPrimitive(new Sphere(vec3(3, 3, 3), 1.5f, new Material(0.9f, vec3(1., 1., 1.))));
	AddPrimitive(new Sphere(vec3(-5, -1, 5), 1.f, new Material(vec3(0., 0., 1.))));
	AddPrimitive(new Sphere(vec3(5, 4, 4), 0.5f, new Material(vec3(1., 0.5, 0.))));

	AddPrimitive(new Plane(vec3(0, 0, 7), vec3(0, 0, -1), new Material(1.f, vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(0, 0, -7), vec3(0, 0, 1), new Material(vec3(0.3, 0.3, 1.))));
	AddPrimitive(new Plane(vec3(0, 7, 0), vec3(0, -1, 0), new Material(1.f, vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(0, -7, 0), vec3(0, 1, 0), new Material(vec3(0.3, 1., 0.3))));
	AddPrimitive(new Plane(vec3(7, 0, 0), vec3(-1, 0, 0), new Material(1.f, vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(-7, 0, 0), vec3(1, 0, 0), new Material(vec3(1., 0.3, 0.3))));

	AddLight(new PointLight(vec3(0, 0, 0), vec3(20.f, 20.f, 20.f)));
	AddLight(new PointLight(vec3(-3.5f, 3.5f, 5.5f), vec3(2.f, 10.f, 2.f)));
	AddLight(new PointLight(vec3(3, -3, 3), vec3(9.f, 1.f, 9.f)));
	AddLight(new SpotLight(vec3(0, 0, 2), vec3(0, 0, 1), .05, vec3(2.5, 0, 0)));

	Material* texture = new Material(.5, "wood.bmp");
	LoadObj("box.obj", primitives, texture, mat4(1, 0, 0, 0,
	0, std::cos(2), -std::sin(2), 0,
	0, std::sin(2), std::cos(2), 0,
	0, 0, 0, 1)
	*
	mat4(std::cos(2), 0, -std::sin(2), 0,
	0, 1, 0, 0,
	std::sin(2), 0, std::cos(2), 0,
	0, 0, 0, 1)
	*
	mat4(.2, 0, 0, 0,
	0, .2, 0, 0,
	0, 0, .2, 4,
	0, 0, 0, 1));
	*/
}

glm::vec3 Renderer::TraceRay(Ray & ray)
{
	// See if ray intersects with primitives

#ifdef DEPTHTRACER
	int depth = 0;
	bvh.Traverse(ray, 0, &depth);
	return vec3(min(1.f, max(0.f, ((float)depth-20.f)/20.f)) , max(0.f, 1-(float)depth/20.f), 0);
#endif

#ifdef USEBVH
	bvh.Traverse(ray, 0);
#else

	for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
	{
		Primitive* p = primitives[i];
		vec3 locthis = p->location;
		p->Intersect(ray);
	}
	
#endif

	vec3 lightIntensity = vec3();

	// If ray collided with a primitive
	if (ray.length < std::numeric_limits<float>::max())
	{
		if (ray.traceDepth >= MAXTRACEDEPTH)
			return DirectIllumination(ray);
		Material material = *(ray.hit->material);
		if (material.IsOpaque())
			lightIntensity = DirectIllumination(ray);
		else if (material.IsReflective())
		{
			float s = material.ref;
			if (s == 1.f)
				lightIntensity = Reflect(ray);
			else
			{
				lightIntensity += s * Reflect(ray);
				lightIntensity += (1 - s) * DirectIllumination(ray);
			}
		}
		else if (material.IsRefractive())
		{
			if (!ray.inside)
				lightIntensity = Refract(ray, 1.f, material.ref);
			else
				lightIntensity = Refract(ray, material.ref, 1.f);
		}
	}
	return lightIntensity;
}

glm::vec3 Renderer::DirectIllumination(Ray& ray)
{
	vec3 lightIntensity = vec3();

	// Draw a shadow ray towards the light sources
	glm::vec3 rayPos = ray.origin + ray.direction * (ray.length - EPSILON);

	for (std::vector<Light>::size_type i = 0; i != lights.size(); i++)
	{
		Light* l = lights[i];
		Ray shadowRay;
		l->getIllumination(rayPos, shadowRay);
		// See if shadow ray intersects with primitives
		/*
		for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
		{
			Primitive* p = primitives[i];
			p->Intersect(shadowRay);
			if (shadowRay.hit != NULL)
				goto next;
		}
		*/
#ifdef USEBVHL
		//TODO, in order for this to be benificial, have to make an alternative version of traversal that stops on first thing hit.
		bvh.Traverse(ray, 0);
		if (shadowRay.hit != NULL)
			continue;
#else

		for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
		{
			Primitive* p = primitives[i];
			p->Intersect(shadowRay);
			if (shadowRay.hit != NULL)
				goto next;
		}

#endif

		// Add color to the lightintensity
		lightIntensity += ray.hit->Sample(ray, shadowRay);
	next:;
	}
	return lightIntensity;
}

glm::vec3 Renderer::Reflect(Ray& ray)
{
	vec3 lightIntensity = vec3();
	vec3 rayPos = ray.origin + ray.direction * (ray.length - EPSILON);

	vec3 colPos = ray.origin + ray.direction * ray.length;
	vec3 normal = ray.hit->Normal(colPos);
	if (ray.inside)
		normal = -normal;
	vec3 rayDir = ray.direction - 2.f * (glm::dot(ray.direction, normal) * normal);

	Ray newRay(rayPos, rayDir);
	newRay.traceDepth = ray.traceDepth + 1;
	vec3 light = TraceRay(newRay);
	lightIntensity = light * ray.hit->Color(colPos);
	return lightIntensity;
}

glm::vec3 Renderer::Refract(Ray& ray, float from, float to)
{
	// Determine reflection and refraction part
	vec3 lightIntensity = vec3();

	float s = ray.length;
	float fdt = from / to;
	vec3 D = ray.direction;
	vec3 colPos = ray.origin + D * s;
	vec3 N = ray.hit->Normal(colPos);
	if (ray.inside)
		N *= -1;

	float cosTheta = dot(N, -D);

	// K calculation
	float l = (fdt * fdt);
	float r = (1 - cosTheta * cosTheta);
	float k = 1 - l * r;

	//TIL
	if (k < 0)
	{
		lightIntensity = Reflect(ray);
	}

	else
	{
		vec3 colPos = ray.origin + ray.direction * s;
		vec3 rayPos = colPos - N * EPSILON;

		vec3 d1 = fdt * D;
		vec3 d2 = N * (fdt * cosTheta - sqrtf(k));
		vec3 rayDir = d1 + d2;

		float mC = 1 - cosTheta;
		float cos5 = mC * mC * mC * mC * mC;
		float R0 = (from - to) / (from + to);
		R0 *= R0;
		float Fr = R0 + (1 - R0) * cos5;

		Ray newRay(rayPos, rayDir);
		newRay.traceDepth = ray.traceDepth + 1;
		if (ray.hit->hax == 1)
			newRay.inside = !ray.inside;
		else
			newRay.inside = ray.inside;

		vec3 transRay = (1 - Fr) * TraceRay(newRay);
		vec3 reflRay = Fr * Reflect(ray);

		vec3 matColor = ray.hit->Color(colPos);

		// Beer's Law
		if (ray.inside)
		{
			transRay.r *= expf(-matColor.r * s);
			transRay.g *= expf(-matColor.g * s);
			transRay.b *= expf(-matColor.b * s);

			reflRay.r *= expf(-matColor.r * s);
			reflRay.g *= expf(-matColor.g * s);
			reflRay.b *= expf(-matColor.b * s);
		}

		lightIntensity = (transRay + reflRay) * matColor;
	}

	return lightIntensity;
}

void Renderer::AddPrimitive(Primitive * p)
{
	primitives.push_back(p);
}

void Renderer::AddLight(Light * p)
{
	lights.push_back(p);
}