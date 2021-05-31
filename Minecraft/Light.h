#pragma once

#include <glm.hpp>

#include "Shader.h"

class Light
{
protected:
	float intensity;
	glm::vec3 color;

public:
	Light(float intensity, glm::vec3 color)
	{
		this->intensity = intensity;
		this->color = color;
	}

	~Light()
	{

	}

	virtual void sendToShader(Shader& program) = 0;
};

class PointLight : public Light
{
protected:
	glm::vec3 pos;
	float constant;
	float linear;
	float cuadratic;

public:
	PointLight(glm::vec3 position, float intensity = 5.0f, glm::vec3 color = glm::vec3(1.0f), float constant = 1.0f, float linear = 0.0045f, float cuadratic = 0.00075f) : Light(intensity, color)
	{
		pos = position;
		this->constant = constant;
		this->linear = linear;
		this->cuadratic = cuadratic;
	}

	~PointLight()
	{

	}

	void setPos(const glm::vec3 position)
	{
		pos = position;
	}

	void sendToShader(Shader& program)
	{
		program.setVec3f("pointLight.position", pos);
		program.setVec1f("pointLight.intensity", intensity);
		program.setVec3f("pointLight.color", color);
		program.setVec1f("pointLight.constant", constant);
		program.setVec1f("pointLight.linear", linear);
		program.setVec1f("pointLight.cuadratic", cuadratic);
	}
};