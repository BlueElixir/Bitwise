#include "user_interface.hpp"
#include "../dx9_imgui/dx9_imgui.hpp"
#include "../dx9_imgui/imgui/imgui.h"
#include "../dx9_imgui/imgui/imgui_impl_dx9.h"
#include "../dx9_imgui/imgui/imgui_impl_win32.h"
#include "../dx9_imgui/imgui/imgui_stdlib.h"
#include "../globals/globals.hpp"
#include "../dx9_imgui/imgui/imgui_internal.h"

user_interface_t user_interface;
// perform rendering tasks


void user_interface_t::draw_intro_logo() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

}


struct FadeAnimationState {
    bool is_hovered = false;
    float hover_timer = 0.0f;
    float anim_duration = 0.1f;
};

ImColor get_fade_color(ImVec2 pos1, ImVec2 pos2, ImColor col1, ImColor col2, FadeAnimationState& state) {
    ImGuiIO& io = ImGui::GetIO();

    // Check if the mouse is hovering over the rectangle
    if (ImGui::IsMouseHoveringRect(pos1, pos2)) {
        state.is_hovered = true;
        state.hover_timer += io.DeltaTime;
    }
    else {
        state.is_hovered = false;
        state.hover_timer -= io.DeltaTime;
    }

    // Clamp hover timer between 0 and anim duration
    state.hover_timer = ImClamp(state.hover_timer, 0.0f, state.anim_duration);

    // Calculate the color based on hover timer
    float t = state.hover_timer / state.anim_duration; // Normalize the timer

    // Interpolate between col1 and col2 based on t
    ImVec4 lerpColor = ImVec4(
        ImLerp(col1.Value.x, col2.Value.x, t),
        ImLerp(col1.Value.y, col2.Value.y, t),
        ImLerp(col1.Value.z, col2.Value.z, t),
        ImLerp(col1.Value.w, col2.Value.w, t)
    );

    ImU32 color = ImGui::ColorConvertFloat4ToU32(lerpColor);

    return color;
}

void user_interface_t::draw_exit_button() {

    ImVec2 pos = { static_cast<float>(gvars.window.width - 25), 0 };
    ImVec2 size = { 25, 25 };

    static FadeAnimationState state1;

    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), get_fade_color(pos, { pos.x + size.x, pos.y + size.y }, ImColor(0, 0, 0), ImColor(255, 0, 0), state1));

    ImGui::SetWindowFontScale(2.f);
    draw->AddText({ pos.x + 5, pos.y - 3 }, ImColor(255, 255, 255), "x");
    ImGui::SetWindowFontScale(1.f);

    if (ImGui::IsMouseHoveringRect(pos, { pos.x + size.x, pos.y + size.y }))
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            gvars.states.should_exit = true;
}








void user_interface_t::draw_window_title() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    draw->AddText({ 6, 6 }, ImColor(200, 200, 200), "Bitwise - Idle Game");

}

// draws the titlebar and outline around the window
void user_interface_t::draw_titlebar() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    draw->AddRect({ 0, 0 }, { static_cast<float>(gvars.window.width), 25 }, ImColor(60, 60, 60));
    draw->AddRect({ 0, 0 }, { static_cast<float>(gvars.window.width), static_cast<float>(gvars.window.height) }, ImColor(60, 60, 60));

}


void user_interface_t::do_draw() noexcept {

    ImGui::StyleColorsDarkC();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    if (ImGui::Begin("main window", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground)) {

        ImDrawList* draw = ImGui::GetWindowDrawList();

        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        draw_exit_button();
        draw_window_title();
        draw_titlebar();
        
    }

    ImGui::End();

}