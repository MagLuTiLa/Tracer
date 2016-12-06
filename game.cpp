#include "template.h"
#include "game.h"
#include "primitive.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "obj.h"
#include <ppl.h>
#include <string>

#define PARALLEL

Game::Game() :
	camera()
{
}

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init()
{
	AddPrimitive(new Sphere(vec3(0, 0, 2), 0.5f, Material(1.5f, vec3(1., 1., 1.))));
	AddPrimitive(new Sphere(vec3(1, 1, 4), 1.f, Material(vec3(0., 1., 0.))));
	AddPrimitive(new Sphere(vec3(-3, -1, 5), 1.f, Material(vec3(1., 0., 0.))));
	AddPrimitive(new Sphere(vec3(0, -3, 5), 1.f, Material(vec3(0., 1., 0.))));
	AddPrimitive(new Sphere(vec3(3, -1, 5), 1.f, Material(vec3(0., 0., 1.))));

	AddPrimitive(new Plane(vec3(0, 5, 5), vec3(0, -1, 0), Material(vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(0, 0, 7), vec3(0, 0, -1), Material(0.9f, vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(0, 0, -2), vec3(0, 0, 1), Material(0.9f, vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(5, 0, 0), vec3(-1, 0, 0), Material(0.9f, vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(-5, 0, 0), vec3(1, 0, 0), Material(0.9f, vec3(1., 1., 1.))));
	AddPrimitive(new Plane(vec3(0, -5, 0), vec3(0, 1, 0), Material(vec3(1., 1., 1.))));

	AddLight(new PointLight(vec3(0, 0, 0), vec3(20.f, 20.f, 20.f)));
	//AddLight(new PointLight(vec3(-3, -5, 3), vec3(9.f, 1.f, 1.f)));
	AddLight(new PointLight(vec3(3, -3, 3), vec3(1.f, 9.f, 1.f)));
	
	/*
	LoadObj("box.obj", primitives, mat4(1, 0, 0, 0,
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
			0, 0, 0, 1));*/
}

// -----------------------------------------------------------
// Input handling
// -----------------------------------------------------------
void Game::HandleInput( float dt )
{
}

// -----------------------------------------------------------
// Main game tick function
// -----------------------------------------------------------
void Game::Tick( float dt )
{
	timer t;
	char textBuffer[20];

	// Clear the screen
	screen->Clear( 0 );

	// Iterate over pixels
#ifdef PARALLEL
	concurrency::parallel_for(0, SCRHEIGHT, [&](int y)
#else
	for (int y = 0; y < SCRHEIGHT; y += 1)
#endif

	{
		for (int x = 0; x < SCRWIDTH; x += 1)
		{
			
			if (x == 570 && y == 327)
				int a = 1;
				
			float u = (float)x / SCRWIDTH;
			float v = (float)y / SCRHEIGHT;

			Ray ray = camera.ShootRay(u, v);

			vec3 lightIntensity = TraceRay(ray);

			screen->Plot(x, y, lightIntensity);
		}
	}
#ifdef PARALLEL
	);
#endif
	int time = t.elapsed();

	sprintf(textBuffer, "PlotTime: %ims", time);
	screen->Print(textBuffer, 2, 2, 0xffffff);

	sprintf(textBuffer, "Mouse X: %i", mouseX );
	screen->Print(textBuffer, 2, 12, 0xffffff);
	sprintf(textBuffer, "Mouse Y: %i", mouseY);
	screen->Print(textBuffer, 2, 22, 0xffffff);
	sprintf(textBuffer, "Key down: %i", keyDown);
	screen->Print(textBuffer, 2, 32, 0xffffff);
}

glm::vec3 Tmpl8::Game::TraceRay(Ray& ray)
{
	// See if ray intersects with primitives
	for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
	{
		Primitive* p = primitives[i];
		vec3 locthis = p->location;
		p->Intersect(ray);
	}

	vec3 lightIntensity = vec3();

	// If ray collided with sphere
	if (ray.length < std::numeric_limits<float>::max())
	{
		if (ray.traceDepth >= MAXTRACEDEPTH)
			return DirectIllumination(ray);
		Material material = ray.hit->material;
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

glm::vec3 Tmpl8::Game::DirectIllumination(Ray ray)
{
	vec3 lightIntensity = vec3();

	// Draw a shadow ray towards the light sources
	glm::vec3 rayPos = ray.origin + ray.direction * (ray.length - EPSILON);

	for (std::vector<Light>::size_type i = 0; i != lights.size(); i++)
	{
		Light* l = lights[i];
		float len = glm::length(l->location - rayPos);
		Ray shadowRay(rayPos, (l->location - rayPos) / len);

		// See if shadow ray intersects with primitives
		for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
		{
			Primitive* p = primitives[i];
			p->Intersect(shadowRay);
			if (shadowRay.length < len)
				goto next;
		}
		
		// Add color to the lightintensity
		shadowRay.length = len;
		shadowRay.color = l->color;
		lightIntensity += ray.hit->Sample(ray, shadowRay);
	next:;
	}
	return lightIntensity;
}

glm::vec3 Tmpl8::Game::Reflect(Ray ray)
{
	vec3 lightIntensity = vec3();
	vec3 rayPos = ray.origin + ray.direction * (ray.length - EPSILON);

	//?? = ?? ? 2(?? ? ??)??.
	vec3 colPos = ray.origin + ray.direction * ray.length;
	vec3 normal = ray.hit->Normal(colPos);
	vec3 rayDir = ray.direction - 2.f * (glm::dot(ray.direction, normal) * normal);

	Ray newRay(rayPos, rayDir);
	newRay.traceDepth = ray.traceDepth + 1;
	vec3 light = TraceRay(newRay);
	lightIntensity = light * ray.hit->Color();
	return lightIntensity;
}

glm::vec3 Tmpl8::Game::Refract(Ray ray, float from, float to)
{
	// Determine reflection and refraction part
	vec3 lightIntensity = vec3();

	float fdt = from / to;
	vec3 D = ray.direction;
	vec3 colPos = ray.origin + D * ray.length;
	vec3 N = ray.hit->Normal(colPos);
	if (ray.inside)
		N *= -1;

	float cosTheta = dot(N, -D);

	// K calculation
	float l = (fdt * fdt);
	float r = (1 - cosTheta * cosTheta);
	float k = 1 - l * r;

	if (k < 0)
	{
		lightIntensity = vec3(0.f, 1.f, 0.f);
	}
	else
	{
		vec3 rayPos = ray.origin + ray.direction * (ray.length) - N * EPSILON;

		vec3 d1 = fdt * D;
		vec3 d2 = N * ((fdt) * cosTheta - sqrtf(k));
		vec3 rayDir = d1 + d2;
		Ray newRay(rayPos, rayDir);
		newRay.traceDepth = ray.traceDepth + 1;
		if (ray.hit->hax == 1)
			newRay.inside = !ray.inside;
		else
			newRay.inside = ray.inside;
		vec3 light = TraceRay(newRay);
		lightIntensity = light * ray.hit->Color();
	}
	/*float fractR = (from - to) / (from + to);
	float R = fractR * fractR;*/


	return lightIntensity;
}

void Tmpl8::Game::AddPrimitive(Primitive * p)
{
	primitives.push_back(p);
}

void Tmpl8::Game::AddLight(Light * l)
{
	lights.push_back(l);
}

void Tmpl8::Game::KeyDown(int a_Key)
{
	float translate = 0.05f;
	float rotate = 1.f;
	float stretch = 0.01f;
	float zoom = 0.01f;

	switch (a_Key)
	{

	// Up
	case 82:
		camera.Pitch(rotate);
		break;

	// Down
	case 81:
		camera.Pitch(-rotate);
		break;

	// Left
	case 79:
		camera.Jaw(-rotate);
		break;

	// Right
	case 80:
		camera.Jaw(rotate);
		break;

	// W
	case 26:
		camera.Axial(translate);
		break;

	// A
	case 4:
		camera.Horizontal(-translate);
		break;

	// S
	case 22:
		camera.Axial(-translate);
		break;

	// D
	case 7:
		camera.Horizontal(translate);
		break;

	// Q
	case 20:
		camera.Vertical(-translate);
		break;

	// E
	case 8:
		camera.Vertical(translate);
		break;

	// U
	case 24:
		camera.VerStretch(-stretch);
		break;

	// H
	case 11:
		camera.HorStretch(stretch);
		break;

	// J
	case 13:
		camera.VerStretch(stretch);
		break;

	// K
	case 14:
		camera.HorStretch(-stretch);
		break;

	// Y
	case 28:
		camera.Zoom(-zoom);
		break;

	// I
	case 12:
		camera.Zoom(zoom);
		break;

	// R
	case 21:
		camera.Reset();
		break;
	}

	keyDown = a_Key;
}

void Game::MouseMove(int _X, int _Y)
{
	mouseX = _X;
	mouseY = _Y;
}