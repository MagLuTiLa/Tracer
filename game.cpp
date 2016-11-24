#include "template.h"
#include "game.h"
#include "primitive.h"
#include "sphere.h"

Game::Game() :
	camera(vec3(0, 0, 0), vec3(0, 0, 1), 1.0f),
	pl(vec3(-3, -3, 3), vec3(0.9f, 0.9f, 0.9f))
{
	Primitive * p1 = new Sphere(vec3(0, 0, 5), 1.0f);
	primitives.push_back(p1);

	p1 = new Sphere(vec3(1, 2, 3), 1.0f);
	primitives.push_back(p1);

	/*p1 = new Sphere(vec3(0, 0, 5), 1.0f);
	primitives.push_back(p1);*/
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
	//screen->Clear( 0 );

	// Camera
	

	// One sphere
	// TODO replace with vector of primitives
	//Sphere sphere (vec3(0, 0, 5), 1.0f);
	//sphere.location += vec3(0.01f, -0.01f, -0.01f);

	// One poinlight 
	// TODO replace with vector of pointlights
	//PointLight pl(vec3(0,0,5), 0x999999);

	// Iterate over pixels
	for (float y = 0.0f; y < SCRHEIGHT; y += 1.0f)
		for (float x = 0.0f; x < SCRWIDTH; x += 1.0f)
		{
			
			float u = x / SCRWIDTH;
			float v = y / SCRHEIGHT;

			if ((int)x == SCRWIDTH / 2 && (int)y == SCRHEIGHT / 2)
				int a = 0;

			Ray ray = camera.ShootRay(u, v);

			if ((int)x == SCRWIDTH / 2 && (int)y == SCRHEIGHT / 2)
				int a = 0;

			// See if ray intersects with sphere
			// TODO iterate over vector of primitives and only work with closest intersection
			for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
			{
				Primitive* p = primitives[i];
				vec3 locthis = p->location;
				p->Intersect(ray);
			}

			// If ray collided with sphere
			if (ray.length < std::numeric_limits<float>::max())
			{

				// Draw a shadow ray towards the light source
				// TODO iterate over vector of light sources and sum up light influences
				glm::vec3 rayPos = ray.direction * (ray.length - 0.0001f);
				Ray shadowRay (rayPos, glm::normalize(pl.location - rayPos));

				// See if shadow ray intersects with sphere
				// TODO iterate over vector of primitives and break upon first intersection (if intersection distance is smaller than distance to light source)
				for (std::vector<Primitive>::size_type i = 0; i != primitives.size(); i++)
				{
					Primitive* p = primitives[i];
					p->Intersect(shadowRay);
				}

				// If shadow ray did not intersect
				if (shadowRay.length == std::numeric_limits<float>::max())
				{
					// Draw the pixel
					// TODO: Consider color of the primitive that is collided with
					screen->Plot(x, y, pl.color);
					continue;
				}
			}
			screen->Plot(x, y, 0x000000);
		}
	
	screen->Print( "ab!<>", 2, 2, 0xffffff );
	/*screen->Plot(20, 20, 0xff0000);
	screen->Bar(30, 30, 100, 50, 0x555555);
	screen->Box(120, 30, 140, 50, 0x779977);
	screen->Line(3 * SCRWIDTH / 4, 0, 1 * SCRWIDTH / 4, SCRHEIGHT - 1, 0x0000dd);
	screen->Line(SCRWIDTH / 2, 0, SCRWIDTH / 2, SCRHEIGHT - 1, 0x00dd00);
	screen->Line(0, SCRHEIGHT / 2, SCRWIDTH - 1, SCRHEIGHT / 2, 0xdd0000);
	*/
}