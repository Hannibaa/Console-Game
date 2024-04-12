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
	cgu::fRect      game_rect{ coord1.x, coord1.y, 30.f, 15.f };

	int id_boxes{};
	cgu::console.construct_console(g_lx, g_ly); // we need to start terminating thread her


	cgu::Particule       p(10, 10);

	// create movement
	//cgu::cin::FFreeMotion      motion([](float x) { return 50.f + 5.f * std::sinf(x/50.f); }, 
	//                                  [](float x) { return 10.f + 5.f * std::cosf(x/50.f); });

	cgu::cin::FreeMotion         motion;

	motion.set_bounds(game_rect.shrink(1.f));
	motion.set_speed(0.11f, 0.11f);

	// if we want generate some number of particule so we use vector
	struct Part {
		cgu::Particule          p;
		cgu::cin::FreeMotion    motion;
	};

	std::vector<Part>           vp;

	auto generate = [&](const cgu::fRect& r) {
		rng::fRG<float>   frand;
		rng::iRG<int>     irand;

		Part  p;
		p.p.set_id(++id_boxes);
		p.p.set_position(frand(r.x, r.x + r.dx), frand(r.y, r.y + r.dy));
		p.motion.set_bounds(r.shrink(1.f));
		float v = frand(0.01f, 0.1f);
		p.motion.set_speed(v, v);

		return p;
		};

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

	while (cgu::console.is_open()) {
		cgu::Dtime = cgu::Dt60 * cgu::elps * 60.f;
		timer.reset();
		// Header , Title, time elapsed ,
		swprintf_s(cgu::title, 255, L"Console"
			L"Fps[%5.2f]"
			L"Boxes[%d]"
			L"[%3.2f, %3.2f]", cgu::fps, vp.size() , p.get_position().x, p.get_position().y);

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


		// add particule
		if (KeyReleased(_u('A'))) {
			vp.push_back(generate(game_rect));
		}


		// Cinematic
		motion(&p, cgu::Dtime);
		
		for (auto& v : vp) v.motion(&v.p, cgu::Dtime);

		// Collision



		// Drawing section
	__next:
		p.draw();
		for (auto& v : vp) v.p.draw();
		
		cgu::draw_rectangle(cgu::Rect<int>(game_rect), 0x2592);
		cgu::draw_rectangle({ 0,0,g_lx - 1, g_ly - 1 });
		cgu::print_text_at(wsMenuTail, 1, g_ly - 1);
		cgu::console.display();



		//if (cgu::get_out()) break;
		cgu::elps = timer.get_elapsed_time();
		cgu::fps = 1.f / cgu::elps;

	}


	std::cout << "End of Program..\n";
	return 0;
}