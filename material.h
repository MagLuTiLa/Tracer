#pragma once

#define REFRACTIVE_GLASS = 1.46;
#define REFRACTIVE_WATER = 1.333;
#define REFRACTIVE_AIR = 1.0;

#define AIR2GLASS = REFRACTIVE_GLASS / REFRACTIVE_AIR;
#define GLASS2AIR = REFRACTIVE_AIR / REFRACTIVE_GLASS;

#define AIR2WATER = REFRACTIVE_WATER / REFRACTIVE_AIR;
#define WATER2AIR = REFRACTIVE_AIR / REFRACTIVE_WATER;

class Material
{
public:
	// Used for opaque materials
	Material::Material(glm::vec3 c) :
		ref(0),
		color(c)
	{};
	
	// Used for reflective (0-1) and see through materials (1>)
	Material::Material(float r, glm::vec3 c) :
		ref(r),
		color(c)
	{};

	bool IsOpaque()
	{
		return (ref == 0);
	}

	bool IsReflective()
	{
		return (ref > 0 && ref <= 1);
	}

	bool IsRefractive()
	{
		return (ref > 1);
	}

	glm::vec3 color;
	float ref;
};

