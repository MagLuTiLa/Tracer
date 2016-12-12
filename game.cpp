#include "template.h"
#include "game.h"
#include "primitive.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "obj.h"
#include "spotLight.h"
#include <ppl.h>
#include <string>

#define PARALLEL

Game::Game() :
	camera(), 
	renderer()
{
}

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
			if (x == 454 && y == 78)
				int a = 1;
				
			float u = (float)x / SCRWIDTH;
			float v = (float)y / SCRHEIGHT;

			Ray ray = camera.ShootRay(u, v);

			vec3 lightIntensity = renderer.TraceRay(ray);

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