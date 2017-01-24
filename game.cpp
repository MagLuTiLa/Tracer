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
#define SMOOTHTIME

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
	bvhConstructTime = renderer.Init();
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
	bufferCount++;
	// Iterate over pixels
#ifdef PARALLEL
	concurrency::parallel_for(0, SCRHEIGHT, [&](int y)
#else
	for (int y = 0; y < SCRHEIGHT; y += 1)
#endif

	{
		for (int x = 0; x < SCRWIDTH; x += 1)
		{
			if(x == mouseX && y == mouseY & !mouseDown)
				screen->Plot(x, y, vec3(1,0,0));
			if (x == mouseX && y == mouseY && mouseDown)
				int a = 1;
				
			float u = (float)x / SCRWIDTH;
			float v = (float)y / SCRHEIGHT;

			Ray ray;
			camera.ShootRay(u, v, ray);
			vec3 lightIntensity = renderer.TraceRay(ray);

			if(bufferCount == 1)
				buffer[y * SCRWIDTH + x] = lightIntensity;
			else
				buffer[y * SCRWIDTH + x] += lightIntensity;


			screen->Plot(x, y, buffer[y * SCRWIDTH + x]/(float)bufferCount);
		}
	}
#ifdef PARALLEL
	);
#endif
	
	sprintf(textBuffer, "BVH build: %ims", bvhConstructTime);
	screen->Print(textBuffer, 2, 2, 0xffffff);

	int time = (int)t.elapsed();
#ifdef SMOOTHTIME
	float a = 0.8;
	time = time * a + prevTime * (1 - a);
	prevTime = time;
#endif
	sprintf(textBuffer, "PlotTime: %ims", time);
	screen->Print(textBuffer, 2, 12, 0xffffff);

	sprintf(textBuffer, "Mouse X: %i", mouseX );
	screen->Print(textBuffer, 2, 22, 0xffffff);
	sprintf(textBuffer, "Mouse Y: %i", mouseY);
	screen->Print(textBuffer, 2, 32, 0xffffff);
	sprintf(textBuffer, "Key down: %i", keyDown);
	screen->Print(textBuffer, 2, 42, 0xffffff);
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
		bufferCount = 0;
		break;

	// Down
	case 81:
		camera.Pitch(-rotate);
		bufferCount = 0;
		break;

	// Left
	case 79:
		camera.Jaw(-rotate);
		bufferCount = 0;
		break;

	// Right
	case 80:
		camera.Jaw(rotate);
		bufferCount = 0;
		break;

	// W
	case 26:
		camera.Axial(translate);
		bufferCount = 0;
		break;

	// A
	case 4:
		camera.Horizontal(-translate);
		bufferCount = 0;
		break;

	// S
	case 22:
		camera.Axial(-translate);
		bufferCount = 0;
		break;

	// D
	case 7:
		camera.Horizontal(translate);
		bufferCount = 0;
		break;

	// Q
	case 20:
		camera.Vertical(-translate);
		bufferCount = 0;
		break;

	// E
	case 8:
		camera.Vertical(translate);
		bufferCount = 0;
		break;

	// U
	case 24:
		camera.VerStretch(-stretch);
		bufferCount = 0;
		break;

	// H
	case 11:
		camera.HorStretch(stretch);
		bufferCount = 0;
		break;

	// J
	case 13:
		camera.VerStretch(stretch);
		bufferCount = 0;
		break;

	// K
	case 14:
		camera.HorStretch(-stretch);
		bufferCount = 0;
		break;

	// Y
	case 28:
		camera.Zoom(-zoom);
		bufferCount = 0;
		break;

	// I
	case 12:
		camera.Zoom(zoom);
		bufferCount = 0;
		break;

	// R
	case 21:
		camera.Reset();
		bufferCount = 0;
		break;
	}

	keyDown = a_Key;
}

void Game::MouseMove(int _X, int _Y)
{
	mouseX = _X;
	mouseY = _Y;
}