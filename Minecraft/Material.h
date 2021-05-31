#pragma once

#include "Shader.h"

class Material
{
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLuint diffuseTex;
	GLuint specularTex;
public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, GLint diffuseTex, GLint specularTex)
		: ambient(ambient), diffuse(diffuse), specular(specular), diffuseTex(diffuseTex), specularTex(specularTex)
	{

	}

	~Material()
	{

	}

	void sendToShader(Shader& program)
	{
		program.setVec3f("material.ambient", ambient);
		program.setVec3f("material.diffuse", diffuse);
		program.setVec3f("material.specular", specular);
		program.set1i("material.diffuseTex", diffuseTex);
		program.set1i("material.specularTex", specularTex);
	}
};