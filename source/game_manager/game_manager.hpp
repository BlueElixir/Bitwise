#pragma once

#include <vector>
#include <string>
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

class game_manager_t {

private: std::string config_file_path{ "C:\\Games\\Bitwise\\savefile.json" };
private: std::string config_folder_path{ "C:\\Games\\Bitwise" };
private: json config;

public: void init();
public: void run_loop();
public: void end();

public: void create_config();
public: json read_config(const std::string& filename);
public: void write_config(const json& config, const std::string& filename);

};

extern game_manager_t game_manager;