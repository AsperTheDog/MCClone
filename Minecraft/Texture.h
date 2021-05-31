#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <SOIL2.h>

class Texture
{
private:
	GLuint id;
	int height;
	int width;
public:
	Texture(const std::string& file, GLenum type) 
	{
		unsigned char* img = SOIL_load_image(file.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);

		glGenTextures(1, &id);
		glBindTexture(type, id);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		if (img)
		{
			glTexImage2D(type, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
			glGenerateMipmap(type);
		}
		else
		{
			std::cout << "Error while trying to load texture" << file << std::endl;
		}

		glActiveTexture(0);
		glBindTexture(type, 0);
		SOIL_free_image_data(img);
	}

	~Texture()
	{
		glDeleteTextures(1, &id);
	}

	void bind(GLint unit) 
	{
		glBindTextureUnit(unit, id);
	}
};

