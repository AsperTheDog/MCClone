#pragma once
#include <vector>
#include <glew.h>
#include <stdlib.h>
#include "vertex.h"
#include "Mesh.h"
#include "Model.h"
#include <unordered_map>
#include <math.h>

class Chunk {
	std::vector<uint8_t> fileIds;
	std::unordered_map<uint8_t, std::vector<Vertex>> blocks;
	std::unordered_map<uint8_t, std::vector<GLuint>> indices;
	std::unordered_map<uint8_t, unsigned short> counts;
	std::unordered_map<uint8_t, std::vector<uint8_t>>* blockData;
	std::vector<std::shared_ptr<Chunk>>* chunks;
	std::unordered_map<uint8_t, std::shared_ptr<Model>> models;
	std::vector<uint8_t>* clrList;
	Chunk* neighbors[4] = { nullptr, nullptr, nullptr, nullptr };
	std::shared_ptr<Shader> shader;
	int xC, yC, zC;
	unsigned short size;
	int seed;
	std::vector<int> modPos;

	int getCoordIndex(const int x, const int y, const int z) {
		return x + y * size + z * size * size;
	}

	void createTerrain() {
		fileIds.reserve(zC * size * size * sizeof(uint8_t));
		for (int z = 0; z < zC; z++) {
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					uint8_t r = abs(std::min(1, rand() % 30) - 1);
					fileIds.push_back(r);
				}
			}
		}
	}

	void getBorderData(const unsigned int index, uint8_t* borderData, Chunk** neighbors) {
		int ix = index % size;
		int iy = floor((index / size) % size);
		int iz = floor(index / (size * size));
		borderData[0] = 1;
		borderData[1] = 1;
		borderData[2] = 1;
		borderData[3] = 1;
		if (neighbors[0] && ix == 0) {
			borderData[0] = neighbors[0]->fileIds.at(getCoordIndex(size - 1, iy, iz));
		}
		if (neighbors[1] && ix == size - 1) {
			borderData[1] = neighbors[1]->fileIds.at(getCoordIndex(0, iy, iz));
		}
		if (neighbors[2] && iy == 0) {
			borderData[2] = neighbors[2]->fileIds.at(getCoordIndex(ix, size - 1, iz));
		}
		if (neighbors[3] && iy == size - 1) {
			borderData[3] = neighbors[3]->fileIds.at(getCoordIndex(ix, 0, iz));
		}
	}

	void reviewSides(const unsigned int index, uint8_t* borderData, Chunk** neighbors)
	{
		unsigned short id = fileIds.at(index);
		int ix = index % size;
		int iy = floor((index / size) % size);
		int iz = floor(index / (size * size));
		int x = ix + size * xC;
		int y = iy + size * yC;
		int z = iz;
		unsigned short texId;
		float clr;
		if (id != 0 && (ix == 0 || ix == size - 1 || iy == 0 || iy == size - 1)){
			getBorderData(index, borderData, neighbors);
		}

		if ((ix == size - 1 && borderData[1] == 0) || (ix != size - 1 && fileIds.at(index + 1) == 0)) {
			texId = blockData->at(id)[1];
			if (std::find(clrList->begin(), clrList->end(), texId) != clrList->end()) {
				clr = 0.0;
			}
			else {
				clr = 1.0;
			}
			blocks[texId].push_back({ glm::vec3(x + 1, z, y), glm::vec2(0, 0), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z + 1, y), glm::vec2(0, 1), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z, y + 1), glm::vec2(1, 0), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z + 1, y + 1), glm::vec2(1, 1), clr });

			indices[texId].push_back(counts.at(texId));
			indices[texId].push_back(counts.at(texId) + 1);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 3);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 1);
			counts.at(texId) += 4;
		}

		if ((ix == 0 && borderData[0] == 0) || (ix != 0 && fileIds.at(index - 1) == 0)) {
			texId = blockData->at(id)[1];
			if (std::find(clrList->begin(), clrList->end(), texId) != clrList->end()) {
				clr = 0.0;
			}
			else {
				clr = 1.0;
			}
			blocks[texId].push_back({ glm::vec3(x, z, y), glm::vec2(0, 0), clr });
			blocks[texId].push_back({ glm::vec3(x, z, y + 1), glm::vec2(1, 0), clr });
			blocks[texId].push_back({ glm::vec3(x, z + 1, y), glm::vec2(0, 1), clr });
			blocks[texId].push_back({ glm::vec3(x, z + 1, y + 1), glm::vec2(1, 1), clr });

			indices[texId].push_back(counts.at(texId));
			indices[texId].push_back(counts.at(texId) + 1);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 3);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 1);
			counts.at(texId) += 4;
		}

		if ((iy == size - 1 && borderData[3] == 0) || (iy != size - 1 && fileIds.at(index + size) == 0)) {
			texId = blockData->at(id)[1];
			if (std::find(clrList->begin(), clrList->end(), texId) != clrList->end()) {
				clr = 0.0;
			}
			else {
				clr = 1.0;
			}
			blocks[texId].push_back({ glm::vec3(x, z, y + 1), glm::vec2(0, 0), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z, y + 1), glm::vec2(1, 0), clr });
			blocks[texId].push_back({ glm::vec3(x, z + 1, y + 1), glm::vec2(0, 1), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z + 1, y + 1), glm::vec2(1, 1), clr });

			indices[texId].push_back(counts.at(texId));
			indices[texId].push_back(counts.at(texId) + 1);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 3);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 1);
			counts.at(texId) += 4;
		}

		if ((iy == 0 && borderData[2] == 0) || (iy != 0 && fileIds.at(index - size) == 0)) {
			texId = blockData->at(id)[1];
			if (std::find(clrList->begin(), clrList->end(), texId) != clrList->end()) {
				clr = 0.0;
			}
			else {
				clr = 1.0;
			}
			blocks[texId].push_back({ glm::vec3(x, z, y), glm::vec2(0, 0), clr });
			blocks[texId].push_back({ glm::vec3(x, z + 1, y), glm::vec2(0, 1), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z, y), glm::vec2(1, 0), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z + 1, y), glm::vec2(1, 1), clr });

			indices[texId].push_back(counts.at(texId));
			indices[texId].push_back(counts.at(texId) + 1);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 3);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 1);
			counts.at(texId) += 4;
		}

		if (iz == zC - 1 || fileIds.at(index + size * size) == 0) {
			texId = blockData->at(id)[2];
			if (std::find(clrList->begin(), clrList->end(), texId) != clrList->end()) {
				clr = 0.0;
			}
			else {
				clr = 1.0;
			}
			blocks[texId].push_back({ glm::vec3(x, z + 1, y), glm::vec2(0, 0), clr });
			blocks[texId].push_back({ glm::vec3(x, z + 1, y + 1), glm::vec2(0, 1), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z + 1, y), glm::vec2(1, 0), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z + 1, y + 1), glm::vec2(1, 1), clr });

			indices[texId].push_back(counts.at(texId));
			indices[texId].push_back(counts.at(texId) + 1);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 3);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 1);
			counts.at(texId) += 4;
		}

		if (iz == 0 || fileIds.at(index - size * size) == 0) {
			texId = blockData->at(id)[0];
			if (std::find(clrList->begin(), clrList->end(), texId) != clrList->end()) {
				clr = 0.0;
			}
			else {
				clr = 1.0;
			}
			blocks[texId].push_back({ glm::vec3(x, z, y), glm::vec2(0, 0), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z, y), glm::vec2(0, 1), clr });
			blocks[texId].push_back({ glm::vec3(x, z, y + 1), glm::vec2(1, 0), clr });
			blocks[texId].push_back({ glm::vec3(x + 1, z, y + 1), glm::vec2(1, 1), clr });

			indices[texId].push_back(counts.at(texId));
			indices[texId].push_back(counts.at(texId) + 1);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 3);
			indices[texId].push_back(counts.at(texId) + 2);
			indices[texId].push_back(counts.at(texId) + 1);
			counts.at(texId) += 4;
		}
	}

	void loadChunk(glm::vec2 erasedCoords, bool erase) {
		neighbors[0] = nullptr;
		neighbors[1] = nullptr;
		neighbors[2] = nullptr;
		neighbors[3] = nullptr;
		uint8_t count = 0;
		for (auto& ch : *chunks) {
			if (erase and ch->getCoords() == erasedCoords) {
				continue;
			}
			if (abs(ch->xC - xC) + abs(ch->yC - yC) == 1) {
				if (ch->xC - xC == -1) {
					neighbors[0] = ch.get();
					count++;
				}
				else if (ch->xC - xC == 1) {
					neighbors[1] = ch.get();
					count++;
				}
				else if (ch->yC - yC == -1) {
					neighbors[2] = ch.get();
					count++;
				}
				else if (ch->yC - yC == 1) {
					neighbors[3] = ch.get();
					count++;
				}
				if (count == 4) {
					break;
				}
			}
		}

		uint8_t borderData[4];
		for (int i = 0; i < fileIds.size(); i++) {
			if (fileIds.at(i) != 0) {
				for (int j = 0; j < 3; j++) {
					if (blocks.find(blockData->at(fileIds.at(i))[j]) == blocks.end()) {
						blocks.emplace(blockData->at(fileIds.at(i))[j], std::vector<Vertex>());
						indices.emplace(blockData->at(fileIds.at(i))[j], std::vector<GLuint>());
						counts.emplace(blockData->at(fileIds.at(i))[j], 0);
					}
				}
				reviewSides(i, borderData, neighbors);
			}
		}
	}

	void updateModel() {
		for (std::pair<unsigned short, std::vector<Vertex>> elem : blocks) {
			std::shared_ptr<Mesh> m = std::shared_ptr<Mesh>(new Mesh(elem.second.data(), elem.second.size(), indices.at(elem.first).data(), indices.at(elem.first).size(), shader));
			std::vector <std::shared_ptr<Mesh>> mesh;
			mesh.push_back(m);
			models.at(elem.first)->resetMeshes(mesh);
		}
		blocks.clear();
		indices.clear();
		counts.clear();
	}

public:
	Chunk(const int cube, const unsigned int xCArg, const int yCArg, const int zCArg, const int sizeArg, std::unordered_map<uint8_t, std::vector<uint8_t>>* blockData, std::vector<uint8_t>* clrlist, std::vector<std::shared_ptr<Chunk>>* chunks, std::shared_ptr<Shader> shader)
		: xC(xCArg), yC(yCArg), zC(zCArg), size(sizeArg), chunks(chunks), clrList(clrlist), blockData(blockData), shader(shader)
	{	
		createTerrain();

		loadChunk(getCoords(), false);
	}

	~Chunk() {
		
	}

	inline bool operator==(glm::vec2& coor) {
		return coor == getCoords();
	}

	void reloadNeighbors(bool erase = false) {
		for (int i = 0; i < 4; i++) {
			if (neighbors[i]) {
				neighbors[i]->loadChunk(getCoords(), erase);
				neighbors[i]->updateModel();
			}
		}
	}

	std::unordered_map<uint8_t, std::shared_ptr<Model>> createModel(std::shared_ptr<Material> mat, std::unordered_map<uint8_t, std::shared_ptr<Texture>> texDiffs) {
		for (std::pair<unsigned short, std::vector<Vertex>> elem : blocks) {
			std::shared_ptr<Mesh> m = std::shared_ptr<Mesh>(new Mesh(elem.second.data(), elem.second.size(), indices.at(elem.first).data(), indices.at(elem.first).size(), shader));
			std::vector <std::shared_ptr<Mesh>> mesh;
			mesh.push_back(m);
			models.emplace(elem.first, std::shared_ptr<Model>(new Model(glm::vec3(0, 0, 0), mat, texDiffs.at(elem.first), mesh)));
		}
		blocks.clear();
		indices.clear();
		counts.clear();
		return models;
	}

	glm::vec2 getCoords() {
		return glm::vec2(xC, yC);
	}

	void updateModPos(int pos) {
		modPos.push_back(pos);
	}

	std::vector<int> getModPos() {
		return modPos;
	}
};