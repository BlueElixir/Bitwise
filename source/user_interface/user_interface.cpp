#include "user_interface.hpp"
#include "../dx9_imgui/dx9_imgui.hpp"
#include "../dx9_imgui/imgui/imgui.h"
#include "../dx9_imgui/imgui/imgui_impl_dx9.h"
#include "../dx9_imgui/imgui/imgui_impl_win32.h"
#include "../dx9_imgui/imgui/imgui_stdlib.h"
#include "../globals/globals.hpp"
#include "../dx9_imgui/imgui/imgui_internal.h"
#include "../game_manager/game_manager.hpp"

#include "../savefile/savefile.hpp"

#include <filesystem>
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
    ImDrawList* draw_fg = ImGui::GetForegroundDrawList();

    ImColor background = ImColor(30, 30, 30);
    ImColor outline = ImColor(150, 150, 150);

    // background
    draw->AddRectFilled({ 0, 0 }, { static_cast<float>(gvars.window.width), static_cast<float>(gvars.window.height) }, background);
    draw_fg->AddRect({ 0, 0 }, { static_cast<float>(gvars.window.width), 25 }, outline);
    draw_fg->AddRect({ 0, 0 }, { static_cast<float>(gvars.window.width), static_cast<float>(gvars.window.height) }, outline);

}

// also draws game version because why not :D
void user_interface_t::draw_stripes() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    ImGuiIO& io = ImGui::GetIO();

    static float test = 0;
    test += io.DeltaTime;

    if (test >= 2.5f)
        test = 0.f;

    for (float i = 0.f; i < gvars.window.width * 1.5; ++i) {

        draw->AddLine({ i * 25 + test * 10, 25 }, { 0, 25 + i * 25 + test * 10 }, ImColor(30, 85, 125, 150), 2.f);

    }

    draw->AddText({ gvars.window.width - 5 - ImGui::CalcTextSize(gvars.states.game_version.c_str()).x, gvars.window.height - 5 - ImGui::CalcTextSize(gvars.states.game_version.c_str()).y }, ImColor(255, 255, 255), gvars.states.game_version.c_str());
    draw->AddText({ 5, gvars.window.height - 5 - ImGui::CalcTextSize(gvars.states.developer_str.c_str()).y }, ImColor(255, 255, 255), gvars.states.developer_str.c_str());

}

void user_interface_t::draw_title_screen() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    constexpr float x = 441, y = 131;
    const float xx = (static_cast<float>(gvars.window.width) - x) / 2;
    const float yy = (static_cast<float>(gvars.window.height) - y) / 2 - y;
    draw->AddImage(gvars.textures.images[gvars.textures.Image_bitwise], { xx, yy }, { xx + x, yy + y }, { 0, 0 }, { 1, 1 }, ImColor(255, 255, 255, 255));



    ImVec2 button_size = { 250.f, 50.f };
    ImVec2 play_button = { (gvars.window.width - button_size.x) / 2, (gvars.window.height - button_size.y) * 0.55f };

    ImGui::PushFont(gvars.textures.fonts[30]);

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(60, 170, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(70, 180, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(50, 150, 230, 255));

    ImGui::SetCursorPos(play_button);
    if (ImGui::Button("Play", button_size))
        this->intro_state = 4;

    ImGui::SetCursorPos({ play_button.x, play_button.y + button_size.y * 1.2f });
    if (ImGui::Button("Settings", button_size))
        this->intro_state = 3;

    ImGui::SetCursorPos({ play_button.x, play_button.y + button_size.y * 2.4f });
    if (ImGui::Button("Quit", button_size))
        gvars.states.should_exit = true;

    ImGui::PopFont();

}

void user_interface_t::draw_settings_screen() {

    std::vector<int> res_w = { 960, 1280, 1360, 1536, 1920, 2560 };
    std::vector<int> res_h = { 540, 720, 768, 864, 1080, 1440 };

    std::vector<std::string> resolutions = {
        "960x540",
        "1280x720",
        "1360x768",
        "1536x864",
        "1920x1080",
        "2560x1440",
    };

    enum resolutions {
        res_960x540   = 0,
        res_1280x720  = 1,
        res_1360x768  = 2,
        res_1536x864  = 3,
        res_1920x1080 = 4,
        res_2560x1440 = 5,
    };

    static std::string current_resolution = resolutions[res_1360x768];


    ImGui::SetCursorPos({ 100, 100 });
    //ImGui::BeginChild("john", { 500, 500 }, true, ImGuiWindowFlags_NoBackground);

    //if (ImGui::BeginCombo("##combo", current_resolution.c_str())) { // The second parameter is the label previewed before opening the combo.

    //    for (int n = 0; n < resolutions.size(); ++n) {

    //        bool is_selected = (current_resolution == resolutions[n]); // You can store your selection however you want, outside or inside your objects

    //        if (ImGui::Selectable(resolutions[n].c_str(), is_selected)) {
    //            current_resolution = resolutions[n];

    //            gvars.window.width = res_w[n];
    //            gvars.window.height = res_h[n];
    //            gvars.ui.resolution_changed = true;
    //            //dx9::resize_window(res_w[n], res_h[n]);
    //        }

    //        if (is_selected)
    //            ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)

    //    }

    //    ImGui::EndCombo();
    //}

    ImGui::PushFont(gvars.textures.fonts[50]);
    ImGui::Text("Under construction,\ncheck back later!");

    ImGui::SetCursorPos({ 100, 500 });
    if (ImGui::Button("Back", { 300.f, 60.f }))
        this->intro_state = 2;

    ImGui::PopFont();

    //ImGui::EndChild();

}

void user_interface_t::draw_play_screen() {

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(60, 170, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(70, 180, 255, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(50, 150, 230, 255));


    ImGui::SetCursorPos({ gvars.window.width * 0.125f, gvars.window.height * 0.15f });
    ImGui::BeginChild("PlayLeft", { gvars.window.width * 0.35f, gvars.window.height * 0.72f }, true);

    ImGui::PushFont(gvars.textures.fonts[30]);
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX((gvars.window.width * 0.35f - ImGui::CalcTextSize("New Game").x) / 2);
    ImGui::Text("New Game");
    ImGui::PopFont();
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX((gvars.window.width * 0.35f - ImGui::CalcTextSize("Start a fresh game of Bitwise.").x) / 2);
    ImGui::TextWrapped("Start a fresh game of Bitwise.");

    ImGui::SetCursorPosY(gvars.window.height * 0.72f - 40.f);

    if (ImGui::Button("Start Game!", { gvars.window.width * 0.3385f, 30.f })) {
        savefile.create_config();
        this->intro_state = 5;
    }

    ImGui::EndChild();

    ImGui::SetCursorPos({ gvars.window.width * 0.525f, gvars.window.height * 0.15f });
    ImGui::BeginChild("PlayRight", { gvars.window.width * 0.35f, gvars.window.height * 0.72f }, true);

    ImGui::PushFont(gvars.textures.fonts[30]);
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX((gvars.window.width * 0.35f - ImGui::CalcTextSize("Load Save").x) / 2);
    ImGui::Text("Load Save");
    ImGui::PopFont();
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX((gvars.window.width * 0.35f - ImGui::CalcTextSize("Load a previous savegame of Bitwise.").x) / 2);
    ImGui::TextWrapped("Load a previous savegame of Bitwise.");

    if (std::filesystem::exists(gvars.config.file_path)) {
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

        try {
            ImGui::Text(std::string(savefile.config["currency"]["name"].get<std::string>() + " collected: " + std::to_string((int)savefile.config["currency"]["amount"].get<float>())).c_str());


            static time_t s_unixtime = savefile.config["last_save_time"].get<int>();
            std::tm s_localtime;
            localtime_s(&s_localtime, &s_unixtime);

            char s_formattedtime[30];
            std::strftime(s_formattedtime, sizeof(s_formattedtime), "%Y.%m.%d, %H:%M:%S", &s_localtime);
            ImGui::Text(std::string("Last saved: " + std::string(s_formattedtime)).c_str()); // + year + "." + month + "." + day + ", " + hour + ":" + minute + ":" + second).c_str());



            static time_t c_unixtime = savefile.config["save_creation_time"].get<int>();

            if (c_unixtime != 0) {

                std::tm c_localtime;
                localtime_s(&c_localtime, &c_unixtime);

                char c_formattedtime[30];
                std::strftime(c_formattedtime, sizeof(c_formattedtime), "%Y.%m.%d, %H:%M:%S", &c_localtime);
                ImGui::Text(std::string("Save created: " + std::string(c_formattedtime)).c_str()); // + year + "." + month + "." + day + ", " + hour + ":" + minute + ":" + second).c_str());

            }
            else {
                ImGui::Text(std::string("Save created: unknown").c_str());
            }
            
        }
        catch (std::exception e) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255));
            ImGui::TextWrapped("\n\nSomething is wrong with your\nsavefile, is it outdated?");
            ImGui::Text(std::string("Error: " + std::string(e.what())).c_str());
            ImGui::PopStyleColor();
        }

    }


    ImGui::SetCursorPosY(gvars.window.height * 0.72f / 2);

    if (!std::filesystem::exists(gvars.config.file_path)) {

        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 50, 50, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(50, 50, 50, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(50, 50, 50, 255));

        ImGui::TextWrapped("You don't have a savefile! Create a new\ngame or add a savefile to C:\\Games\\Bitwise!");

    }

    ImGui::SetCursorPosY(gvars.window.height * 0.72f - 40.f);
    if (ImGui::Button("Continue!", { gvars.window.width * 0.3385f, 30.f })) {
        savefile.config = savefile.read_config();
        this->intro_state = 5;
    }

    if (!std::filesystem::exists(gvars.config.file_path)) {
        ImGui::PopStyleColor(3);
    }


    ImGui::EndChild();

    ImGui::PopStyleVar();

}

void user_interface_t::draw_game() {

    ImGuiIO& io = ImGui::GetIO();

    gvars.states.curtime += io.DeltaTime + savefile.config["upgrades"].at(3)["level"].get<int>() * savefile.config["upgrades"].at(3)["increment"].get<float>();

    ImGui::SetCursorPos({ 1, 28 });
    ImGui::BeginChild("GameWindow", { gvars.window.width - 2.f, gvars.window.height - 29.f }, false, ImGuiWindowFlags_NoBackground);

    ImDrawList* draw = ImGui::GetWindowDrawList();

    draw->AddRectFilled({ 4.f, 28.f }, { gvars.window.width - 4.f, 90.f }, ImColor(50, 50, 50, 255), 5.f);

    ImGui::SetCursorPosX(3.f);
    ImGui::ProgressBar(savefile.config["currency"]["amount"].get<float>() / 1000000.f, { gvars.window.width - 8.f, 30.f }, std::string(std::to_string((int)(savefile.config["currency"]["amount"].get<float>())) + " | " + std::to_string((int)(savefile.config["currency"]["amount"].get<float>() / 1000000.f * 100)) + "%").c_str());
    
    ImGui::SetCursorPosX(7.f);
    ImGui::Text(std::to_string(gvars.states.curtime).c_str());


    // upgrades
    for (int i = 0; i < 4; ++i) {

        if (ImGui::Button(std::string("Upgrade: " + savefile.config["upgrades"].at(i)["name"].get<std::string>() + " " + std::to_string(savefile.config["upgrades"].at(i)["level"].get<int>()) + " -> " + std::to_string(savefile.config["upgrades"].at(i)["level"].get<int>() + 1) + " | " + std::to_string(savefile.config["upgrades"].at(i)["base_price"].get<float>() + savefile.config["upgrades"].at(i)["base_price"].get<float>() * (savefile.config["upgrades"].at(i)["level"].get<int>() * savefile.config["upgrades"].at(i)["price_modifier"].get<float>())) + " Bits").c_str())) {

            if (savefile.config["upgrades"].at(i)["base_price"].get<float>() + savefile.config["upgrades"].at(i)["base_price"].get<float>() * (savefile.config["upgrades"].at(i)["level"].get<int>() * savefile.config["upgrades"].at(i)["price_modifier"].get<float>()) <= savefile.config["currency"]["amount"].get<float>()) {

                savefile.config["currency"]["amount"] = savefile.config["upgrades"].at(i)["base_price"].get<float>() + savefile.config["upgrades"].at(i)["base_price"].get<float>() * (savefile.config["upgrades"].at(i)["level"].get<int>() * savefile.config["upgrades"].at(i)["price_modifier"].get<float>());

            }

        }

    }


    ImGui::EndChild();

}

void user_interface_t::do_draw() noexcept {

    ImGui::PushFont(gvars.textures.fonts[14]);
    ImGui::StyleColorsDarkC();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    if (ImGui::Begin("main window", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground)) {

        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImDrawList* drawfg = ImGui::GetForegroundDrawList();

        ImGuiStyle* style = &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        draw_overlay();
        draw_exit_button();
        draw_window_title();

        switch (this->intro_state) {
        case 0:
        case 1:
            draw_intro_logo();
            break;

        case 2:
            draw_stripes();
            draw_title_screen();
            break;

        case 3:
            draw_stripes();
            draw_settings_screen();
            break;

        case 4:
            draw_stripes();
            draw_play_screen();
            break;

        case 5: {

            try {

                draw_game();
                break;

            }
            catch (std::exception e) {

                std::vector<std::string> _warning = {
                    "Warning: your savefile seems to be damaged.",
                    "Unfortunately, this means that you will not be",
                    "able to continue playing on this save file.",
                    "You can try to contact us on Discord for help",
                    "as a last resort: discord.gg/sKSR3C7hBD/"
                };

                draw->AddRectFilled({ gvars.window.width * 0.31f + 2, gvars.window.height * 0.35f + 2 }, { gvars.window.width * 0.69f + 2, gvars.window.height * 0.58f + 2 }, ImColor(0, 0, 0, 150), 10.f);
                draw->AddRectFilled({ gvars.window.width * 0.31f, gvars.window.height * 0.35f }, { gvars.window.width * 0.69f, gvars.window.height * 0.58f }, ImColor(40, 40, 40), 10.f);
                draw->AddRect({ gvars.window.width * 0.31f, gvars.window.height * 0.35f }, { gvars.window.width * 0.69f, gvars.window.height * 0.58f }, ImColor(255, 50, 50), 10.f);

                for (size_t i = 0; i < _warning.size(); ++i) {
                    draw->AddText({ (gvars.window.width - ImGui::CalcTextSize(_warning[i].c_str()).x) / 2.f, gvars.window.height * 0.37f + 16.f*i }, ImColor(255, 0, 0), _warning[i].c_str());
                }
                
                ImGui::SetCursorPos({ gvars.window.width * 0.315f, gvars.window.height * 0.503f });
                if (ImGui::Button("Okay", { gvars.window.width * 0.368f, gvars.window.height * 0.03f })) {
                    //savefile.create_config();
                    //savefile.write_config();
                    user_interface.intro_state = 2;
                }

            }
        }
            
        }
        
    }

    ImGui::End();

    ImGui::PopFont();

}