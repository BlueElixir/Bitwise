#include "game_manager.hpp"
#include <fstream>
#include <Windows.h>
#include <Lmcons.h>
#include <string>
#include <sstream>

#include "../dx9_imgui/dx9_imgui.hpp"
#include "../globals/globals.hpp"
#include "../user_interface/user_interface.hpp"

void game_manager_t::init() {

	dx9::create_window("Bitwise - Idle Game", "bitwise_class");
	dx9::create_device();
	gui::create_imgui();

	char username[UNLEN + 1];
	DWORD usernameSize = UNLEN + 1;
	GetUserName(username, &usernameSize);

	// Construct the file path
	std::string config_file_path = "C:\\Users\\";
	config_file_path += username;
	config_file_path += "\\AppData\\Roaming\\Bitwise";

	// Create the Bitwise directory
	CreateDirectory(config_file_path.c_str(), nullptr);

	// Append the file name to the file path
	config_file_path += "\\savefile.json";

	// Check if the file exists
	if (GetFileAttributes(config_file_path.c_str()) == INVALID_FILE_ATTRIBUTES) {
		// The file does not exist, create it
		std::ofstream file(config_file_path);
		if (file.is_open()) {
			file << "";
			file.close();
		}
	}

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

void game_manager_t::read_config_file(std::vector<std::vector<std::string>>& config, std::string file_name) {
	std::ifstream config_file(file_name);

	std::string line;
	size_t i{ 0 };
	while (std::getline(config_file, line)) {

		if (i == 0 && line != "config_version=1") {
			throw std::runtime_error(std::string("invalid configuration. add \"config_version=1\" to the top of the config"));
		}
		else if (i == 0 && line == "config_version=1") {
			++i;
			continue;
		}

		std::stringstream ss(line);
		std::string sub, sub2;
		int j{ 0 };

		while (std::getline(ss, sub, ':') && j < 3) {
			config[j].push_back(sub);
			++j;
		}
		if (ss.peek() == ':') {
			ss.ignore();
		}
		std::getline(ss, sub2);
		//config[notes].push_back(sub + sub2);

	}

	config_file.close();
}

void game_manager_t::write_config_file(std::vector<std::vector<std::string>>& config, std::string file_name) {

	wchar_t username[UNLEN + 1];
	DWORD usernameSize = UNLEN + 1;
	GetUserNameW(username, &usernameSize);

	// Construct the file path
	std::wstring filePath = L"C:\\Users\\";
	filePath += username;
	filePath += L"\\AppData\\Roaming\\Bitwise";

	// Create the Bitwise directory
	CreateDirectoryW(filePath.c_str(), nullptr);

	// Append the file name to the file path
	filePath += L"\\runtime.cfg";

	// Check if the file exists
	if (GetFileAttributesW(filePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
		// The file does not exist, create it
		std::ofstream file(filePath);
		if (file.is_open()) {
			file << "";
			file.close();
		}
	}

}

game_manager_t game_manager;