#pragma once

class user_interface_t {

private: int intro_state = 0;
private: float anim_duration = 0;
private: void draw_intro_logo();
private: void draw_exit_button();
private: void draw_window_title();
private: void draw_overlay();

// runs every frame. handles rendering the ui. called in a while loop in main.cpp
public: void do_draw() noexcept;
	  
};

extern user_interface_t user_interface;