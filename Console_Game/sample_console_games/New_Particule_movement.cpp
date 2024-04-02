#include <unordered_map>
#include <functional>
#include "MyLib/chrono/Game_timer.h"
#include "../include/Particules.h"
#include "../include/Cinematic.hpp"
#include "MyLib/random_generator.h"




int main() {

	int g_lx = 145;
	int g_ly = 35;

	// box rectangle coordinate
	cgu::fPoint2d   coord1{ 5.f , 5.f };
	cgu::fPoint2d   coord2{ 70.f , 20.f };
	cgu::fRect      game_rect{ coord1.x, coord1.y, 65.f, 15.f };

	int id_boxes{};
	cgu::console.construct_console(g_lx, g_ly);


	cgu::Particule       p(10, 10);

	// create movement
	cgu::cin::FreeMotion      motion;
	motion.set_bounds(5.f, 5.f, 65.f, 15.f);
	motion.set_speed(0.09f, 0.09f);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Menu Setup : 
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::wstring  wsMenuTail = L"Esc:Quite M:Menu R:Reset A:Add -:Remv";
	std::wstring  wsMenuBody = LR"(M: Pause and show menu
R: Reset Moving Box
U: Increase speed
D: Decrease speed
-: Remove box)";
	std::wstring  wsDataBox;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Main Loop  
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer  timer;
	bool              b_pause{ false };
	cgu::Dt60 = 1.5;

	while (true) {
		cgu::Dtime = cgu::Dt60 * cgu::elps * 60.f;
		timer.reset();
		// Header , Title, time elapsed ,
		swprintf_s(cgu::title, 255, L"Console"
			L"Fps[%5.2f]"
			L"Boxes[%d]", cgu::fps, 0);

		TITLE(cgu::title);
		// clear screen
		cgu::console.clear_screen();

		// Menu
		if (KeyReleased(_u('M'))) {
			b_pause = !b_pause;
		}
		if (b_pause) {
			cgu::message_box(g_lx / 2, g_ly / 2, L"Menu", wsMenuBody);
			goto __next;
		}



		// Cinematic
		motion(&p, cgu::Dtime);


		// Collision



		// Drawing section
	__next:
		p.draw();
		
		cgu::draw_rectangle(cgu::Rect<int>(game_rect), 0x2592);
		cgu::draw_rectangle({ 0,0,g_lx - 1, g_ly - 1 });
		cgu::print_text_at(wsMenuTail, 1, g_ly - 1);
		cgu::console.display();



		if (cgu::get_out()) break;
		cgu::elps = timer.get_elapsed_time();
		cgu::fps = 1.f / cgu::elps;

	}


	std::cout << "End of Program..\n";
	return 0;
}