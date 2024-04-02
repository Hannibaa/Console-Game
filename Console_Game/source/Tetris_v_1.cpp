#include "Tetris_v_1.hpp"
#include "MyLib/chrono/Game_timer.h"
#include "MyLib/chrono/to_day.h"
#include "../include/wstring_utility.h"

void prepare_tetromino() {
	for (int i = 0; i != 7; ++i) {
		Str::replace_all_by(cgu::tetromino[i], L'.', L' ');
		Str::replace_all_by(cgu::tetromino[i], L'X', L'\x2588');
	}
}

std::wstring  time_to_day() {
	std::chrono::sys_seconds  time =
		std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
	std::wstringstream ss;
	ss << time;
	return ss.str();
}


int main() {

	// Console ;---------------------------------------------------------------------------------
	const int g_lx{ 145 };
	const int g_ly{ 35 };

	const int g_flx{ 55 };
	const int g_fly{ 34 };

	cgu::iRect  field{ 1,1,g_flx,g_fly -2 };

	// convert tetromino to solid char
	prepare_tetromino();
	int next{};

	cgu::console.construct_console(g_lx, g_ly);

	// tetris object  : --------------------------------------------------------------------------
	tetro4    t2{ cgu::tetromino[2], 10,10};
	Motion    move;
	move.set_speed(0.5f, 0.5f);
	move.set_bounds(1, 1, g_flx, g_fly);

	Field<g_flx, g_fly - 3>           g_field(0x2591, 2, 2);
	g_field.set_last_line();

	Collision                 collision;

	

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//   MAIN LOOP   
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	Time::Game_Timer       timer;
	bool b_pause{ false };
	cgu::Dt60 = 1.5f;
	
	while (true) {
		timer.reset();
		cgu::Dtime = cgu::Dt60 * 60.f * cgu::elps;

		// Title : 
		swprintf_s(cgu::title, 255, L"TETRIS::FperSec[%5.2f]", cgu::fps);
		TITLE(cgu::title);

		// Menu and pause
		if (KeyReleased(_u('M'))) {
			b_pause = !b_pause;
		}
		if (b_pause) {
			cgu::message_box(g_lx / 2, g_ly / 2, L"Menu", L"M: Menu");
			goto __next;
		}
		// Clear screen
		cgu::console.clear_screen();


		// Cinematic:
		if (KeyReleased(_u('R'))) {
			t2.rotate();
		}
		if (KeyReleased(_u('N'))) {
			t2.set_buffer(cgu::tetromino[(++next) % 7]);
		}



		move(&t2, cgu::Dtime);
		//collision(&t2, 2, g_flx, 2, g_fly - 2);
		collision.is_collid(&t2, &g_field);


		__next:
		// Drawing

		g_field.draw();
		t2.draw();

		cgu::draw_rectangle({ 0,0,g_lx - 1, g_ly - 1 });
		cgu::draw_rectangle(field, 0x2593);
		cgu::print_text_at(time_to_day(), 120, 34);
		cgu::console.display();

		// exit 
		if (cgu::get_out()) break;
		cgu::elps = timer.get_elapsed_time();
		cgu::fps = 1.f / cgu::elps;
	}



	std::cout << "End of Game...\n";
	return 0;
}