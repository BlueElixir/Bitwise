#pragma once

#include <vector>
#include <string>

class game_manager_t {

public: void init();
public: void run_loop();
public: void end();

public: void read_config_file(std::vector<std::vector<std::string>>& config, std::string file_name);
public: void write_config_file(std::vector<std::vector<std::string>>& config, std::string file_name);

private: std::string config_file_path{ "" };


};

extern game_manager_t game_manager;