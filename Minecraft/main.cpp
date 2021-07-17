#define _CRTDBG_MAP_ALLOC
#include "Game.h"

std::mutex mtx, renderMtx, updateMtx, reloadMtx;

int main()
{
	Game game("3D game", WinWidth, WinHeight, 4, 5, true, 90.0f, 0.001f, 1000.0f);

	//MAIN LOOP
	while (!game.getWinShouldClose())
	{
		//CHECK UPDATE INPUT
		game.update();
		game.render();
	}
	return 0;
}