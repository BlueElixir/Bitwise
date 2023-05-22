#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#include <d3d9.h>
#include <d3dx9.h>

class dx_manager_t {

	HWND window = nullptr;
	WNDCLASSEXA window_class = { };
	POINTS position = { };
	PDIRECT3D9 d3d = nullptr;
	LPDIRECT3DDEVICE9 device = nullptr;
	D3DPRESENT_PARAMETERS present_parameters = { };

public:

	void create_window();
	void destroy_window();
	HWND get_window();

	void create_device();
	void reset_device();
	void destroy_device();
	LPDIRECT3DDEVICE9* get_device();

	POINTS* get_position();

	D3DPRESENT_PARAMETERS* get_parameters();

};

extern dx_manager_t* dx_manager;