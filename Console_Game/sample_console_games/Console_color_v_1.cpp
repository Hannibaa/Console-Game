#include <unordered_map>
#include <functional>
#include "MyLib/chrono/Game_timer.h"
#include "../include/shape_v_1.hpp"
#include "../include/Cinematic.hpp"
#include "MyLib/random_generator.h"


using TSharedBox = std::shared_ptr<cgu::DynBox>;

TSharedBox make_new_box() {
	rng::iRG<int> irand;
	auto dimx = irand(2, 3);
	auto dimy = irand(2, 7);
	auto x = irand(1, 123);
	auto y = irand(1, 5);

	TSharedBox ptrbox = std::make_shared<cgu::DynBox>(dimx, dimy, x, y);

	return ptrbox;
}


int main() {

	int g_lx = 145;
	int g_ly = 35;
	int id_boxes{};
	cgu::console.construct_console(g_lx,g_ly);


	TSharedBox  ptr = make_new_box();

	std::vector<cgu::DynBox>  boxes;

	auto add_box = [&] {
		rng::iRG<int>   irand;
		boxes.emplace_back(irand(2, 4), irand(2, 4), irand(5, g_lx - 5), irand(5, g_ly - 5));
		boxes[boxes.size() - 1].set_id(++id_boxes);
		};
	
	cgu::DynSprite    Bar{ std::wstring(3*4, 0x2593), 3,4,10,10};

	cgu::DynBox     box(2, 6, 10, 10);
	cgu::cin::FreeMotion      motion;
	motion.set_bounds(1, 1, g_lx - 1, g_ly - 1);
	motion.set_speed(0.5f, 0.5f);

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
	std::wstring  wsDataBox ;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Main Loop  
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer  timer;
	bool              b_pause{ false };
	cgu::Dt60 = 1.5;
	cgu::fPoint2d     original_point = Bar.get_position();

	while (true) {
		cgu::Dtime = cgu::Dt60 * cgu::elps * 60.f;
		timer.reset();
		// Header , Title, time elapsed ,
		swprintf_s(cgu::title, 255, L"Console"
			                        L"Fps[%5.2f]"
			                        L"Boxes[%d]", cgu::fps , boxes.size());

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

		if (KeyReleased(_u('R'))) {
			ptr.reset();
			ptr = make_new_box();
			Bar.set_position(original_point);
		}

		if (KeyReleased(_u('A'))) {
			add_box();
		}

		if (KeyReleased(VK_OEM_MINUS)) {
			if (!boxes.empty()) boxes.pop_back();
		}


		// Movement
		motion(ptr.get(), cgu::Dtime);

		if (KeyReleased(_u('U'))) {
			auto sp = motion.get_speed();
			motion.set_speed(sp.x + 0.01f, sp.y + 0.01f);
		}
		if (KeyReleased(_u('D'))) {
			auto sp = motion.get_speed();
			motion.set_speed(sp.x - 0.01f, sp.y - 0.01f);
		}

		// Collision
		if (ptr) {
			if (cgu::is_collid(*ptr, Bar)) {
				cgu::collision_process(*ptr, Bar);
			}
		}

		if (ptr) {
			for (auto& box : boxes) {
				if (cgu::is_collid(*ptr, box)) {
					cgu::collision_process(*ptr, box);
				}
			}
		}

		for (auto& box1 : boxes) {
			for (auto& box2 : boxes) {
				if (box1.get_id() != box2.get_id()) {
					if (cgu::is_collid(box1, box2)) {
						cgu::collision_process(box1, box2);
					}
				}
			}
		}

		// Drawing section
	__next:
		if (ptr) ptr->draw();
		Bar.draw();

		for (auto& box : boxes) box.draw();

		cgu::draw_rectangle({ 0,0,g_lx - 1, g_ly - 1});
		cgu::print_text_at(wsMenuTail, 1, g_ly - 1);
		cgu::console.display();



		if (cgu::get_out()) break;
		cgu::elps = timer.get_elapsed_time();
		cgu::fps = 1.f / cgu::elps;

	}


	std::cout << "End of Program..\n";
	return 0;
}