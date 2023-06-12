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

    switch (this->intro_state) {

        case 0: {

            // static_cast<float>(gvars.window.width)
            // static_cast<float>(gvars.window.height)
            constexpr float x = 622, y = 241;
            const float xx = (static_cast<float>(gvars.window.width) - x) / 2;
            const float yy = (static_cast<float>(gvars.window.height) - y) / 2;

            draw->AddImage(gvars.textures.images[gvars.textures.Image_springtime], { xx, yy }, { xx + x, yy + y });



            //draw->AddImage(gvars.textures.images[gvars.textures.Image_springtime], { 50, 50 }, { 50 + x, 50 + y });


        }

        case 1: {


        }

        case 2: {
            return;
        }
        
    }
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
    ImColor color = ImColor(
        ImLerp(col1.Value.x, col2.Value.x, t),
        ImLerp(col1.Value.y, col2.Value.y, t),
        ImLerp(col1.Value.z, col2.Value.z, t),
        ImLerp(col1.Value.w, col2.Value.w, t)
    );

    return color;
}

void user_interface_t::draw_exit_button() {

    ImVec2 pos = { static_cast<float>(gvars.window.width - 25), 0 };
    ImVec2 size = { 25, 25 };

    static FadeAnimationState state1;

    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), get_fade_color(pos, { pos.x + size.x, pos.y + size.y }, ImColor(255, 0, 0, 0), ImColor(255, 0, 0, 255), state1));

    ImGui::PushFont(gvars.textures.fonts[20]);
    draw->AddText({ pos.x + 5, pos.y - 3 }, ImColor(255, 255, 255), "x");
    ImGui::PopFont();

    if (ImGui::IsMouseHoveringRect(pos, { pos.x + size.x, pos.y + size.y }))
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
            gvars.states.should_exit = true;
}








void user_interface_t::draw_window_title() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    draw->AddText({ 6, 6 }, ImColor(200, 200, 200), "Bitwise - Idle Game");

}

// draws the titlebar and outline around it + around the window
void user_interface_t::draw_overlay() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    ImColor background = ImColor(30, 30, 30);
    ImColor outline = ImColor(150, 150, 150);

    // background
    draw->AddRectFilled({ 0, 0 }, { static_cast<float>(gvars.window.width), static_cast<float>(gvars.window.height) }, background);
    draw->AddRect({ 0, 0 }, { static_cast<float>(gvars.window.width), 25 }, outline);
    draw->AddRect({ 0, 0 }, { static_cast<float>(gvars.window.width), static_cast<float>(gvars.window.height) }, outline);

}


void user_interface_t::do_draw() noexcept {

    ImGui::PushFont(gvars.textures.fonts[14]);

    ImGui::StyleColorsDarkC();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    if (ImGui::Begin("main window", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground)) {

        ImDrawList* draw = ImGui::GetWindowDrawList();

        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        draw_overlay();
        draw_exit_button();
        draw_window_title();
        draw_intro_logo();
        
    }

    ImGui::End();

    ImGui::PopFont();

}