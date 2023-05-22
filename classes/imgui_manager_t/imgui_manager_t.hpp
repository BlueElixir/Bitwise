#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#include <d3d9.h>
#include <d3dx9.h>

class imgui_manager_t {

	void create(); // create imgui
	void destroy(); // destroy imgui

public:

	void initialise();
	void render(); // perform render

};

extern imgui_manager_t* imgui_manager;