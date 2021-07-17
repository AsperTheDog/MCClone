#pragma once

#include <iostream>
#include <vector>

#include "Material.h"
#include "Texture.h"
#include "Vertex.h"
#include "Shader.h"


class Mesh
{
private:
	std::shared_ptr<Vertex[]> vArray;
	std::shared_ptr<GLuint[]> iArray;
	unsigned vertexNum;
	unsigned indexNum;

	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 modelMatrix;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	std::shared_ptr<Shader> coreProgram;

	void initVAO()
	{
		glCreateVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(Vertex), this->vArray.get(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexNum * sizeof(GLuint), this->iArray.get(), GL_STATIC_DRAW);

		GLuint attribLoc = coreProgram->getAttribLoc("position");
		glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(attribLoc);

		attribLoc = coreProgram->getAttribLoc("texCoord");
		glVertexAttribPointer(attribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(attribLoc);

		attribLoc = coreProgram->getAttribLoc("clr");
		glVertexAttribPointer(attribLoc, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, clr));
		glEnableVertexAttribArray(attribLoc);
		//attribLoc = coreProgram->getAttribLoc("normal");
		//glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		//glEnableVertexAttribArray(attribLoc);

		glBindVertexArray(0);
	}

	void updateUniforms()
	{
		updateModelMatrix();
		coreProgram->setMat4fv("ModelMatrix", modelMatrix);
	}

	void updateModelMatrix()
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, origin);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		modelMatrix = glm::translate(modelMatrix, position - origin);
		modelMatrix = glm::scale(modelMatrix, scale);
	}

public:
	Mesh(Vertex* vArray, const unsigned& vertexNum, GLuint* iArray, const unsigned& indexNum, std::shared_ptr<Shader> shader)
	{
		coreProgram = shader;
		position = glm::vec3(0.0f);
		this->origin = glm::vec3(0.0f);
		rotation = glm::vec3(0.0f);
		scale = glm::vec3(1.0f);

		this->vertexNum = vertexNum;
		this->indexNum = indexNum;

		this->vArray = std::shared_ptr<Vertex[]>(new Vertex[vertexNum]);
		for (size_t i = 0; i < vertexNum; i++)
		{
			this->vArray[i] = vArray[i];
		}

		this->iArray = std::shared_ptr<GLuint[]>(new GLuint[indexNum]);
		for (size_t i = 0; i < indexNum; i++)
		{
			this->iArray[i] = iArray[i];
		}

		initVAO();
		updateModelMatrix();
	}

	Mesh(const Mesh& obj)
	{
		coreProgram = obj.coreProgram;
		position = obj.position;
		origin = obj.origin;
		rotation = obj.rotation;
		scale = obj.scale;

		this->vertexNum = obj.vertexNum;
		this->indexNum = obj.indexNum;

		this->vArray = std::shared_ptr<Vertex[]>(new Vertex[vertexNum]);
		for (size_t i = 0; i < obj.vertexNum; i++)
		{
			this->vArray[i] = obj.vArray[i];
		}

		this->iArray = std::shared_ptr<GLuint[]>(new GLuint[indexNum]);
		for (size_t i = 0; i < obj.indexNum; i++)
		{
			this->iArray[i] = obj.iArray[i];
		}

		initVAO();
		updateModelMatrix();
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		if (indexNum > 0)
		{
			glDeleteBuffers(1, &EBO);
		}
	}

	void setPos(const glm::vec3 pos)
	{
		this->position = pos;
	}

	void setOrig(const glm::vec3 pos)
	{
		origin = pos;
	}

	void move(const glm::vec3 pos)
	{
		this->position += pos;
	}

	void setRotation(const glm::vec3 rot)
	{
		this->rotation = rot;
	}

	void rotate(const glm::vec3 rot)
	{
		this->rotation += rot;
	}

	void setScale(const glm::vec3 scale)
	{
		this->scale = scale;
	}

	void rescale(const glm::vec3 scale)
	{
		this->scale += scale;
	}

	void update()
	{
		
	}

	void render() 
	{
		updateUniforms();
		coreProgram->use();
		glBindVertexArray(VAO);
		if (indexNum == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexNum);
		}
		else
		{
			glDrawElements(GL_TRIANGLES, indexNum, GL_UNSIGNED_INT, 0);
		}
	}
};