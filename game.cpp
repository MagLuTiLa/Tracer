#include "template.h"
#include "game.h"

// -----------------------------------------------------------
// Initialize the game
// -----------------------------------------------------------
void Game::Init()
{
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
	screen->Clear( 0 );
	Camera camera (vec3(0, 0, 0), vec3(0, 0, 1), 1.0f);
	Sphere sphere (vec3(0, 0, 5), 1.0f);
	PointLight pl (vec3(-3, -3, 3), 0x999999);
	Ray ray = camera.ShootRay(0,0);
	for (float y = 0.0f; y < SCRHEIGHT; y += 1.0f)
		for (float x = 0.0f; x < SCRWIDTH; x += 1.0f)
		{
			float u = x / SCRWIDTH;
			float v = y / SCRHEIGHT;

			ray = camera.ShootRay(u, v);

			sphere.Intersect(ray);

			if (ray.length < std::numeric_limits<float>::max())
			{
				glm::vec3 rayPos = ray.direction * (ray.length - 0.0001f);
				Ray shadowRay (rayPos, glm::normalize(pl.location - rayPos), std::numeric_limits<float>::max());

				sphere.Intersect(shadowRay);
				if (shadowRay.length == std::numeric_limits<float>::max())
				{
					screen->Plot(x, y, 0xffffff);
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