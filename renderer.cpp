#include "template.h"
#include "renderer.h"
#include "plane.h"
#include "obj.h"
#include "trianglelight.h"
#include "spherelight.h"
#define USEBVH
#define USESAH
#define USEBVHL
//#define DEPTHTRACER
#define MCLIGHT

Renderer::Renderer()
{
}

int Renderer::Init()
{
	Material* texture = new Material(.0007, "wood.bmp");

	AddPrimitive(new Sphere(vec3(-2, -0.3f, 5), 0.8f, new Material(1.7f, vec3(1., 1., 1.))));
	AddPrimitive(new Sphere(vec3(0, -0.3f, 5), 0.8f, new Material(vec3(1., 1., 1.))));
	AddPrimitive(new Sphere(vec3(2, -0.3f, 5), 0.8f, new Material(.5f, vec3(0., 1., 0.))));
	AddPrimitive(new Sphere(vec3(0, -0.3f, 3), 0.8f, new Material(1.f, vec3(1., 1., 0.))));


	AddPrimitive(new Plane(vec3(0, 1, 0), vec3(0, -1, 0), new Material(vec3(1.f, 1.f, 1.f))));
	AddPrimitive(new Plane(vec3(0, -5, 0), vec3(0, 1, 0), new Material(vec3(1.f, 1.f, 1.f))));
	AddPrimitive(new Plane(vec3(0, 0, 6), vec3(0, 0, -1), new Material(vec3(1.f, 1.f, 1.f))));
	AddPrimitive(new Plane(vec3(0, 0, -6), vec3(0, 0, 1), new Material(vec3(1.f, 1.f, 1.f))));
	AddPrimitive(new Plane(vec3(-4, 0, 0), vec3(1, 0, 0), new Material(vec3(1.f, 1.f, 1.f))));
	AddPrimitive(new Plane(vec3(4, 0, 0), vec3(-1, 0, 0), new Material(vec3(1.f, 1.f, 1.f))));

	Triangle* tri = new Triangle(vec3(-2, -5 + EPSILON, 1), vec3(0, -5 + EPSILON, 5), vec3(2, -5 + EPSILON, 1));
	tri->light = true;
	AddPrimitive(tri);
	AddLight(new TriangleLight(tri, vec3(1000.f, 1000.f, 1000.f)));
	
	tri = new Triangle(vec3( 4 - EPSILON, -4, 1), vec3(4 - EPSILON, -2.5f, 5), vec3(4 - EPSILON, -1, 1));
	tri->light = true;
	AddPrimitive(tri);
	AddLight(new TriangleLight(tri, vec3(1000.f, 1000.f, 1000.f)));


	timer t;
#ifdef USESAH
	bvh = BVH();
	bvh.ConstructBVHSAH(&primitives);
#elif USEBVH
	bvh = BVH();
	bvh.ConstructBVH(&primitives);
#endif
	int time = (int)t.elapsed();

	return time;
}

glm::vec3 Renderer::TraceRay(Ray & ray)
{
	// See if ray intersects with primitives

	if (depthMode)
	{
		int depth = 0;

		bvh.Traverse(ray, 0, &depth);
		return vec3(min(0.f, 1.f-depth/5.f), depth / 5.f,0);
	}

	if(bvhMode)
		bvh.Traverse(ray, 0);
	else
		for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
		{
			Primitive* p = primitives[i];
			vec3 locthis = p->location;
			p->Intersect(ray);
		}
	
	vec3 lightIntensity = vec3();

	// If ray collided with a primitive
	if (ray.length < std::numeric_limits<float>::max())
	{
		if (ray.hit->light)
			return ray.hit->material->texture[0];
		if (ray.traceDepth >= MAXTRACEDEPTH)
			return DirectIllumination(ray);
		Material material = *(ray.hit->material);
		if (material.IsOpaque())
			lightIntensity =  0.01f * DirectIllumination(ray) +  IndirectIllumination(ray);
		else if (material.IsReflective())
		{
			float s = material.ref;
			if (s == 1.f)
				lightIntensity = Reflect(ray);
			else
			{
#if 0
				if(Rand(1.f) < s)
					lightIntensity += s / s * Reflect(ray);
				else
					lightIntensity += (1-s) / (1 - s) * DirectIllumination(ray);
#else
				//lightIntensity += s * Reflect(ray);
				lightIntensity += DirectIllumination(ray);
 				lightIntensity += IndirectIllumination(ray);
#endif
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
#ifdef MCLIGHT
	int size = lights.size();
	if (size < 1)
	{
		return lightIntensity;
	}
	//int index = rand() % size;
	float index = Randamonium();
	index *= size;
	index = (int)fmin(index, size - 1);
	Light* l = lights[index];
	Ray shadowRay;
	l->getIllumination(ray, shadowRay);
	
	// See if shadow ray intersects with primitives
	for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
	{
		Primitive* p = primitives[i];
		p->Intersect(shadowRay);
		if (shadowRay.hit != NULL)
			goto next;
	}
#else
	for (std::vector<Light>::size_type i = 0; i != lights.size(); i++)
	{
		Light* l = lights[i];
		Ray shadowRay;
		l->getIllumination(ray, shadowRay);
		// See if shadow ray intersects with primitives

#ifdef USEBVHL
		if (bvh.LightTraverse(shadowRay, 0))
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
#endif
		lightIntensity += ray.hit->Sample(ray, shadowRay);
	next:;
#ifdef MCLIGHT
		lightIntensity *= (float)size;
#else
	}
#endif
	return lightIntensity;
}

glm::vec3 Renderer::IndirectIllumination(Ray& ray)
{
	vec3 loc = ray.origin + ray.direction * ray.length;
	vec3 N = ray.hit->Normal(loc);
	if (ray.hit->material->IsOpaque())
	{
		vec3 p = normalize(PhongBRDF());
		Ray r(loc, WorldToLocal(p, N));
		r.traceDepth = ray.traceDepth + 1;
		r.origin += EPSILON * N;
		vec3 yay = TraceRay(r);
		if (r.hit != NULL)
			return yay * (float)(!r.hit->light) * ray.hit->Color(loc) / PI;
		//Skybox
		return yay * ray.hit->Color(loc) / PI;
	}
	else
	{
		float alpha = 5000 * ray.hit->material->ref;
		vec3 reflectDir = ray.direction - 2.f * (glm::dot(ray.direction, N) * N);
		Ray r(loc, WorldToLocal(glm::normalize(PhongBRDF(alpha)), reflectDir));
		r.traceDepth = ray.traceDepth + 1;
		r.origin += EPSILON * N;
		float dot = fmax(0.f,glm::dot(r.direction, -ray.direction));
		vec3 matcol = ray.hit->Color(loc);
		vec3 mult = matcol*(alpha + 2) / (2 * PI)*pow(dot, alpha);
		return mult == vec3() ? mult : mult * this->TraceRay(r);
	}
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

glm::vec3 Renderer::WorldToLocal(vec3 world, vec3 N)
{
	vec3 W;
	/*if (abs(N.x) > .95)
		W = vec3(1, 0, 0);
	else*/
		W = vec3(0, 0, 1);
	vec3 T = glm::normalize(glm::cross(N,W));
	vec3 B = glm::cross(T,N);
	return vec3(
		glm::dot(world, T),
		glm::dot(world, B),
		glm::dot(world, N));
}

glm::vec3 Renderer::PhongBRDF()
{
	float r1 = Randamonium();
	float r2 = Randamonium();
	float temp = 2 * PI * r1;
	float temp2 = sqrtf(1 - r2);
	return vec3(
		cos(temp) * temp2,
		sin(temp) * temp2,
		sqrtf(r2));
}

glm::vec3 Renderer::PhongBRDF(float alpha)
{
	float r1 = Randamonium();
	float t = pow(Randamonium(),2/(alpha+1));

	float temp = 2 * PI * r1;
	float temp2 = sqrtf(1 - t);
	return vec3(
		cos(temp) * temp2,
		sin(temp) * temp2,
		sqrtf(t));
}

float Renderer::Randamonium()
{
	seed ^= seed << 13;
	seed ^= seed >> 17;
	seed ^= seed << 5;
	return (float)seed * 2.3283064365387e-10f;
}