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
	Material::Material(glm::vec3 c) : Material(0.f, c)
	{};
	
	// Used for reflective, non-see through materials
	Material::Material(float refl, glm::vec3 c) : Material(0.f,0.f,c)
	{};

	// Used for see-through materials
	Material::Material(float refl, float refr, glm::vec3 c) :
		reflection(refl),
		refraction(refr)
	{
		texture = new glm::vec3[1]{ c };
	};

	inline bool IsOpaque()
	{
		return ((reflection == 0) & (refraction == 0));
	}

	inline bool IsReflective()
	{
		return ((reflection > 0) & (refraction == 0));
	}

	bool IsRefractive()
	{
		return (refraction > 0);
	}

	glm::vec3 Color(const glm::vec2 &uv)
	{
		return texture[(int)(uv.x * 0.99999 * width) + (int)(uv.y * 0.99999 * height) * width];
	}

	glm::vec3* texture;
	unsigned int width = 1, height = 1;
	float reflection;
	float refraction;
};

