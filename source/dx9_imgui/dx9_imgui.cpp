#include "dx9_imgui.hpp"
#include <sstream>
#include <string>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_stdlib.h"

#include "../globals/globals.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wide_parameter, LPARAM long_parameter);

long __stdcall Window_process(HWND window, UINT message, WPARAM wide_parameter, LPARAM long_parameter) {
	if (ImGui_ImplWin32_WndProcHandler(window, message, wide_parameter, long_parameter))
		return true;

	switch (message) {
	case WM_SIZE: {
		if (dx9::device && wide_parameter != SIZE_MINIMIZED) {
			dx9::present_parameters.BackBufferWidth = LOWORD(long_parameter);
			dx9::present_parameters.BackBufferHeight = HIWORD(long_parameter);
			dx9::reset_device();
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
		dx9::position = MAKEPOINTS(long_parameter);
	}return 0;

	case WM_MOUSEMOVE: {
		if (wide_parameter == MK_LBUTTON) {
			const auto points = MAKEPOINTS(long_parameter);
			auto rect = ::RECT{ };

			GetWindowRect(dx9::window, &rect);

			rect.left += points.x - dx9::position.x;
			rect.top += points.y - dx9::position.y;

			auto maxy = 30;

			if (dx9::position.x > -0 && dx9::position.x <= gvars.window.width && dx9::position.y >= 0 && dx9::position.y <= maxy) {
				SetWindowPos(dx9::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
			}
		}
	}
	}

	return DefWindowProcA(window, message, wide_parameter, long_parameter);
}

void dx9::create_window(const char* window_name, const char* class_name) noexcept {

	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_DROPSHADOW;
	window_class.lpfnWndProc = Window_process;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = GetModuleHandleA(0);
	window_class.hIcon = 0;
	window_class.hCursor = 0;
	window_class.hbrBackground = 0;
	window_class.lpszMenuName = 0;
	window_class.lpszClassName = class_name;
	window_class.hIconSm = 0;

	RegisterClassEx(&window_class);

	window = CreateWindowEx(0, class_name, window_name, WS_POPUP, 0, 0, gvars.window.width, gvars.window.height, 0, 0, window_class.hInstance, 0);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void dx9::destroy_window() noexcept {
	DestroyWindow(window);
	UnregisterClass(window_class.lpszClassName, window_class.hInstance);
}

bool dx9::create_device() noexcept {
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

	ZeroMemory(&present_parameters, sizeof(present_parameters));

	present_parameters.Windowed = TRUE;
	present_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	present_parameters.BackBufferFormat = D3DFMT_UNKNOWN;
	present_parameters.EnableAutoDepthStencil = TRUE;
	present_parameters.AutoDepthStencilFormat = D3DFMT_D16;
	present_parameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &present_parameters, &device) < 0)
		return false;

	return true;
}

void dx9::reset_device() noexcept {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&present_parameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void dx9::destroy_device() noexcept {
	if (device) {
		device->Release();
		device = nullptr;
	}

	if (d3d) {
		d3d->Release();
		d3d = nullptr;
	}
}

void gui::create_imgui() noexcept {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui_ImplWin32_Init(dx9::window);
	ImGui_ImplDX9_Init(dx9::device);

	ImFontConfig font_config;
	font_config.FontDataOwnedByAtlas = false;
}

void gui::destroy_imgui() noexcept {
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::begin_render() noexcept {
	MSG message;

	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::end_render() noexcept {
	ImGui::EndFrame();

	dx9::device->SetRenderState(D3DRS_ZENABLE, FALSE);
	dx9::device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	dx9::device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	dx9::device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (dx9::device->BeginScene() >= 0) {
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		dx9::device->EndScene();
	}

	const auto result = dx9::device->Present(0, 0, 0, 0);

	if (result == D3DERR_DEVICELOST && dx9::device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		dx9::reset_device();
}