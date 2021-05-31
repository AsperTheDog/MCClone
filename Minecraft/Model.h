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
	std::shared_ptr<Mesh> mesh;
	glm::vec3 position;

public:
	Model(glm::vec3 pos, std::shared_ptr<Material> mat, std::shared_ptr<Texture> texDiff, std::shared_ptr<Mesh> mesh)
		: position(pos), mat(mat), texDiff(texDiff), mesh(mesh)
	{
		this->mesh->move(position);
		this->mesh->setOrig(position);
	}

	void resetMeshes(std::shared_ptr<Mesh> mesh) {
		this->mesh = mesh;

		this->mesh->move(position);
		this->mesh->setOrig(position);
	}

	void rotate(const glm::vec3 rotation)
	{
		this->mesh->rotate(rotation);
		this->mesh->setOrig(position);
	}

	void resize(const glm::vec3 size)
	{
		this->mesh->setScale(size);
	}

	void render(std::shared_ptr<Shader> shader)
	{
		mat->sendToShader(*shader);

		shader->use();
		texDiff->bind(0);

		this->mesh->render();

		glBindVertexArray(0);
		shader->unUse();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};
