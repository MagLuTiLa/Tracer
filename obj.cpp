#include "template.h"
#include "obj.h"
#include <fstream>
#include <sstream>


void LoadObj(const char* fileName, std::vector<Primitive*> &primitives, const glm::mat4 &trans)
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
	for (int i = 0; i < vertexIndices.size(); i += 3)
		primitives.push_back(new Triangle(vertices[vertexIndices[i] - 1], vertices[vertexIndices[i + 1] - 1], vertices[vertexIndices[i + 2] - 1]));
}


