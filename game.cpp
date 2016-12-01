#include "template.h"
#include "game.h"
#include "primitive.h"
#include "sphere.h"
#include "plane.h"
#include <string>

Game::Game() :
	camera(vec3(0, 0, 0), vec3(0, 0, 10), 0.5f)
{
	Primitive * p1 = new Sphere(vec3(0, 0, 5), 1.0f);
	primitives.push_back(p1);

	p1 = new Sphere(vec3(1, 2, 3), 1.0f, vec3(0., 0., 1.));
	primitives.push_back(p1);

	p1 = new Sphere(vec3(2, 0, 4), 0.8f, vec3(1., 0., 0.));
	primitives.push_back(p1);
	
	p1 = new Sphere(vec3(-2, -3, 5), 1.f, Material(vec3(1., 1., 1.)));
	primitives.push_back(p1);
	
	p1 = new Plane(vec3(0, 5, 5), vec3(0, -1, 0));
	primitives.push_back(p1);
	
	Light * l = new PointLight(vec3(-1, -3, -1), vec3(7.f, 7.f, 7.f));
	lights.push_back(l);
	
	l = new PointLight(vec3(-3, -5, 3), vec3(9.f, 9.f, 9.f));
	lights.push_back(l);
	
	l = new PointLight(vec3(3, -3, 3), vec3(1.f, 9.f, 1.f));
	lights.push_back(l);
}

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init()
{
	// TODO place camera, primitives, lights here
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
	// Clear the screen
	screen->Clear( 0 );

	// Iterate over pixels
	for (float y = 0.0f; y < SCRHEIGHT; y += 1.0f)
		for (float x = 0.0f; x < SCRWIDTH; x += 1.0f)
		{
			
			if ((int)x == 565 && (int)y == 250)
				int a = 1;

			float u = x / SCRWIDTH;
			float v = y / SCRHEIGHT;

			Ray ray = camera.ShootRay(u, v);

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
				lightIntensity = DirectIllumination(ray);

			screen->Plot(x, y, lightIntensity);
		}
	
	screen->Print( "ab!<>", 2, 2, 0xffffff );
	char textBuffer [20];
	sprintf(textBuffer, "Mouse X: %i", mouseX );
	screen->Print(textBuffer, 2, 12, 0xffffff);
	sprintf(textBuffer, "Mouse Y: %i", mouseY);
	screen->Print(textBuffer, 2, 22, 0xffffff);
}

glm::vec3 Tmpl8::Game::DirectIllumination(Ray ray)
{
	vec3 lightIntensity = vec3();

	// Draw a shadow ray towards the light sources
	glm::vec3 rayPos = ray.direction * (ray.length - 0.0001f);

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

		// If shadow ray did not intersect

		// Draw the pixel
		// TODO: Consider color of the primitive that is collided with
		shadowRay.length = len;
		shadowRay.color = l->color;
		lightIntensity += ray.hit->Sample(ray, shadowRay);
	next:;
	}
	return lightIntensity;
}

void Tmpl8::Game::KeyDown(int a_Key)
{
	if (a_Key == 82)
		camera.MoveForward();
	if (a_Key == 81)
		camera.MoveBack();
	/*
	char text [50];
	sprintf(text, "%i", a_Key);
	screen->Print(text, 2, 12, 0xffffff);*/
}


void Game::MouseMove(int _X, int _Y)
{
	mouseX = _X;
	mouseY = _Y;
}