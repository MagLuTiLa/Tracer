#pragma once

#include "primitive.h"
#include "camera.h"
#include "pointlight.h"
#include "renderer.h"

#define SCRWIDTH	   1280
#define SCRHEIGHT	    800
#define MAXTRACEDEPTH    10
#define EPSILON     0.0001f

namespace Tmpl8 {

class Surface;
class Game
{
public:
	Game();

	void SetTarget( Surface* _Surface ) { screen = _Surface; }
	void Init();
	void Shutdown() { /* implement if you want code to be executed upon app exit */ };
	void HandleInput( float dt );
	void Tick( float dt );

	void MouseUp( int _Button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int _Button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove(int _X, int _Y);
	void KeyUp( int a_Key ) { /* implement if you want to handle keys */ }
	void KeyDown(int a_Key);
private:
	Surface* screen;
	Renderer renderer;
	Camera camera;

	int mouseX = 0;
	int mouseY = 0;
	int keyDown = 0;
	int bvhConstructTime = 0;
};

}; // namespace Tmpl8