#include "game_manager.hpp"

#include "../dx9_imgui/dx9_imgui.hpp"
#include "../globals/globals.hpp"
#include "../user_interface/user_interface.hpp"

void game_manager_t::init() {

	dx9::create_window("Bitwise - Idle Game", "bitwise_class");
	dx9::create_device();
	gui::create_imgui();

}

void game_manager_t::run_loop() {

	while (!gvars.states.should_exit && IsWindow(dx9::window)) {

		gui::begin_render();
		user_interface.do_draw();
		gui::end_render();

	}

}

void game_manager_t::end() {

	gui::destroy_imgui();
	dx9::destroy_device();
	dx9::destroy_window();

}

game_manager_t game_manager;