#include <iostream>
#include <Windows.h>
#include "dx9_imgui/dx9_imgui.hpp"
#include "globals/globals.hpp"
#include "user_interface/user_interface.hpp"

#define DEBUG
#ifdef DEBUG
int main() {
#else
int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
#endif

	dx9::create_window("window", "window class");
	dx9::create_device();
	gui::create_imgui();

	while (!gvars.states.should_exit) {

		gui::begin_render();
		user_interface.do_draw();
		gui::end_render();

	}

	gui::destroy_imgui();
	dx9::destroy_device();
	dx9::destroy_window();

	return EXIT_SUCCESS;
}