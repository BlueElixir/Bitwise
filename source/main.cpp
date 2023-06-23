#include <Windows.h>
#include "game_manager/game_manager.hpp"

#define DEBUG
#ifdef DEBUG
int main() {
#else
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
#endif

	game_manager.init();
	game_manager.run_loop();
	game_manager.end();

	return EXIT_SUCCESS;
}