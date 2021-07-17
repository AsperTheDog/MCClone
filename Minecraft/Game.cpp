#include "Game.h"
#include <fstream>
#include <string>
#include <iostream>
#include <Windows.h>

#define DISTANCE_SELECTION distancesFar
#define MAX_DISTANCE 20
#define CHUNK_SIZE 16

#define DISTANCE_SIZE *(&DISTANCE_SELECTION + 1) - DISTANCE_SELECTION

extern std::mutex mtx, renderMtx, updateMtx, reloadMtx;

glm::vec2 distancesNear[] = {
	{ 0,  0}, { 1,  0}, {-1,  0}, {-1, -1},
	{ 0, -1}, { 2,  0}, { 1,  1}, { 0,  1},
	{-1,  1}, {-2,  1}, {-2,  0}, {-2, -1},
	{-2, -2}, {-1, -2}, { 0, -2}, { 1, -2},
	{ 1, -1}, { 3,  0}, { 2,  1}, { 2,  2},
	{ 1,  2}, { 0,  2}, {-1,  2}, {-2,  2},
	{-3,  2}, {-3,  1}, {-3,  0}, {-3, -1},
	{-3, -2}, {-3, -3}, {-2, -3}, {-1, -3},
	{ 0, -3}, { 1, -3}, { 2, -3}, { 2, -2},
	{ 2, -1}, { 4,  0}, { 3,  1}, { 3,  2},
	{ 2,  3}, { 1,  3}, { 0,  3}, {-1,  3},
	{-2,  3}, {-3,  3}, {-4,  2}, {-4,  1},
	{-4,  0}, {-4, -1}, {-4, -2}, {-4, -3},
	{-3, -4}, {-2, -4}, {-1, -4}, { 0, -4},
	{ 1, -4}, { 2, -4}, { 3, -3}, { 3, -2},
	{ 3, -1}
};

glm::vec2 distancesMid[] = {
	{ 0,  0}, { 1,  0}, {-1,  0}, {-1, -1},
	{ 0, -1}, { 2,  0}, { 1,  1}, { 0,  1},
	{-1,  1}, {-2,  1}, {-2,  0}, {-2, -1},
	{-2, -2}, {-1, -2}, { 0, -2}, { 1, -2},
	{ 1, -1}, { 3,  0}, { 2,  1}, { 2,  2},
	{ 1,  2}, { 0,  2}, {-1,  2}, {-2,  2},
	{-3,  2}, {-3,  1}, {-3,  0}, {-3, -1},
	{-3, -2}, {-3, -3}, {-2, -3}, {-1, -3},
	{ 0, -3}, { 1, -3}, { 2, -3}, { 2, -2},
	{ 2, -1}, { 4,  0}, { 3,  1}, { 3,  2},
	{ 2,  3}, { 1,  3}, { 0,  3}, {-1,  3},
	{-2,  3}, {-3,  3}, {-4,  2}, {-4,  1},
	{-4,  0}, {-4, -1}, {-4, -2}, {-4, -3},
	{-3, -4}, {-2, -4}, {-1, -4}, { 0, -4},
	{ 1, -4}, { 2, -4}, { 3, -3}, { 3, -2},
	{ 3, -1}, { 5,  0}, { 4,  1}, { 4,  2},
	{ 3,  3}, { 2,  4}, { 1,  4}, { 0,  4},
	{-1,  4}, {-2,  4}, {-3,  4}, {-4,  3},
	{-5,  2}, {-5,  1}, {-5,  0}, {-5, -1},
	{-5, -2}, {-5, -3}, {-4, -4}, {-3, -5},
	{-2, -5}, {-1, -5}, { 0, -5}, { 1, -5},
	{ 2, -5}, { 3, -4}, { 4, -3}, { 4, -2},
	{ 4, -1}, { 6,  0}, { 5,  1}, { 5,  2},
	{ 5,  3}, { 4,  3}, { 4,  4}, { 3,  4},
	{ 3,  5}, { 2,  5}, { 1,  5}, { 0,  5},
	{-1,  5}, {-2,  5}, {-3,  5}, {-4,  5},
	{-4,  4}, {-5,  4}, {-5,  3}, {-6,  3},
	{-6,  2}, {-6,  1}, {-6,  0}, {-6, -1},
	{-6, -2}, {-6, -3}, {-6, -4}, {-5, -4},
	{-5, -5}, {-4, -5}, {-4, -6}, {-3, -6},
	{-2, -6}, {-1, -6}, { 0, -6}, { 1, -6},
	{ 2, -6}, { 3, -6}, { 3, -5}, { 4, -5},
	{ 4, -4}, { 5, -4}, { 5, -3}, { 5, -2},
	{ 5, -1}, { 7,  0}, { 6,  1}, { 6,  2},
	{ 6,  3}, { 5,  4}, { 4,  5}, { 3,  6},
	{ 2,  6}, { 1,  6}, { 0,  6}, {-1,  6},
	{-2,  6}, {-3,  6}, {-4,  6}, {-5,  5},
	{-6,  4}, {-7,  3}, {-7,  2}, {-7,  1},
	{-7,  0}, {-7, -1}, {-7, -2}, {-7, -3},
	{-7, -4}, {-6, -5}, {-5, -6}, {-4, -7},
	{-3, -7}, {-2, -7}, {-1, -7}, { 0, -7},
	{ 1, -7}, { 2, -7}, { 3, -7}, { 4, -6},
	{ 5, -5}, { 6, -4}, { 6, -3}, { 6, -2},
	{ 6, -1}
};

glm::vec2 distancesFar[] = {
	{  0,   0}, {  1,   0}, { -1,   0}, { -1,  -1},
	{  0,  -1}, {  2,   0}, {  1,   1}, {  0,   1},
	{ -1,   1}, { -2,   1}, { -2,   0}, { -2,  -1},
	{ -2,  -2}, { -1,  -2}, {  0,  -2}, {  1,  -2},
	{  1,  -1}, {  3,   0}, {  2,   1}, {  2,   2},
	{  1,   2}, {  0,   2}, { -1,   2}, { -2,   2},
	{ -3,   2}, { -3,   1}, { -3,   0}, { -3,  -1},
	{ -3,  -2}, { -3,  -3}, { -2,  -3}, { -1,  -3},
	{  0,  -3}, {  1,  -3}, {  2,  -3}, {  2,  -2},
	{  2,  -1}, {  4,   0}, {  3,   1}, {  3,   2},
	{  2,   3}, {  1,   3}, {  0,   3}, { -1,   3},
	{ -2,   3}, { -3,   3}, { -4,   2}, { -4,   1},
	{ -4,   0}, { -4,  -1}, { -4,  -2}, { -4,  -3},
	{ -3,  -4}, { -2,  -4}, { -1,  -4}, {  0,  -4},
	{  1,  -4}, {  2,  -4}, {  3,  -3}, {  3,  -2},
	{  3,  -1}, {  5,   0}, {  4,   1}, {  4,   2},
	{  3,   3}, {  2,   4}, {  1,   4}, {  0,   4},
	{ -1,   4}, { -2,   4}, { -3,   4}, { -4,   3},
	{ -5,   2}, { -5,   1}, { -5,   0}, { -5,  -1},
	{ -5,  -2}, { -5,  -3}, { -4,  -4}, { -3,  -5},
	{ -2,  -5}, { -1,  -5}, {  0,  -5}, {  1,  -5},
	{  2,  -5}, {  3,  -4}, {  4,  -3}, {  4,  -2},
	{  4,  -1}, {  6,   0}, {  5,   1}, {  5,   2},
	{  5,   3}, {  4,   3}, {  4,   4}, {  3,   4},
	{  3,   5}, {  2,   5}, {  1,   5}, {  0,   5},
	{ -1,   5}, { -2,   5}, { -3,   5}, { -4,   5},
	{ -4,   4}, { -5,   4}, { -5,   3}, { -6,   3},
	{ -6,   2}, { -6,   1}, { -6,   0}, { -6,  -1},
	{ -6,  -2}, { -6,  -3}, { -6,  -4}, { -5,  -4},
	{ -5,  -5}, { -4,  -5}, { -4,  -6}, { -3,  -6},
	{ -2,  -6}, { -1,  -6}, {  0,  -6}, {  1,  -6},
	{  2,  -6}, {  3,  -6}, {  3,  -5}, {  4,  -5},
	{  4,  -4}, {  5,  -4}, {  5,  -3}, {  5,  -2},
	{  5,  -1}, {  7,   0}, {  6,   1}, {  6,   2},
	{  6,   3}, {  5,   4}, {  4,   5}, {  3,   6},
	{  2,   6}, {  1,   6}, {  0,   6}, { -1,   6},
	{ -2,   6}, { -3,   6}, { -4,   6}, { -5,   5},
	{ -6,   4}, { -7,   3}, { -7,   2}, { -7,   1},
	{ -7,   0}, { -7,  -1}, { -7,  -2}, { -7,  -3},
	{ -7,  -4}, { -6,  -5}, { -5,  -6}, { -4,  -7},
	{ -3,  -7}, { -2,  -7}, { -1,  -7}, {  0,  -7},
	{  1,  -7}, {  2,  -7}, {  3,  -7}, {  4,  -6},
	{  5,  -5}, {  6,  -4}, {  6,  -3}, {  6,  -2},
	{  6,  -1}, {  8,   0}, {  7,   1}, {  7,   2},
	{  7,   3}, {  6,   4}, {  6,   5}, {  5,   5},
	{  5,   6}, {  4,   6}, {  3,   7}, {  2,   7},
	{  1,   7}, {  0,   7}, { -1,   7}, { -2,   7},
	{ -3,   7}, { -4,   7}, { -5,   6}, { -6,   6},
	{ -6,   5}, { -7,   5}, { -7,   4}, { -8,   3},
	{ -8,   2}, { -8,   1}, { -8,   0}, { -8,  -1},
	{ -8,  -2}, { -8,  -3}, { -8,  -4}, { -7,  -5},
	{ -7,  -6}, { -6,  -6}, { -6,  -7}, { -5,  -7},
	{ -4,  -8}, { -3,  -8}, { -2,  -8}, { -1,  -8},
	{  0,  -8}, {  1,  -8}, {  2,  -8}, {  3,  -8},
	{  4,  -7}, {  5,  -7}, {  5,  -6}, {  6,  -6},
	{  6,  -5}, {  7,  -4}, {  7,  -3}, {  7,  -2},
	{  7,  -1}, {  9,   0}, {  8,   1}, {  8,   2},
	{  8,   3}, {  8,   4}, {  7,   4}, {  7,   5},
	{  6,   6}, {  5,   7}, {  4,   7}, {  4,   8},
	{  3,   8}, {  2,   8}, {  1,   8}, {  0,   8},
	{ -1,   8}, { -2,   8}, { -3,   8}, { -4,   8},
	{ -5,   8}, { -5,   7}, { -6,   7}, { -7,   6},
	{ -8,   5}, { -8,   4}, { -9,   4}, { -9,   3},
	{ -9,   2}, { -9,   1}, { -9,   0}, { -9,  -1},
	{ -9,  -2}, { -9,  -3}, { -9,  -4}, { -9,  -5},
	{ -8,  -5}, { -8,  -6}, { -7,  -7}, { -6,  -8},
	{ -5,  -8}, { -5,  -9}, { -4,  -9}, { -3,  -9},
	{ -2,  -9}, { -1,  -9}, {  0,  -9}, {  1,  -9},
	{  2,  -9}, {  3,  -9}, {  4,  -9}, {  4,  -8},
	{  5,  -8}, {  6,  -7}, {  7,  -6}, {  7,  -5},
	{  8,  -5}, {  8,  -4}, {  8,  -3}, {  8,  -2},
	{  8,  -1}, { 10,   0}, {  9,   1}, {  9,   2},
	{  9,   3}, {  9,   4}, {  8,   5}, {  7,   6},
	{  7,   7}, {  6,   7}, {  5,   8}, {  4,   9},
	{  3,   9}, {  2,   9}, {  1,   9}, {  0,   9},
	{ -1,   9}, { -2,   9}, { -3,   9}, { -4,   9},
	{ -5,   9}, { -6,   8}, { -7,   7}, { -8,   7},
	{ -8,   6}, { -9,   5}, {-10,   4}, {-10,   3},
	{-10,   2}, {-10,   1}, {-10,   0}, {-10,  -1},
	{-10,  -2}, {-10,  -3}, {-10,  -4}, {-10,  -5},
	{ -9,  -6}, { -8,  -7}, { -8,  -8}, { -7,  -8},
	{ -6,  -9}, { -5, -10}, { -4, -10}, { -3, -10},
	{ -2, -10}, { -1, -10}, {  0, -10}, {  1, -10},
	{  2, -10}, {  3, -10}, {  4, -10}, {  5,  -9},
	{  6,  -8}, {  7,  -8}, {  7,  -7}, {  8,  -6},
	{  9,  -5}, {  9,  -4}, {  9,  -3}, {  9,  -2},
	{  9,  -1}, { 11,   0}, { 10,   1}, { 10,   2},
	{ 10,   3}, { 10,   4}, {  9,   5}, {  9,   6},
	{  8,   6}, {  8,   7}, {  7,   8}, {  6,   8},
	{  6,   9}, {  5,   9}, {  4,  10}, {  3,  10},
	{  2,  10}, {  1,  10}, {  0,  10}, { -1,  10},
	{ -2,  10}, { -3,  10}, { -4,  10}, { -5,  10},
	{ -6,   9}, { -7,   9}, { -7,   8}, { -8,   8},
	{ -9,   7}, { -9,   6}, {-10,   6}, {-10,   5},
	{-11,   4}, {-11,   3}, {-11,   2}, {-11,   1},
	{-11,   0}, {-11,  -1}, {-11,  -2}, {-11,  -3},
	{-11,  -4}, {-11,  -5}, {-10,  -6}, {-10,  -7},
	{ -9,  -7}, { -9,  -8}, { -8,  -9}, { -7,  -9},
	{ -7, -10}, { -6, -10}, { -5, -11}, { -4, -11},
	{ -3, -11}, { -2, -11}, { -1, -11}, {  0, -11},
	{  1, -11}, {  2, -11}, {  3, -11}, {  4, -11},
	{  5, -10}, {  6, -10}, {  6,  -9}, {  7,  -9},
	{  8,  -8}, {  8,  -7}, {  9,  -7}, {  9,  -6},
	{ 10,  -5}, { 10,  -4}, { 10,  -3}, { 10,  -2},
	{ 10,  -1}, { 12,   0}, { 11,   1}, { 11,   2},
	{ 11,   3}, { 11,   4}, { 10,   5}, { 10,   6},
	{  9,   7}, {  8,   8}, {  7,   9}, {  6,  10},
	{  5,  10}, {  4,  11}, {  3,  11}, {  2,  11},
	{  1,  11}, {  0,  11}, { -1,  11}, { -2,  11},
	{ -3,  11}, { -4,  11}, { -5,  11}, { -6,  10},
	{ -7,  10}, { -8,   9}, { -9,   8}, {-10,   7},
	{-11,   6}, {-11,   5}, {-12,   4}, {-12,   3},
	{-12,   2}, {-12,   1}, {-12,   0}, {-12,  -1},
	{-12,  -2}, {-12,  -3}, {-12,  -4}, {-12,  -5},
	{-11,  -6}, {-11,  -7}, {-10,  -8}, { -9,  -9},
	{ -8, -10}, { -7, -11}, { -6, -11}, { -5, -12},
	{ -4, -12}, { -3, -12}, { -2, -12}, { -1, -12},
	{  0, -12}, {  1, -12}, {  2, -12}, {  3, -12},
	{  4, -12}, {  5, -11}, {  6, -11}, {  7, -10},
	{  8,  -9}, {  9,  -8}, { 10,  -7}, { 10,  -6},
	{ 11,  -5}, { 11,  -4}, { 11,  -3}, { 11,  -2},
	{ 11,  -1}, { 13,   0}, { 12,   1}, { 12,   2},
	{ 12,   3}, { 12,   4}, { 11,   5}, { 11,   6},
	{ 10,   7}, { 10,   8}, {  9,   8}, {  9,   9},
	{  8,   9}, {  8,  10}, {  7,  10}, {  6,  11},
	{  5,  11}, {  4,  12}, {  3,  12}, {  2,  12},
	{  1,  12}, {  0,  12}, { -1,  12}, { -2,  12},
	{ -3,  12}, { -4,  12}, { -5,  12}, { -6,  11},
	{ -7,  11}, { -8,  10}, { -9,  10}, { -9,   9},
	{-10,   9}, {-10,   8}, {-11,   8}, {-11,   7},
	{-12,   6}, {-12,   5}, {-13,   4}, {-13,   3},
	{-13,   2}, {-13,   1}, {-13,   0}, {-13,  -1},
	{-13,  -2}, {-13,  -3}, {-13,  -4}, {-13,  -5},
	{-12,  -6}, {-12,  -7}, {-11,  -8}, {-11,  -9},
	{-10,  -9}, {-10, -10}, { -9, -10}, { -9, -11},
	{ -8, -11}, { -7, -12}, { -6, -12}, { -5, -13},
	{ -4, -13}, { -3, -13}, { -2, -13}, { -1, -13},
	{  0, -13}, {  1, -13}, {  2, -13}, {  3, -13},
	{  4, -13}, {  5, -12}, {  6, -12}, {  7, -11},
	{  8, -11}, {  8, -10}, {  9, -10}, {  9,  -9},
	{ 10,  -9}, { 10,  -8}, { 11,  -7}, { 11,  -6},
	{ 12,  -5}, { 12,  -4}, { 12,  -3}, { 12,  -2},
	{ 12,  -1}, { 14,   0}, { 13,   1}, { 13,   2},
	{ 13,   3}, { 13,   4}, { 13,   5}, { 12,   5},
	{ 12,   6}, { 12,   7}, { 11,   7}, { 11,   8},
	{ 10,   9}, {  9,  10}, {  8,  11}, {  7,  11},
	{  7,  12}, {  6,  12}, {  5,  12}, {  5,  13},
	{  4,  13}, {  3,  13}, {  2,  13}, {  1,  13},
	{  0,  13}, { -1,  13}, { -2,  13}, { -3,  13},
	{ -4,  13}, { -5,  13}, { -6,  13}, { -6,  12},
	{ -7,  12}, { -8,  12}, { -8,  11}, { -9,  11},
	{-10,  10}, {-11,   9}, {-12,   8}, {-12,   7},
	{-13,   7}, {-13,   6}, {-13,   5}, {-14,   5},
	{-14,   4}, {-14,   3}, {-14,   2}, {-14,   1},
	{-14,   0}, {-14,  -1}, {-14,  -2}, {-14,  -3},
	{-14,  -4}, {-14,  -5}, {-14,  -6}, {-13,  -6},
	{-13,  -7}, {-13,  -8}, {-12,  -8}, {-12,  -9},
	{-11, -10}, {-10, -11}, { -9, -12}, { -8, -12},
	{ -8, -13}, { -7, -13}, { -6, -13}, { -6, -14},
	{ -5, -14}, { -4, -14}, { -3, -14}, { -2, -14},
	{ -1, -14}, {  0, -14}, {  1, -14}, {  2, -14},
	{  3, -14}, {  4, -14}, {  5, -14}, {  5, -13},
	{  6, -13}, {  7, -13}, {  7, -12}, {  8, -12},
	{  9, -11}, { 10, -10}, { 11,  -9}, { 11,  -8},
	{ 12,  -8}, { 12,  -7}, { 12,  -6}, { 13,  -6},
	{ 13,  -5}, { 13,  -4}, { 13,  -3}, { 13,  -2},
	{ 13,  -1}, { 15,   0}, { 14,   1}, { 14,   2},
	{ 14,   3}, { 14,   4}, { 14,   5}, { 13,   6},
	{ 13,   7}, { 12,   8}, { 11,   9}, { 11,  10},
	{ 10,  10}, { 10,  11}, {  9,  11}, {  8,  12},
	{  7,  13}, {  6,  13}, {  5,  14}, {  4,  14},
	{  3,  14}, {  2,  14}, {  1,  14}, {  0,  14},
	{ -1,  14}, { -2,  14}, { -3,  14}, { -4,  14},
	{ -5,  14}, { -6,  14}, { -7,  13}, { -8,  13},
	{ -9,  12}, {-10,  11}, {-11,  11}, {-11,  10},
	{-12,  10}, {-12,   9}, {-13,   8}, {-14,   7},
	{-14,   6}, {-15,   5}, {-15,   4}, {-15,   3},
	{-15,   2}, {-15,   1}, {-15,   0}, {-15,  -1},
	{-15,  -2}, {-15,  -3}, {-15,  -4}, {-15,  -5},
	{-15,  -6}, {-14,  -7}, {-14,  -8}, {-13,  -9},
	{-12, -10}, {-12, -11}, {-11, -11}, {-11, -12},
	{-10, -12}, { -9, -13}, { -8, -14}, { -7, -14},
	{ -6, -15}, { -5, -15}, { -4, -15}, { -3, -15},
	{ -2, -15}, { -1, -15}, {  0, -15}, {  1, -15},
	{  2, -15}, {  3, -15}, {  4, -15}, {  5, -15},
	{  6, -14}, {  7, -14}, {  8, -13}, {  9, -12},
	{ 10, -12}, { 10, -11}, { 11, -11}, { 11, -10},
	{ 12,  -9}, { 13,  -8}, { 13,  -7}, { 14,  -6},
	{ 14,  -5}, { 14,  -4}, { 14,  -3}, { 14,  -2},
	{ 14,  -1}
};

std::shared_ptr<Chunk> findChunk(const std::vector<std::shared_ptr<Chunk>>& vecOfElements, const glm::vec2& element)
{
	int result;
	for (std::shared_ptr<Chunk> ch : vecOfElements) {
		if (ch->getCoords() == element) {
			return ch;
		}
	}
	return nullptr;
}

void resizeWindow(GLFWwindow* window, int fbw, int fbh)
{
	glViewport(0, 0, fbw, fbh);
}

void Game::initWindow(const std::string& title, int width, const int height, int GLmayor, int GLminor, bool resizable)
{
	if (!glfwInit())
	{
		std::cout << "Error loading glfw" << std::endl;
	}
	
	//CREATE WINDOW
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLmayor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLminor);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_SAMPLES, 16);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (window == nullptr)
	{
		std::cout << "Error creating window" << std::endl;
	}

	glfwGetFramebufferSize(window, &fbwidth, &fbheight);
	glfwSetFramebufferSizeCallback(window, resizeWindow);
	//glViewport(0, 0, frameBufferW, frameBufferH);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error loading glew" << std::endl;
	}
}

void Game::initMatrices()
{
	worldUp = glm::vec3 (0.0f, 1.0f, 0.0f);
	camFront = glm::vec3 (0.0f, 0.0f, -1.0f);
	cameraPos = glm::vec3(0.0f, 10.0f, 1.0f);
	viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(cameraPos, cameraPos + camFront, worldUp);

	glm::mat4 ProjectionMatrix(1.0f);
	projMatrix = glm::perspective(glm::radians(fov), static_cast<float>(fbwidth) / fbheight, nearPlane, farPlane);
}

void Game::configureOpenGL()
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initShaders(int majVer, int minVer)
{
	shaders.push_back(std::shared_ptr<Shader>(new Shader(majVer, minVer, "VertexShader.glsl", "FragmentShader.glsl")));
}

void Game::initTextures()
{
	materials.push_back(std::shared_ptr<Material>(new Material(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f), 0, 1)));
	std::ifstream file("blockData.dat");
	if (file) {
		std::string ch;
		uint8_t texCount = 0;
		std::vector<std::string> clrStrs;
		while (std::getline(file, ch)) {
			size_t pos = 0;
			std::string token;
			std::vector<uint8_t> elems;
			pos = ch.find(":");
			int id = std::stoi(ch.substr(0, pos));
			ch.erase(0, pos + 1);
			while ((pos = ch.find(":")) != std::string::npos) {
				std::string path = "assets/block/" + ch.substr(0, pos);
				std::cout << (int)id << " " << (unsigned int)texCount << " " << path << std::endl;
				ch.erase(0, pos + 1);
				if (id != -1) {
					textures.emplace(texCount, std::shared_ptr<Texture>(new Texture(path, GL_TEXTURE_2D)));
					if (std::find(clrStrs.begin(), clrStrs.end(), path) != clrStrs.end()) {
						clrList.push_back(texCount);
						std::cout << "pushing " << path << " | " << (unsigned int)texCount << std::endl;
					}
				}
				else {
					clrStrs.push_back(path);
				}
				if (id != -1) {
					elems.push_back(texCount);
					texCount++;
				}
			}
			if (id != -1) {
				std::vector<uint8_t> blockTexs;
				for (int i = 0; i < 3; i++) {
					if (!elems.empty()) {
						blockTexs.push_back(elems.back());
						elems.pop_back();
					}
					else {
						blockTexs.push_back(blockTexs.back());
					}
				}
				blockData[id] = blockTexs;
			}
		}
	}
	//textures.push_back(new Texture("assets/block/cobblestone.png", GL_TEXTURE_2D));
}

void Game::initPointLights()
{
	this->pointLights.push_back(std::shared_ptr<PointLight>(new PointLight(glm::vec3(0.0f))));
}

void Game::initLights()
{
	//initPointLights();
}

void Game::initUniforms()
{
	for (auto& p : pointLights)
	{
		p->sendToShader(*shaders[0]);
	}

	shaders[0]->setVec3f("cameraPos", cameraPos);
	shaders[0]->setMat4fv("ProjectionMatrix", projMatrix);
}

void Game::updateUniforms()
{
	glfwGetFramebufferSize(window, &fbwidth, &fbheight);

	viewMatrix = cam.getVM();
	shaders[0]->setMat4fv("ViewMatrix", viewMatrix);
	shaders[0]->setVec3f("cameraPos", cam.getPos());
	for (auto& p : pointLights)
	{
		p->sendToShader(*shaders[0]);
	}

	projMatrix = glm::mat4(1.0f);
	projMatrix = glm::perspective(glm::radians(fov), static_cast<float>(fbwidth) / fbheight, nearPlane, farPlane);
	shaders[0]->setMat4fv("ProjectionMatrix", projMatrix);
}

void Game::addChunk()
{
	if (updateMtx.try_lock()) {
		while (!finishedChunks.empty()) {
			mtx.lock();
			std::shared_ptr<Chunk> chunk = finishedChunks.back();
			finishedChunks.pop_back();
			notLoadedChunks.pop_back();
			mtx.unlock();
			chunks.push_back(chunk);
			glm::vec2 coor = chunk->getCoords();
			loadedChunks.push_back({ coor.x, coor.y });
			models.emplace(coor, std::vector<std::shared_ptr<Model>>());
			for (auto& mod : chunk->createModel(materials[0], textures))
				models.at(coor).push_back(mod.second);
		}
		while (!chunksToUpdate.empty()) {
			reloadMtx.lock();
			Chunk* chunk = chunksToUpdate.back();
			chunksToUpdate.pop_back();
			chunk->updateModel();
			reloadMtx.unlock();
		}
		updateMtx.unlock();
	}
}

void Game::loadChunks()
{
	while (!getWinShouldClose()) {
		glm::vec2 posCamera(floor(cam.getPos().x / CHUNK_SIZE), floor(cam.getPos().z / CHUNK_SIZE));
		if (!lastChunk or *lastChunk != posCamera) {
			glm::vec2 coor;
			bool finished = false;
			bool found = false;
			for (int i = 0; i < DISTANCE_SIZE; i++) {
				coor = DISTANCE_SELECTION[i] + posCamera;
				mtx.lock();
				if (std::find(loadedChunks.begin(), loadedChunks.end(), coor) == loadedChunks.end() and
					std::find(notLoadedChunks.begin(), notLoadedChunks.end(), coor) == notLoadedChunks.end()) {
					found = true;
					mtx.unlock();
					break;
				}
				mtx.unlock();
			}
			if (found) {
				std::shared_ptr<Chunk> chunk = std::shared_ptr<Chunk>(new Chunk(coor.x, coor.y, 100, CHUNK_SIZE, &blockData, &clrList, &chunks, &finishedChunks, shaders[0]));
				mtx.lock();
				finishedChunks.push_back(chunk);
				notLoadedChunks.push_back(coor);
				mtx.unlock();
				Chunk** toLoad = chunk->reloadNeighbors();
				reloadMtx.lock();
				for (int i = 0; i < 4; i++) {
					if (toLoad[i]) {
						chunksToUpdate.push_back(toLoad[i]);
					}
				}
				reloadMtx.unlock();
			}
			else {
				lastChunk = std::shared_ptr<glm::vec2>(new glm::vec2(posCamera));
			}
			mtx.lock();
			for (int i = 0; i < chunks.size(); i++) {
				if (i >= chunks.size()) {
					break;
				}
				coor = chunks.at(i)->getCoords();
				if (calculateDistance(coor) > MAX_DISTANCE) {
					renderMtx.lock();
					models.erase(coor);
					renderMtx.unlock();
					loadedChunks.erase(loadedChunks.begin() + i);
					chunks.erase(chunks.begin() + i);
				}
			}
			mtx.unlock();
		}
		Sleep(1);
	}
}

float Game::calculateDistance(glm::vec2 coor)
{
	glm::vec2 posCamera(floor(cam.getPos().x / CHUNK_SIZE), floor(cam.getPos().z / CHUNK_SIZE));
	return glm::distance(glm::vec2(posCamera.x, posCamera.y), coor);
}

Game::Game(const std::string& title, int width, const int height, int GLmayor, int GLminor, bool resizable, float fov, float nearPlane, float farPlane)
	: fbheight(0), fbwidth(0), fov(fov), nearPlane(nearPlane), farPlane(farPlane), cam(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
{
	srand(0);
	dt = 0.0f;
	curTime = 0.0f;
	lastTime = 0.0f;
	nbFrames = 0;
	lastTimeFPS = static_cast<float>(glfwGetTime());

	lastMouseX = 0.0f;
	lastMouseY = 0.0f;
	mouseX = 0.0f;
	mouseY = 0.0f;
	mouseOffsetX = 0.0f;
	mouseOffsetY = 0.0f;
	firstMouse = true;

	initWindow(title, width, height, GLmayor, GLminor, resizable);
	configureOpenGL();
	initMatrices();
	initShaders(GLmayor, GLminor);
	initTextures();
	initLights();
	initUniforms();

	chunkLoader = std::thread(&Game::loadChunks, this);
}

Game::~Game()
{
	chunkLoader.join();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Game::update()
{
	addChunk();
	bool fl = false;
	glm::vec2 pos = glm::vec2(floor(cam.getPos().x / CHUNK_SIZE), floor(cam.getPos().z / CHUNK_SIZE));
	glm::vec3 absPos = glm::vec3(cam.getPos());
	std::shared_ptr<Chunk> ch = findChunk(chunks, pos);
	if (ch) {
		fl = ch->isNotAir(absPos);
	}
	cam.updateGravity(dt, fl);
	updateDt();
	updateInput();
}

void Game::render()
{
	glClearColor(0.3f, 0.7f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//rotation.y += 0.5f;

	updateUniforms();

	renderMtx.lock();
	for (auto& i : models)
	{
		for (auto& j : i.second) {
			j->render(shaders[0]);
		}
	}
	renderMtx.unlock();

	glfwSwapBuffers(window);
	glFlush();
}

bool Game::getWinShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Game::closeWindow()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Game::updateKeyboardInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		closeWindow();
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cam.move(dt, FORW);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cam.move(dt, BACKW);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cam.move(dt, LEFT);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cam.move(dt, RIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		cam.move(dt, UP);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		cam.move(dt, DOWN);
	}
}

void Game::updateMouseInput()
{
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (firstMouse)
	{
		lastMouseX = mouseX;
		lastMouseY = mouseY;
		firstMouse = false;
	}

	mouseOffsetX = mouseX - lastMouseX;
	mouseOffsetY = mouseY - lastMouseY;
	lastMouseX = mouseX;
	lastMouseY = mouseY;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
	{
		//pointLights[0]->setPos(cam.getPos());
	}
}

void Game::updateInput()
{
	glfwPollEvents();
	updateKeyboardInput();
	updateMouseInput();
	cam.updateInput(dt, -1, mouseOffsetX, mouseOffsetY);
}

void Game::updateDt()
{
	curTime = static_cast<float>(glfwGetTime());
	dt = curTime - lastTime;
	nbFrames++;
	if (curTime - lastTimeFPS >= 1.0) {
		std::cout << 1000.0 / (double)nbFrames << " | " << nbFrames << " | " << chunks.size() << " | " << "(" << cam.getPos().x << ", " << cam.getPos().z << ", " << cam.getPos().y << ")\n";
		nbFrames = 0;
		lastTimeFPS += 1.0;
	}
	lastTime = curTime;
}
