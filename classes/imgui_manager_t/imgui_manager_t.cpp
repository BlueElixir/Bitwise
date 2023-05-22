#include "imgui_manager_t.hpp"
#include "../dx_manager_t/dx_manager_t.hpp"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_impl_dx9.h"
#include "../../lib/imgui/imgui_impl_win32.h"

#include "../dx_manager_t/dx_manager_t.hpp"

void imgui_manager_t::create() {
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui_ImplWin32_Init(dx_manager->get_window());
	ImGui_ImplDX9_Init(*dx_manager->get_device());

	ImFontConfig font_config;
	font_config.FontDataOwnedByAtlas = false;

}

imgui_manager_t* imgui_manager;