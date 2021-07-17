#pragma once

#include "libs.h"
#include "Chunk.h"
#include <thread>
#include <mutex>
#include <unordered_map>

struct pair_hash {
	std::size_t operator () (const glm::vec2& p) const {
		auto h1 = std::hash<int>{}(p.x);
		auto h2 = std::hash<int>{}(p.y);

		// Mainly for demonstration purposes, i.e. works but is overly simple
		// In the real world, use sth. like boost.hash_combine
		return h1 ^ h2;
	}
};

class Game
{
private:
	std::thread chunkLoader;
	GLFWwindow* window;
	GLint fbwidth;
	GLint fbheight;

	float fov;
	float nearPlane;
	float farPlane;

	float dt;
	float curTime;
	float lastTime;
	int nbFrames;
	double lastTimeFPS;

	double lastMouseX;
	double lastMouseY;
	double mouseX;
	double mouseY;
	double mouseOffsetX;
	double mouseOffsetY;
	bool firstMouse;

	Camera cam;

	std::vector<std::shared_ptr<Shader>> shaders;
	std::unordered_map<uint8_t, std::vector<uint8_t>> blockData;
	std::unordered_map<uint8_t, std::shared_ptr<Texture>> textures;
	std::vector<std::shared_ptr<Material>> materials;
	std::unordered_map<glm::vec2, std::vector<std::shared_ptr<Model>>, pair_hash> models;
	std::vector< std::shared_ptr<Chunk>> chunks;
	std::vector<glm::vec2> loadedChunks;
	std::vector< std::shared_ptr<PointLight>> pointLights;
	std::vector<uint8_t> clrList;

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	glm::vec3 cameraPos;
	glm::vec3 camFront;
	glm::vec3 worldUp;

	std::shared_ptr<glm::vec2> lastChunk;
	std::vector<glm::vec2> notLoadedChunks;
	std::vector<std::shared_ptr<Chunk>> finishedChunks;
	std::vector<Chunk*> chunksToUpdate;

	void initWindow(const std::string& title, int width, const int height, int GLmayor, int GLminor, bool resizable);
	void initMatrices();
	void configureOpenGL();
	void initShaders(int majVer, int minVer);
	void initTextures();
	void initPointLights();
	void initLights();
	void initUniforms();
	void updateUniforms();
	void addChunk();
	void loadChunks();
	float calculateDistance(glm::vec2 coor);

public:
	Game(const std::string& title, int width, const int height, int GLmayor, int GLminor, bool resizable, float fov, float nearPlane, float farPlane);
	~Game();

	void update();
	void render();
	bool getWinShouldClose();
	void closeWindow();
	void updateKeyboardInput();
	void updateMouseInput();
	void updateInput();
	void updateDt();
};

