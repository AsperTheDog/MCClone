#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Vertex.h"
#include "Mesh.h"
#include "Model.h"

#define PI 3.14159265

static struct VertexInd
{
	GLuint ind;
	short indCount;
};

static struct polyVertex
{
	Vertex v;
	int ind;

	bool isEqual(polyVertex p2)
	{
		return p2.v.position.x == this->v.position.x &&
			   p2.v.position.y == this->v.position.y &&
			   p2.v.position.z == this->v.position.z;
	}

	std::string prt()
	{
		return std::to_string(v.position.x) + ", " + std::to_string(v.position.y) + ", " + std::to_string(v.position.z) + " (" + std::to_string(ind) + ")";
	}
};


static Model* loadOBJ(const std::string& filename, glm::vec3 pos, Material* mat, Texture* texDiff, Texture* texSpec, Shader* shader)
{
	std::vector<glm::fvec3> vertexPos;
	std::vector<glm::fvec2> vertexTexCoord;
	std::vector<glm::fvec3> vertexNormal;

	std::vector<VertexInd> vertexPosIndices;
	std::vector<GLuint> vertexNormalIndices;
	std::vector<GLuint> vertexTexCoordIndices;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	std::stringstream ss;
	std::ifstream file(filename);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 tempv3;
	glm::vec2 tempv2;
	GLuint tempi = 0;
	int lCount = 0;

	bool isQuad = false;

	std::vector<Mesh*> meshes;
	Mesh* tmpMesh;

	if (!file.is_open())
	{
		throw "Error trying to load OBJ file " + filename;
		return nullptr;
	}

	while (std::getline(file, line))
	{
		lCount++;
		ss.clear();
		ss.str(line);
		ss >> prefix;
		if (prefix == "o")
		{
			if (vertexPos.size() != 0)
			{
				vertices.resize(vertexPosIndices.size(), Vertex());
				//indices.resize(vertices.size() / 4 * 6, 0);

				for (size_t i = 0; i < vertices.size(); i++)
				{
					vertices[i].position = vertexPos[vertexPosIndices[i].ind - 1];
					if (vertexTexCoord.size() == 0)
						vertices[i].texCoord = glm::vec2(0.0f);
					else
						vertices[i].texCoord = vertexTexCoord[vertexTexCoordIndices[i] - 1];
					if (vertexNormal.size() == 0)
						vertices[i].normal = glm::vec3(0.0f);
					else
						vertices[i].normal = vertexNormal[vertexNormalIndices[i] - 1];
					vertices[i].color = glm::vec3(1.0f, 1.0f, 1.0f);
				}
				int i = 0;
				while (i < vertexPosIndices.size())
				{
// 					triangulate(indices, &vertices[i], vertexPosIndices[i].indCount, i);
// 					i += vertexPosIndices[i].indCount;
 					if (vertexPosIndices[i].indCount > 3)
 					{
 						indices.push_back(0 + i);
 						indices.push_back(1 + i);
 						indices.push_back(2 + i);
 
 						indices.push_back(2 + i);
 						indices.push_back(3 + i);
 						indices.push_back(0 + i);
 						i += 4;
 					}
 					else
 					{
 						indices.push_back(0 + i);
 						indices.push_back(1 + i);
 						indices.push_back(2 + i);
 						i += 3;
 					}
				}

				tmpMesh = new Mesh(vertices.data(), vertices.size(), indices.data(), indices.size(), shader);
				tmpMesh->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
				tmpMesh->setScale(glm::vec3(0.1f));
				meshes.push_back(tmpMesh);

				vertexPosIndices.clear();
				vertexNormalIndices.clear();
				vertexTexCoordIndices.clear();

				vertices.clear();
				indices.clear();
			}
		}

		if (prefix == "v")
		{
			ss >> tempv3.x >> tempv3.y >> tempv3.z;
			vertexPos.push_back(tempv3);
		}

		else if (prefix == "vt")
		{
			ss >> tempv2.x >> tempv2.y;
			vertexTexCoord.push_back(tempv2);
		}

		else if (prefix == "vn")
		{
			ss >> tempv3.x >> tempv3.y >> tempv3.z;
			vertexNormal.push_back(tempv3);
		}

		else if (prefix == "f")
		{
			std::stringstream ln(line);
			std::string segment;
			std::vector<std::string> seglist;

			while (std::getline(ln, segment, ' '))
			{
				if (segment != "f" && segment != "")
				{
					seglist.push_back(segment);
				}
			}

			short n = seglist.size();
			int count = 0;
			int vertCount = 0;
			while (ss >> tempi)
			{
				if (vertCount > 3)
				{
					break;
				}
				switch(count)
				{
				case 0:
					vertexPosIndices.push_back({ tempi, n });
					break;
				case 1:
					vertexTexCoordIndices.push_back(tempi);
					break;
				case 2:
					vertexNormalIndices.push_back(tempi);
					break;
				default:
					break;
				}

				if (ss.peek() == '/')
				{
					count++;
					ss.ignore(1, '/');
				}
				else if (ss.peek() == ' ')
				{
					count = 0;
					ss.ignore(1, ' ');
					vertCount++;
				}
			}
		}
	}

	if (vertexPos.size() != 0)
	{
		vertices.resize(vertexPosIndices.size(), Vertex());
		indices.resize(vertices.size() / 4 * 6, 0);

		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].position = vertexPos[vertexPosIndices[i].ind - 1];
			if (vertexTexCoord.size() == 0)
			{
				vertices[i].texCoord = glm::vec2(0.0f);
			}
			else
			{
				vertices[i].texCoord = vertexTexCoord[vertexTexCoordIndices[i] - 1];
			}
			if (vertexNormal.size() == 0)
			{
				vertices[i].normal = glm::vec3(0.0f);
			}
			else
			{
				vertices[i].normal = vertexNormal[vertexNormalIndices[i] - 1];
			}
			
			vertices[i].color = glm::vec3(1.0f, 1.0f, 1.0f);
		}
		int i = 0;
		while (i < vertexPosIndices.size())
		{
// 			triangulate(indices, &vertices[i], vertexPosIndices[i].indCount, i);
// 			i += vertexPosIndices[i].indCount;
 			if (vertexPosIndices[i].indCount > 3)
 			{
 				indices.push_back(0 + i);
 				indices.push_back(1 + i);
 				indices.push_back(2 + i);
 
 				indices.push_back(2 + i);
 				indices.push_back(3 + i);
 				indices.push_back(0 + i);
 				i += 4;
 			}
 			else
 			{
 				indices.push_back(0 + i);
 				indices.push_back(1 + i);
 				indices.push_back(2 + i);
 				i += 3;
 			}
		}

		tmpMesh = new Mesh(vertices.data(), vertices.size(), indices.data(), indices.size(), shader);
		tmpMesh->setPos(glm::vec3(0.0f, 0.0f, 0.0f));
		tmpMesh->setScale(glm::vec3(0.01f));
		meshes.push_back(tmpMesh);
	}
	Model* md = new Model(pos, mat, texDiff, texSpec, meshes);
	std::cout << "OBJ file " << filename << " loaded" << std::endl;
	for (Mesh* i : meshes)
	{
		delete i;
	}
	file.close();
	return md;
}
