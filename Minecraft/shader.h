#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
private:
	GLuint id;
	int versionMajor;
	int versionMinor;

	std::string loadShaderSrc(const std::string& file)
	{

		std::string temp = "";
		std::string src = "";
		std::fstream inFile;

		inFile.open(file);
		if (inFile.is_open())
		{
			while (std::getline(inFile, temp))
				src += temp + "\n";
		}
		else
		{
			std::cout << "Error opening " << file << " file" << std::endl;
		}
		inFile.close();

		std::string version = std::to_string(versionMajor) + std::to_string(versionMinor) + "0";
		src.replace(src.find("#version"), 12, ("#version " + version));

		return src;
	}

	GLuint loadShader(GLenum type, const std::string& file)
	{
		char infoLog[512];
		GLint success;


		GLuint shader = glCreateShader(type);
		std::string src = loadShaderSrc(file);
		const GLchar* shSrc = src.c_str();
		glShaderSource(shader, 1, &shSrc, NULL);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "Error compiling shader " << file << std::endl;
			std::cout << infoLog << std::endl;
		}

		return shader;
	}

	void linkProgram(GLuint vs, GLuint gs, GLuint fs)
	{
		char infoLog[512];
		GLint success;

		id = glCreateProgram();
		glAttachShader(id, vs);
		if (gs) glAttachShader(id, gs);
		glAttachShader(id, fs);

		glLinkProgram(id);

		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "Error linking program" << std::endl;
			std::cout << infoLog << std::endl;
			glDeleteShader(vs);
			glDeleteShader(fs);
		}


	}

public:
	Shader(const int versionMajor, const int versionMinor, const std::string& vf, const std::string& ff, const std::string& gf = "")
		: versionMinor(versionMinor), versionMajor(versionMajor)
	{
		GLuint vs = 0;
		GLuint gs = 0;
		GLuint fs = 0;

		vs = loadShader(GL_VERTEX_SHADER, vf);
		if (gf != "") gs = loadShader(GL_GEOMETRY_SHADER, gf);
		fs = loadShader(GL_FRAGMENT_SHADER, ff);

		linkProgram(vs, gs, fs);

		glDeleteShader(vs);
		if (gs) glDeleteShader(gs);
		glDeleteShader(fs);
	}

	~Shader()
	{
		glDeleteProgram(id);
	}

	void use()
	{
		glUseProgram(id);
	}

	void unUse()
	{
		glUseProgram(0);
	}

	GLuint getAttribLoc(const std::string& name)
	{
		use();
		return glGetAttribLocation(id, name.c_str());
		unUse();
	}

	void setVec4f(const std::string& name, glm::fvec4 value)
	{
		use();
		glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
		unUse();
	}

	void setVec3f(const std::string& name, glm::fvec3 value)
	{
		use();
		glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
		unUse();
	}

	void setVec2f(const std::string& name, glm::fvec2 value)
	{
		use();
		glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value));
		unUse();
	}

	void setVec1f(const std::string& name, GLfloat value)
	{
		use();
		glUniform1f(glGetUniformLocation(id, name.c_str()), value);
		unUse();
	}

	void setMat4fv(const std::string& name, glm::mat4 value, GLboolean trans = false)
	{
		use();
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, trans, glm::value_ptr(value));
		unUse();
	}

	void setMat3fv(const std::string& name, glm::mat3 value, GLboolean trans = false)
	{
		use();
		glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, trans, glm::value_ptr(value));
		unUse();
	}

	void set1i(const std::string& name, GLuint value)
	{
		use();
		glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
		unUse();
	}
};