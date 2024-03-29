#include "savefile.hpp"
#include <Windows.h>
#include "../globals/globals.hpp"
#include <fstream>
#include <iostream>

void savefile_t::create_config() {

	//json default_config;

	this->config["currency"] = {
		{ "name", "Bits" },
		{ "amount", 0.f }
	};

	this->config["upgrades"] = {
	{
		{ "name", "New CPU" },
		{ "description", "Increases manual programming speed."},
		{ "internal_name", "newcpu" },
		{ "level", 0 },
		{ "increment", 0.05f },
		{ "base_price", 20.f },
		{ "price_modifier", 1.0f }
	},
	{
		{ "name", "Increased Code Quality" },
		{ "description", "Gain more bits per submission."},
		{ "internal_name", "bettercode" },
		{ "level", 0 },
		{ "increment", 0.5f },
		{ "base_price", 25.f },
		{ "price_modifier", 3.0f }
	},
	{
		{ "name", "Monitor Resolution" },
		{ "description", "Increases manual reviewing speed."},
		{ "internal_name", "highres" },
		{ "level", 0 },
		{ "increment", 0.05f },
		{ "base_price", 25.f },
		{ "price_modifier", 1.5f }
	},
	{
		{ "name", "Daylight Savings Time" },
		{ "description", "Time advances more quickly."},
		{ "internal_name", "dst" },
		{ "level", 0 },
		{ "increment", 0.0f },
		{ "base_price", 100.f },
		{ "price_modifier", 3.3f }
	},
	{
		{ "name", "ChatGPT" },
		{ "description", "Gain bits while completely idle."},
		{ "internal_name", "idlebits" },
		{ "level", 0 },
		{ "increment", 0.5f },
		{ "base_price", 10.0f },
		{ "price_modifier", 1.0f }
	}
	};

	this->config["games"] = {
	{
		{ "name", "Programming" },
		{ "description", "Write a simple program."},
		{ "internal_name", "programming" },
		{ "reward", 10.f },
		{ "duration", 5.f }
	},
	{
		{ "name", "Code Reviews" },
		{ "description", "Check out other people's code."},
		{ "internal_name", "codereviews" },
		{ "reward", 10.f },
		{ "duration", 5.f }
	}
	};

	this->config["last_save_time"] = std::time(nullptr);
	this->config["save_creation_time"] = std::time(nullptr);

}


json savefile_t::read_config(std::string filename) {

	filename = filename.length() != 0 ? filename : gvars.config.file_path;

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

void savefile_t::write_config(json config, std::string filename) {

	config = config != nullptr ? config : this->config;
	filename = filename.length() != 0 ? filename : gvars.config.file_path;

	try {
		config["save_creation_time"].get<int>();
	}
	catch (std::exception e) {
		config["save_creation_time"] = 0;
		std::cout << "save_creation_time is null. Setting to 0. Re-read the savefile to update.";
	}

	std::ofstream file(filename);
	if (file.is_open()) {

		file << std::setw(4) << config;
		file.close();
		std::cout << "Game saved successfully." << std::endl;

	}
	else {
		std::cerr << "Unable to save the game." << std::endl;
	}

}

savefile_t savefile;