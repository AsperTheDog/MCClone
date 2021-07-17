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
	glm::vec3 speed;

	glm::vec3 worldUp;
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 frontMovement;
	glm::vec3 right;
	glm::vec3 up;

	GLfloat pitch;
	GLfloat yaw;
	GLfloat roll;
	bool inFloor;

	void updateCameraVectors()
	{
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontMovement.x = cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		frontMovement.z = sin(glm::radians(yaw));

		front = glm::normalize(front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}

public:
	Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 worldUp)
	{
		viewMatrix = glm::mat4(1.0f);
		movementSpeed = 8.0f;
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

	void updateGravity(const float& dt, bool inFloor) {
		if (inFloor and speed.y <= 0) {
			speed.y = 0;
		}else if (speed.y > -70) {
			speed.y -= 9.8 * (dt) * 4;
		}
		pos += speed * (dt);
		this->inFloor = inFloor;
	}

	void move(const float& dt, const int dir)
	{
		switch (dir)
		{
		case FORW:
			pos += frontMovement * movementSpeed * dt;
			break;
		case BACKW:
			pos -= frontMovement * movementSpeed * dt;
			break;
		case LEFT:
			pos -= right * movementSpeed * dt;
			break;
		case RIGHT:
			pos += right * movementSpeed * dt;
			break;
		case UP:
			if (inFloor) {
				speed.y = 12;
			}
			//pos += worldUp * movementSpeed * dt;
			break;
		case DOWN:
			//pos -= worldUp * movementSpeed * dt;
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

	glm::vec3 getFront() {
		return front;
	}
};