// Template, major revision 6
// IGAD/NHTV - Jacco Bikker - 2006-2015

#pragma once

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE

#include "math.h"
#include "stdlib.h"
#include "emmintrin.h"
#include "stdio.h"
#include "windows.h"
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "surface.h"
#include "game.h"
#include <vector>
#include "freeimage.h"
#include "threads.h"

extern "C" 
{ 
#include "glew.h" 
}
#include "gl.h"
#include "io.h"
#include <ios>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "SDL.h"
#include "SDL_syswm.h"
#include "wglext.h"
#include "fcntl.h"

#include "sphere.h"
#include "pointlight.h"
#include "ray.h"
#include "camera.h"

using namespace Tmpl8;				// to use template classes
using namespace glm;				// to use glm vector stuff
using namespace std;				// to use stl vectors

inline float Rand( float range ) { return ((float)rand() / RAND_MAX) * range; }
inline int IRand( int range ) { return rand() % range; }
int filesize( FILE* f );
#define MALLOC64(x) _aligned_malloc(x,64)
#define FREE64(x) _aligned_free(x)

namespace Tmpl8 {

#define MIN(a,b) (((a)>(b))?(b):(a))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define PI					3.14159265358979323846264338327950288419716939937510582097494459072381640628620899862803482534211706798f

#define PREFETCH(x)			_mm_prefetch((const char*)(x),_MM_HINT_T0)
#define PREFETCH_ONCE(x)	_mm_prefetch((const char*)(x),_MM_HINT_NTA)
#define PREFETCH_WRITE(x)	_m_prefetchw((const char*)(x))
#define loadss(mem)			_mm_load_ss((const float*const)(mem))
#define broadcastps(ps)		_mm_shuffle_ps((ps),(ps), 0)
#define broadcastss(ss)		broadcastps(loadss((ss)))

	// turn those verbose intrinsics into something readable.
#define loadps(mem)		_mm_load_ps((const float * const)(mem))
#define storess(ss,mem)		_mm_store_ss((float * const)(mem),(ss))
#define minss			_mm_min_ss
#define maxss			_mm_max_ss
#define minps			_mm_min_ps
#define maxps			_mm_max_ps
#define mulps			_mm_mul_ps
#define subps			_mm_sub_ps
#define rotatelps(ps)		_mm_shuffle_ps((ps),(ps), 0x39)	// a,b,c,d -> b,c,d,a
#define muxhps(low,high)	_mm_movehl_ps((low),(high))	// low{a,b,c,d}|high{e,f,g,h} = {c,d,g,h}

static const float flt_plus_inf = -logf(0);	// let's keep C and C++ compilers happy.
static const __m128 _MM_ALIGN16
	q_plus_inf = _mm_set_ps( flt_plus_inf,  flt_plus_inf,  flt_plus_inf,  flt_plus_inf ),
	q_minus_inf = _mm_set_ps( -flt_plus_inf, -flt_plus_inf, -flt_plus_inf, -flt_plus_inf);

struct timer 
{ 
	typedef long long value_type; 
	static double inv_freq; 
	value_type start; 
	timer() : start( get() ) { init(); } 
	float elapsed() const { return (float)((get() - start) * inv_freq); } 
	static value_type get() 
	{ 
		LARGE_INTEGER c; 
		QueryPerformanceCounter( &c ); 
		return c.QuadPart; 
	} 
	static double to_time(const value_type vt) { return double(vt) * inv_freq; } 
	void reset() { start = get(); }
	static void init() 
	{ 
		LARGE_INTEGER f; 
		QueryPerformanceFrequency( &f ); 
		inv_freq = 1000./double(f.QuadPart); 
	} 
}; 

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned char byte;
void HandleEvents();
#define BADFLOAT(x) ((*(uint*)&x & 0x7f000000) == 0x7f000000)

}; // namespace Tmpl8