#include "game_manager.hpp"
#include <fstream>
#include <Windows.h>
#include <Lmcons.h>
#include <string>
#include <sstream>
#include <time.h>
#include <iostream>

#include "../dx9_imgui/dx9_imgui.hpp"
#include "../globals/globals.hpp"
#include "../user_interface/user_interface.hpp"
#include "../dx9_imgui/imgui/imgui_impl_dx9.h"
#include "../dx9_imgui/imgui/imgui_impl_win32.h"

void game_manager_t::init() {

	dx9::create_window("Bitwise - Idle Game", "bitwise_class");
	dx9::create_device();
	gui::create_imgui();

	this->create_config();

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

}

void game_manager_t::create_config() {

	json default_config;
	default_config["currency"] = {
		{ "name", "Bits" },
		{ "amount", 100.f }
	};

	default_config["upgrades"] = {
	{
		{ "name", "New CPU" },
		{ "description", "Increases manual programming speed."},
		{ "internal_name", "" },
		{ "level", 0.f },
		{ "increment", 0.2f },
		{ "base_price", 10.f },
		{ "price_modifier", 1.618f }
	},
	{
		{ "name", "Better IDE Font" },
		{ "description", "Increases Code Quality"},
		{ "internal_name", "" },
		{ "level", 0.f },
		{ "increment", 0.1f },
		{ "base_price", 2000.f },
		{ "price_modifier", 2.415f }
	},
	{
		{ "name", "Monitor Resolution" },
		{ "description", "Increases manual reviewing speed."},
		{ "internal_name", "" },
		{ "level", 0.f },
		{ "increment", 0.2f },
		{ "base_price", 20.f },
		{ "price_modifier", 1.818f }
	},
	{
		{ "name", "Daylight Savings Time" },
		{ "description", "Time advances more quickly."},
		{ "internal_name", "" },
		{ "level", 0.f },
		{ "increment", 0.2f },
		{ "base_price", 400.f },
		{ "price_modifier", 3.14f }
	}
	};

	default_config["games_completed"] = {
		{ "coding", 0 },
		{ "reviewing", 0 }
	};

	default_config["last_save_time"] = std::time(nullptr);


	CreateDirectory(gvars.config.folder_path.c_str(), nullptr);

	this->config = this->read_config(gvars.config.file_path);

	if (this->config == nullptr)
		this->write_config(default_config, gvars.config.file_path);

}

json game_manager_t::read_config(const std::string& filename) {

	std::ifstream file(filename);
	if (file.is_open()) {
		json config;
		file >> config;
		file.close();
		return config;
	}
	else {
		return nullptr;
	}

}

void game_manager_t::write_config(const json& config, const std::string& filename) {

	std::ofstream file(filename);
	if (file.is_open()) {

		file << std::setw(4) << config;
		file.close();
		std::cout << "Configuration saved successfully." << std::endl;

	}
	else {
		std::cerr << "Unable to save configuration." << std::endl;
	}

}

game_manager_t game_manager;