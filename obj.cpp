#include "template.h"
#include "obj.h"
#include <fstream>
#include <sstream>


int LoadObj(const char* fileName, std::vector<Primitive*> &primitives, Material* mat, const glm::mat4 &trans, Triangle** triangleOut)
{
	glm::mat3 normalTrans = glm::transpose(glm::inverse(mat3(trans)));

	std::string line;
	ifstream f;
	f.open(fileName);
	std::vector<glm::vec3> vertices;
	std::vector<int> vertexIndices;

	std::vector<glm::vec2> uv;
	std::vector<int> uvIndices;

	std::vector<glm::vec3> normals;
	std::vector<int> normalIndices;

	while (!f.eof())
	{
		std::getline(f, line);
		if (line.length() == 0)
			continue;

		switch (line[0])
		{
		case 'v':
		{
			if (line[1] == 't')
			{
				size_t off = 2;
				string sub = line.substr(off);
				float f1 = stof(sub, &off);
				sub = sub.substr(off);
				uv.push_back(glm::vec2(f1, stof(sub)));
				break;
			}
			if (line[1] == 'n')
			{
				size_t off = 2;
				string sub = line.substr(off);
				float f1 = stof(sub, &off);
				sub = sub.substr(off);
				float f2 = stof(sub, &off);
				sub = sub.substr(off);
				normals.push_back(glm::vec3(glm::vec3(f1, f2, stof(sub)) * normalTrans));
				break;
			}
			if (line[1] == ' ')
			{
				size_t off = 1;
				string sub = line.substr(off);
				float f1 = stof(sub, &off);
				sub = sub.substr(off);
				float f2 = stof(sub, &off);
				sub = sub.substr(off);
				vertices.push_back(glm::vec3(glm::vec4(f1, f2, stof(sub), 1) * trans));
				break;
			}
		}
		case 'f':
		{
			string sub = line.substr((size_t)1);
			size_t off = 0;
			for (int i = 0; i < 3; i++)
			{
				vertexIndices.push_back(stoi(sub, &off));
				sub = sub.substr(off);
				if (sub[0] == '/')
				{
					sub = sub.substr(1);
					if (sub[0] != '/')
					{
						uvIndices.push_back(stoi(sub, &off));
						sub = sub.substr(off);
					}
					else
						uvIndices.push_back(0);

					if (sub[0] == '/')
					{
						sub = sub.substr(1);
						normalIndices.push_back(stoi(sub, &off));
						sub = sub.substr(off);
					}
					else
						normalIndices.push_back(0);
				}
				else
				{
					uvIndices.push_back(0);
					normalIndices.push_back(0);
				}
			}
		}
		}
	}
	Triangle* triangles = new Triangle[vertexIndices.size() / 3];
	if(triangleOut != NULL)
		*triangleOut = triangles;
	for (unsigned int i = 0, j = 0; j < vertexIndices.size(); i++, j+=3)
	{
		triangles[i].material = mat;

		triangles[i].location = vertices[vertexIndices[j] - 1];
		triangles[i].location2 = vertices[vertexIndices[j + 1] - 1];
		triangles[i].location3 = vertices[vertexIndices[j + 2] - 1];

		if(normalIndices[j] == 0)
			triangles[i].CalculateNormal();
		else
			triangles[i].normal = glm::normalize((normals[normalIndices[j] - 1] + normals[normalIndices[j+1] - 1] + normals[normalIndices[j+2] - 1]) / 3.f);
		
		if (uvIndices[j] == 0)
		{
			if (!mat->IsSingleColor())
			{
				triangles[i].uv = new vec2[3];
				triangles[i].uv[0] = vec2(0, 0);
				triangles[i].uv[1] = vec2(1,0);
				triangles[i].uv[2] = vec2(0,1);
			}
		}
		else
		{
			triangles[i].uv = new vec2[3];
			triangles[i].uv[0] = uv[uvIndices[j] - 1];
			triangles[i].uv[1] = uv[uvIndices[j + 1] - 1];
			triangles[i].uv[2] = uv[uvIndices[j + 2] - 1];
		}

		primitives.push_back(&triangles[i]);
	}
	return vertexIndices.size() / 3;
}
