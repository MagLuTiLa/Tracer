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
	Material::Material(glm::vec3 c) : Material(0.f, c)
	{}

	// Used for reflective (0-1) and see through materials (1>)
	Material::Material(float r, glm::vec3 c) :
		ref(r)
	{
		texture = new glm::vec3[1]{ c };
	};

	Material::Material(char* filename) : Material(0, filename)
	{};

	Material::Material(float r, char* filename) : 
		ref(r)
	{
		readBMP(filename);
	}

	inline bool IsOpaque()
	{
		return (ref == 0);
	}

	inline bool IsReflective()
	{
		return (ref > 0 && ref <= 1);
	}

	inline bool IsRefractive()
	{
		return (ref > 1);
	}

	inline bool IsSingleColor()
	{
		return width == 0;
	}

	inline glm::vec3 Color(const glm::vec2 &uv)
	{
		//return glm::vec3(uv.x < 0, uv.y<0, 0);

		if (width == 0)
			return texture[0];
		return texture[(int)(uv.x * 0.99999 * width) + (int)(uv.y * 0.99999 * height) * width];
	}

	void readBMP(char* filename)
	{
		FILE* f = fopen(filename, "rb");
		unsigned char info[54];
		fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

												   // extract image height and width from header
		width = *(int*)&info[18];
		height = *(int*)&info[22];

		int size = 3 * width * height;
		unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
		fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
		fclose(f);
		
		texture = new glm::vec3[width*height];
		for (int i = 0, j = 0; i < size; i += 3, j++)
		{
			texture[j].r = (float)data[i + 2]/256.f;
			texture[j].g = (float)data[i + 1]/256.f;
			texture[j].b = (float)data[i]/256.f;
		}
		delete[] data;
	}

	glm::vec3* texture;
	unsigned int width = 0, height = 0;
	float ref;
};

static Material* BASE_MATERIAL = new Material(glm::vec3(1.0f, 1.0f, 1.0f));