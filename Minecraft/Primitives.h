#pragma once

#include <vector>
#include <glew.h>
#include <glfw3.h>

#include "Vertex.h"

class Primitive
{
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
public:
	Primitive()
	{

	}

	virtual ~Primitive()
	{

	}

	void set(const Vertex* vertices, const unsigned vertexNum, const GLuint* indices, const unsigned indexNum)
	{
		for (int i = 0; i < vertexNum; i++)
		{
			this->vertices.push_back(vertices[i]);
		}

		for (int i = 0; i < indexNum; i++)
		{
			this->indices.push_back(indices[i]);
		}
	}

	inline Vertex* getVertices() { return this->vertices.data(); }
	inline GLuint* getIndices() { return this->indices.data(); }
	inline const unsigned getVertexNum() { return vertices.size(); }
	inline const unsigned getIndexNum() { return indices.size(); }
};

class Quad : public Primitive 
{
public:
	Quad()
	{
		Vertex vertices[] =
		{
			glm::vec3(-0.5f,  0.5f,  0.0f),	glm::vec2(0.0f, 1.0f), 1,
			glm::vec3(-0.5f, -0.5f,  0.0f),	glm::vec2(0.0f, 0.0f), 1,
			glm::vec3(0.5f, -0.5f,  0.0f),	glm::vec2(1.0f, 0.0f), 1,
			glm::vec3(0.5f,  0.5f,  0.0f),	glm::vec2(1.0f, 1.0f), 1
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

		GLuint indices[] =
		{
			0,  1,  2,
			0,  2,  3,
		};
		unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

		this->set(vertices, nrOfVertices, indices, nrOfIndices);
	}
};


class Pyramid : public Primitive
{
public:
	Pyramid()
		: Primitive()
	{
		Vertex vertices[] =
		{
			//Position								//Texcoords					
			//Triangle front
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec2(0.5f, 1.f),		1,
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec2(0.f, 0.f),		1,
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec2(1.f, 0.f),		1,

			//Triangle left
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec2(0.5f, 1.f),		1,
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec2(0.f, 0.f),		1,
			glm::vec3(-0.5f, -0.5f, 0.5f),			glm::vec2(1.f, 0.f),		1,

			//Triangle back
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec2(0.5f, 1.f),		1,
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec2(0.f, 0.f),		1,
			glm::vec3(-0.5f, -0.5f, -0.5f),			glm::vec2(1.f, 0.f),		1,

			//Triangles right
			glm::vec3(0.f, 0.5f, 0.f),				glm::vec2(0.5f, 1.f),		1,
			glm::vec3(0.5f, -0.5f, 0.5f),			glm::vec2(0.f, 0.f),		1,
			glm::vec3(0.5f, -0.5f, -0.5f),			glm::vec2(1.f, 0.f),		1,
		};
		unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);
		
		this->set(vertices, nrOfVertices, nullptr, 0);
	}
};