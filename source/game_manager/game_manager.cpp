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

void game_manager_t::init() {

	dx9::create_window("Bitwise - Idle Game", "bitwise_class");
	dx9::create_device();
	gui::create_imgui();

	this->create_config();

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

void game_manager_t::create_config() {

	this->config["player"]["name"] = "bob knob";
	config["player"]["currency"] = {
		{ "name", "Bit" },
		{ "amount", 100 }
	};

	this->config["upgrades"] = {
		{
			{ "name", "Efficient Worker" },
			{ "internal_name", "speed" },
			{ "level", 2 }
		},
		{
			{ "name", "Powerful Strikes" },
			{ "internal_name", "damage" },
			{ "level", 1 }
		}
	};

	this->config["games_completed"] = {
		{ "coding", 3 },
		{ "reviewing", 5 }
	};

	this->config["last_save_time"] = std::time(nullptr);


	CreateDirectory(config_folder_path.c_str(), nullptr);

	if (GetFileAttributes(config_file_path.c_str()) == INVALID_FILE_ATTRIBUTES) {
		this->write_config(this->config, this->config_file_path);
	}
	else {
		this->config = this->read_config(this->config_file_path);
	}

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
		return json();
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