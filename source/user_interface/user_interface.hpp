#pragma once

class user_interface_t {

private: int intro_state = 2;
private: float anim_duration = 0.8f;
private: float stay_duration1 = 3.f;
private: float stay_duration2 = 2.f;
private: float anim_timer = 0.f;

private: void draw_intro_logo();
private: void draw_exit_button();
private: void draw_window_title();
private: void draw_overlay();
private: void draw_stripes();
private: void draw_title_screen();
private: void draw_settings_screen();
private: void draw_play_screen();
private: void draw_game();

// runs every frame. handles rendering the ui. called in a while loop in main.cpp
public: void do_draw() noexcept;
	  
};

extern user_interface_t user_interface;