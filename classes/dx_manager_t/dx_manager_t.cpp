#include "dx_manager_t.hpp"

#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_dx9.h"
#include "../../lib/imgui/imgui_impl_win32.h"
#include "../../lib/imgui/imgui_stdlib.h"

dx_manager_t* dx_manager;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wide_parameter, LPARAM long_parameter);

long __stdcall Window_process(HWND window, UINT message, WPARAM wide_parameter, LPARAM long_parameter) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, wide_parameter, long_parameter))
		return true;

	switch (message) {
	case WM_SIZE: {
		if (*dx_manager->get_device() && wide_parameter != SIZE_MINIMIZED) {
			dx_manager->get_parameters()->BackBufferWidth = LOWORD(long_parameter);
			dx_manager->get_parameters()->BackBufferHeight = HIWORD(long_parameter);
			dx_manager->reset_device();
		}
	}return 0;

	case WM_SYSCOMMAND: {
		if ((wide_parameter & 0xFFF0) == SC_KEYMENU)
			return 0;
	}break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}return 0;

	case WM_LBUTTONDOWN: {
		*dx_manager->get_position() = MAKEPOINTS(long_parameter);
	}return 0;

	case WM_MOUSEMOVE: {
		if (wide_parameter == MK_LBUTTON) {
			const auto points = MAKEPOINTS(long_parameter);
			auto rect = ::RECT{ };

			GetWindowRect(window, &rect);

			rect.left += points.x - dx_manager->get_position()->x;
			rect.top += points.y - dx_manager->get_position()->y;

			auto maxy = 0;

			if (dx_manager->get_position()->x > -0 && dx_manager->get_position()->x <= 0 && dx_manager->get_position()->y >= 0 && dx_manager->get_position()->y <= maxy) {
				SetWindowPos(window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
			}
		}
	}
	}

	return DefWindowProcA(window, message, wide_parameter, long_parameter);
}

void dx_manager_t::create_window() {

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_CLASSDC;
	window_class.lpfnWndProc = Window_process;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = GetModuleHandleA(0);
	window_class.hIcon = 0;
	window_class.hCursor = 0;
	window_class.hbrBackground = 0;
	window_class.lpszMenuName = 0;
	window_class.lpszClassName = "";
	window_class.hIconSm = 0;

	RegisterClassEx(&window_class);

	window = CreateWindowEx(0, "", "", WS_TILED | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), 0, 0, window_class.hInstance, 0);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);

}

void dx_manager_t::destroy_window() {
	DestroyWindow(window);
	UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}

HWND dx_manager_t::get_window() {
	return window;
}

void dx_manager_t::create_device() {

	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return;

	ZeroMemory(&present_parameters, sizeof(present_parameters));

	present_parameters.Windowed = TRUE;
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	present_parameters.EnableAutoDepthStencil = TRUE;
	present_parameters.AutoDepthStencilFormat = D3DFMT_D16;
	present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_parameters, &device);

}

void dx_manager_t::reset_device() {

	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&present_parameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();

}

void dx_manager_t::destroy_device() {
	if (device) {
		device->Release();
		device = nullptr;
	}

	if (d3d) {
		d3d->Release();
		d3d = nullptr;
	}
}

LPDIRECT3DDEVICE9* dx_manager_t::get_device() {
	return &device;
}

POINTS* dx_manager_t::get_position() {
	return &position;
}

D3DPRESENT_PARAMETERS* dx_manager_t::get_parameters() {
	return &present_parameters;
}
