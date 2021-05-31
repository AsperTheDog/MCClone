#pragma once

#include <iostream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum direction {FORW = 0, BACKW, LEFT, RIGHT, UP, DOWN};

class Camera
{
private:
	glm::mat4 viewMatrix;

	GLfloat movementSpeed;
	GLfloat sensitivity;

	glm::vec3 worldUp;
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;

	void updateCameraVectors()
	{
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

public:
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 worldUp)
	{
		viewMatrix = glm::mat4(1.0f);
		movementSpeed = 20.0f;
		sensitivity = 8.0f;

		this->worldUp = worldUp;
		this->pos = pos;
		this->up = worldUp;

		right = glm::vec3(0.0f);

		pitch = 0.0f;
		yaw = -90.0f;
		roll = 0.0f;

		updateCameraVectors();
	}

	~Camera()
	{

	}

	const glm::mat4 getVM()
	{
		updateCameraVectors();
		viewMatrix = glm::lookAt(pos, pos + front, up);

		return viewMatrix;
	}

	const glm::vec3 getPos() const
	{
		return pos;
	}

	void move(const float& dt, const int dir)
	{
		switch (dir)
		{
		case FORW:
			pos += front * movementSpeed * dt;
			break;
		case BACKW:
			pos -= front * movementSpeed * dt;
			break;
		case LEFT:
			pos -= right * movementSpeed * dt;
			break;
		case RIGHT:
			pos += right * movementSpeed * dt;
			break;
		case UP:
			pos += worldUp * movementSpeed * dt;
			break;
		case DOWN:
			pos -= worldUp * movementSpeed * dt;
			break;
		default:
			break;
		};
	}

	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
	{
		pitch -= static_cast<GLfloat>(offsetY) * sensitivity * dt;
		yaw += static_cast<GLfloat>(offsetX) * sensitivity * dt;

		if (pitch > 80.0f)
			pitch = 80.0f;
		else if (pitch < -80.0f)
			pitch = -80.0f;

		if (yaw > 360.0f || yaw < -360.0f)
			yaw = 0.0f;
	}

	void updateInput(const float& dt, const int dir, const double& offsetX, const double &offsetY)
	{
		updateMouseInput(dt, offsetX, offsetY);
	}
};