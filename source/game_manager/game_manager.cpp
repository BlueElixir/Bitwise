#include "game_manager.hpp"

#include "../dx9_imgui/dx9_imgui.hpp"
#include "../globals/globals.hpp"
#include "../user_interface/user_interface.hpp"
#include "../savefile/savefile.hpp"

void game_manager_t::init() {

	CreateDirectory(gvars.config.folder_path.c_str(), nullptr);

	if (std::filesystem::exists(gvars.config.file_path)) {
		if (savefile.read_config() == nullptr) {
			DeleteFile(gvars.config.file_path.c_str());
		}
		else {
			savefile.config = savefile.read_config();
		}
	}

	dx9::create_window("Bitwise - Idle Game", "bitwise_class");
	dx9::create_device();
	gui::create_imgui();

}

void game_manager_t::run_loop() {

	ImGuiIO& io = ImGui::GetIO();

	while (!gvars.states.should_exit && IsWindow(dx9::window)) {

		/*if (gvars.ui.resolution_changed) {

			dx9::destroy_window();
			gui::destroy_imgui();

			dx9::create_window("Bitwise - Idle Game", "bitwise_class");
			gui::create_imgui();
			

			gvars.ui.resolution_changed = false;
		}*/

		gui::begin_render();
		user_interface.do_draw();
		gui::end_render();

	}

}

void game_manager_t::end() {

	gui::destroy_imgui();
	dx9::destroy_device();
	dx9::destroy_window();

	if (user_interface.intro_state == 5) {
		savefile.config["last_save_time"] = std::time(nullptr);;
		savefile.write_config();
	}
		

}

game_manager_t game_manager;