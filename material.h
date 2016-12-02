#pragma once

#define REFRACTIVE_GLASS = 1.46f;
#define REFRACTIVE_WATER = 1.333f;
#define REFRACTIVE_AIR = 1.f;

#define AIR2GLASS = REFRACTIVE_GLASS / REFRACTIVE_AIR;
#define GLASS2AIR = REFRACTIVE_AIR / REFRACTIVE_GLASS;

#define AIR2WATER = REFRACTIVE_WATER / REFRACTIVE_AIR;
#define WATER2AIR = REFRACTIVE_AIR / REFRACTIVE_WATER;

class Material
{
public:
	// Used for opaque materials
	Material::Material(glm::vec3 c) :
		reflection(0),
		refraction(0),
		color(c)
	{};
	
	// Used for reflective, non-see through materials
	Material::Material(float refl, glm::vec3 c) :
		reflection(refl),
		refraction(0),
		color(c)
	{};

	// Used for see-through materials
	Material::Material(float refl, float refr, glm::vec3 c) :
		reflection(refl),
		refraction(refr),
		color(c)
	{};

	bool IsOpaque()
	{
		return (reflection == 0 && refraction == 0);
	}

	bool IsReflective()
	{
		return (reflection > 0 && refraction == 0);
	}

	bool IsRefractive()
	{
		return (refraction > 0);
	}

	glm::vec3 color;
	float reflection;
	float refraction;
};

