#include "user_interface.hpp"
#include "../dx9_imgui/dx9_imgui.hpp"
#include "../dx9_imgui/imgui/imgui.h"
#include "../dx9_imgui/imgui/imgui_impl_dx9.h"
#include "../dx9_imgui/imgui/imgui_impl_win32.h"
#include "../dx9_imgui/imgui/imgui_stdlib.h"
#include "../globals/globals.hpp"
#include "../dx9_imgui/imgui/imgui_internal.h"

#include <string>

user_interface_t user_interface;
// perform rendering tasks


void user_interface_t::draw_intro_logo() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

 
    if (this->intro_state == 0) {

        // static_cast<float>(gvars.window.width)
        // static_cast<float>(gvars.window.height)
        constexpr float x = 622, y = 241;
        const float xx = (static_cast<float>(gvars.window.width) - x) / 2;
        const float yy = (static_cast<float>(gvars.window.height) - y) / 2;


        ImGuiIO& io = ImGui::GetIO();
        ImColor color = ImColor(255, 255, 255, 0);


        this->anim_timer += io.DeltaTime;
        this->anim_timer = ImClamp(this->anim_timer, 0.0f, this->anim_duration * 2 + this->stay_duration1);

        static float first = 0.f;
        static float second = 0.f;

        if (this->anim_timer < this->anim_duration) {

            first += io.DeltaTime;

            color = ImColor(255, 255, 255, ImLerp(0, 255, first / this->anim_duration));

        }
        else if ((this->anim_timer >= this->anim_duration) && (this->anim_timer < (this->anim_duration + this->stay_duration1)))
        {

            color = ImColor(255, 255, 255, 255);

        }
        else if ((this->anim_timer >= this->anim_duration + this->stay_duration1) && (this->anim_timer < (this->anim_duration * 2 + this->stay_duration1))) {

            second += io.DeltaTime;

            color = ImColor(255, 255, 255, ImLerp(255, 0, second / this->anim_duration));

        }

        //draw->AddText({ 20, 50 }, ImColor(255, 255, 255), std::string("timer: " + std::to_string(this->anim_timer)).c_str());
        //draw->AddText({ 20, 70 }, ImColor(255, 255, 255), std::string("first: " + std::to_string(first)).c_str());
        //draw->AddText({ 20, 90 }, ImColor(255, 255, 255), std::string("secnd: " + std::to_string(second)).c_str());

        draw->AddImage(gvars.textures.images[gvars.textures.Image_springtime], { xx, yy }, { xx + x, yy + y }, { 0, 0 }, { 1, 1 }, color);

        if (this->anim_timer == 4.6f) {
            this->intro_state = 1;
            this->anim_timer = 0.f;
        }
        
    }
    else if (this->intro_state == 1) {

        constexpr float x = 441, y = 131;
        const float xx = (static_cast<float>(gvars.window.width) - x) / 2;
        const float yy = (static_cast<float>(gvars.window.height) - y) / 2;

        ImGuiIO& io = ImGui::GetIO();
        ImColor color = ImColor(255, 255, 255, 0);


        this->anim_timer += io.DeltaTime;
        this->anim_timer = ImClamp(this->anim_timer, 0.0f, this->anim_duration * 2 + this->stay_duration2);

        static float first = 0.f;
        static float second = 0.f;

        if (this->anim_timer < this->anim_duration) {

            first += io.DeltaTime;

            color = ImColor(255, 255, 255, ImLerp(0, 255, first / this->anim_duration));

        }
        else if ((this->anim_timer >= this->anim_duration) && (this->anim_timer < (this->anim_duration + this->stay_duration2)))
        {

            color = ImColor(255, 255, 255, 255);

        }
        else if ((this->anim_timer >= this->anim_duration + this->stay_duration2) && (this->anim_timer < (this->anim_duration * 2 + this->stay_duration2))) {

            second += io.DeltaTime;

            color = ImColor(255, 255, 255, ImLerp(255, 0, second / this->anim_duration));

        }

        //draw->AddText({ 20, 50 }, ImColor(255, 255, 255), std::string("timer: " + std::to_string(this->anim_timer)).c_str());
        //draw->AddText({ 20, 70 }, ImColor(255, 255, 255), std::string("first: " + std::to_string(first)).c_str());
        //draw->AddText({ 20, 90 }, ImColor(255, 255, 255), std::string("secnd: " + std::to_string(second)).c_str());

        draw->AddImage(gvars.textures.images[gvars.textures.Image_bitwise], { xx, yy }, { xx + x, yy + y }, { 0, 0 }, { 1, 1 }, color);

        if (this->anim_timer == 3.6f) {
            this->intro_state = 2;
            this->anim_timer = 0.f;
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
    draw->AddText({ pos.x + 5, pos.y + 1 }, ImColor(255, 255, 255), "x");
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

//#define skip_intro // comment this line out if you don't want to skip

void user_interface_t::draw_title_screen() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    ImVec2 button_size = { 250.f, 60.f };
    ImVec2 play_button = { (gvars.window.width - button_size.x) / 2, (gvars.window.height - button_size.y) * 0.35f };

    draw->AddRectFilled(play_button, { play_button.x + button_size.x, play_button.y + button_size.y }, ImColor(255, 255, 255));

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

#ifdef skip_intro
        this->intro_state = 2;
#endif

        if (this->intro_state < 2)
            draw_intro_logo();
        else
            draw_title_screen();
        
    }

    ImGui::End();

    ImGui::PopFont();

}