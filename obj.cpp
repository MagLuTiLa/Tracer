#include "template.h"
#include "trianglemesh.h"
#include "obj.h"
#include <fstream>
#include <sstream>


int LoadObj(const char* fileName, std::vector<Primitive*> &primitives, const glm::mat4 &trans, Triangle** triangleOut)
{
	std::string line;
	ifstream f;
	f.open(fileName);
	std::vector<glm::vec3> vertices;
	std::vector<int> vertexIndices;
	while (!f.eof())
	{
		std::getline(f, line);
		if (line.length() == 0)
			continue;
		
		switch (line[0])
		{
		case 'v':
		{
			if (line[1] != ' ')
				break;
			size_t off = 1;
			string sub = line.substr(off);
			float f1 = stof(sub, &off);
			sub = sub.substr(off);
			float f2 = stof(sub, &off);
			sub = sub.substr(off);
			vertices.push_back(glm::vec3(glm::vec4(f1, f2, stof(sub), 1) * trans));
			break;
		}
		case 'f':
		{
			string sub = line.substr(1);
			size_t off;
			vertexIndices.push_back(stoi(sub, &off));
			sub = sub.substr(sub.find(' ', off));
			vertexIndices.push_back(stoi(sub, &off));
			sub = sub.substr(sub.find(' ', off));
			vertexIndices.push_back(stoi(sub, &off));
		}
		}
	}
	Triangle* triangles = new Triangle[vertexIndices.size() / 3];
	if(triangleOut != NULL)
		*triangleOut = triangles;
	for (unsigned int i = 0, j = 0; j < vertexIndices.size(); i++, j+=3)
	{
		triangles[i].location = vertices[vertexIndices[j] - 1];
		triangles[i].location2 = vertices[vertexIndices[j + 1] - 1];
		triangles[i].location3 = vertices[vertexIndices[j + 2] - 1];
		triangles[i].CalculateNormal();
		primitives.push_back(&triangles[i]);
	}
	return vertexIndices.size() / 3;
}
