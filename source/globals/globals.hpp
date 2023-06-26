#include <string>
#include <vector>
#include "../dx9_imgui/dx9_imgui.hpp"
#include "../dx9_imgui/imgui/imgui.h"

struct globals_t {

	struct window_t {

		int width = 1360;
		int height = 768;

	} window;

	struct states_t {

		bool should_exit = false; // set to true when you want to close program
		float curtime = 0.f; // stores time since game start
		std::string game_version = "Version: 0.1b";
		std::string developer_str = "Made by BlueElixir";

	} states;

	struct user_interface_t {

		bool resolution_changed = false;

	} ui;

	struct textures_t {

		enum images {

			Image_springtime = 0,
			Image_bitwise = 1,
		};

		std::vector<LPDIRECT3DTEXTURE9> images = { nullptr, nullptr };
		// max font size: 50, array size 51
		std::vector<ImFont*> fonts;

	} textures;

	struct config_t {

		std::string file_path{ "C:\\Games\\Bitwise\\savefile.json" };
		std::string folder_path{ "C:\\Games\\Bitwise" };

	} config;

};

extern globals_t gvars;