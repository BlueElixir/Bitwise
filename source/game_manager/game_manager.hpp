#pragma once

#include <vector>
#include <string>

class game_manager_t {

public: void init();
public: void run_loop();
public: void end();

};

extern game_manager_t game_manager;