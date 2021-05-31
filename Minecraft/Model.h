#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"

class Model
{
private:
	std::shared_ptr<Material> mat;
	std::shared_ptr<Texture> texDiff;
	std::vector<std::shared_ptr<Mesh>> meshes;
	glm::vec3 position;

	void updateUniforms()
	{

	}

public:
	Model(glm::vec3 pos, std::shared_ptr<Material> mat, std::shared_ptr<Texture> texDiff, std::vector<std::shared_ptr<Mesh>> meshes)
	{
		this->position = pos;
		this->mat = mat;
		this->texDiff = texDiff;

		for (auto& i : meshes)
		{
			this->meshes.push_back(std::shared_ptr<Mesh>(new Mesh(*i)));
		}

		for (auto& i : this->meshes)
		{
			i->move(position);
			i->setOrig(position);
		}
	}

	~Model()
	{
		
	}

	void resetMeshes(std::vector<std::shared_ptr<Mesh>> meshes) {
		this->meshes.clear();

		for (auto& i : meshes)
		{
			this->meshes.push_back(std::shared_ptr<Mesh>(new Mesh(*i)));
		}

		for (auto& i : this->meshes)
		{
			i->move(position);
			i->setOrig(position);
		}
	}

	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : meshes)
		{
			i->rotate(rotation);
			i->setOrig(position);
		}
	}

	void resize(const glm::vec3 size)
	{
		for (auto& i : meshes)
		{
			i->setScale(size);
		}
	}

	void update()
	{

	}

	void render(std::shared_ptr<Shader> shader)
	{
		updateUniforms();

		mat->sendToShader(*shader);

		shader->use();
		texDiff->bind(0);

		for (auto& i : meshes)
		{
			i->render();
		}

		glBindVertexArray(0);
		shader->unUse();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
