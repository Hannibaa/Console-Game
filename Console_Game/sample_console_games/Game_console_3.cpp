#include <iostream>
#include <queue>
#include <array>
#include "game_timer.h"
#include "shipes.h"
#include "sprite.h"

/*
		   Box Collision detection
		   Author : KADDA Aoues
		   Version: 1.0.0.0
		   Collision detection 
*/



int main() {

	// Console : -------------------------------------------------
	const int _length = 145;
	const int _width = 35;

	cu::console.construct_console(_length, _width);

	// Object : --------------------------------------------------
	// create two object and one pointer that switch between objects
	cu::Sprite* ptr{};   // pointer to null object:

	cu::Sprite  box1({ L"\x250f\x2501\x2501\x2513", L"\x2503  \x2503", L"\x2517\x2501\x2501\x251b" });
	box1.set_controle_key();

	cu::Sprite  box2({ L"\x250f\x2501\x2501\x2513", L"\x2503  \x2503", L"\x2517\x2501\x2501\x251b" },10.f, 20.f);
	box2.set_controle_key();
	


	// Menu : ----------------------------------------------------
	std::wstring wstrMenu = LR"(P : Controle box 1
O : Controle Box 2
R : Remove any Controle
I : Pause Move
ESCAP : Exit 
)";

	// timer to calculate frame per seconds
	Game_Timer g_timer;
	bool b_pause = false;
	int collision_count{};

	// elapsed time variable, maximum value and minimul value
	float elapsed_time{ 1.f };
	// string wchar_t buffer for title
	wchar_t s[150]{};
	std::wstringstream ss;

	// Start loop for game
	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;

		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per sec[%5.2f]|Collision[%d]", frames,collision_count
			);
		// title
		TITLE(s);

		// Show the Menu
		if (KeyReleased(_u('I'))) {
			b_pause = !b_pause;
		}

		if (b_pause) {
			cu::message_box(30, 10, L"Menu", wstrMenu);
			goto near_end;
		}




		cu::console.clear_screen();


		// Controle box1
		if (KeyReleased(_u('P'))) {
			ptr = &box1;
		}

		// Controle box2
		if (KeyReleased(_u('O'))) {
			ptr = &box2;
		}

		// Remove one Particule
		if (KeyReleased(_u('R'))) {
			ptr = nullptr;
		}

		// draw particule
		if (ptr) {
			ptr->move();
		}


		// Collision Removal

		if (cu::is_collid(box1,box2)) {
			cu::collision_process(box1, box2);
			++collision_count;
		}

		ss << box1.get_bounds().left << L" , " << box1.get_bounds().top;
		// printing some text at : 
		cu::print_text_at(ss.str(), 120, 2);
		ss.str(L"");

		///////////// drawing section priority  /.///////

		box1.draw();
		box2.draw();

		// draw rectangle scene
		cu::draw_rectangle({ 0,0,_length - 1, _width - 1 });

	near_end:
		
		cu::console.draw();
		elapsed_time = g_timer.get_elapsed_time();
		if (cu::get_out()) break;
	}

	return 0;
}
