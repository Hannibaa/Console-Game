#include <iostream>
#include <queue>
#include <array>
#include "game_timer.h"
#include "shipes.h"
#include "sprite.h"
#include "gsprite.h"
#include "MyLib/container_utility.h"

/*
		   Box Collision detection
		   
		   Author : KADDA Aoues
		   Version: 1.0.0.0
		   Collision detection by creating vector of boxes with deferente size
		   Implement walking in ground
		   Floppy bird version 1.0.0
*/





int main() {

	// Console : -------------------------------------------------
	const int _length = 145;
	const int _width = 35;
	int _id{};

	cu::console.construct_console(_length, _width);

	// Object : --------------------------------------------------
	// create two object and one pointer that switch between objects
	cu::Sprite* ptr{};   // pointer to null object:
	std::vector<std::wstring> wstr;
	std::vector<cu::MSprite> vm_box;

	auto generate_box = [&](int l, int w, int px, int py) {
		cu::make_dim_boxes(wstr, l, w);
		vm_box.emplace_back(wstr, px, py);
		vm_box[vm_box.size() - 1].set_speed(0.01f, 0.01f);
		vm_box[vm_box.size() - 1].set_id(++_id);
		};

	cu::make_dim_boxes(wstr, 2, 9);
	cu::MSprite m_box(wstr, _length - 3, _width - 10);

	cu::MSprite stars({ L". . :", L" . . " }, 5, 4);

	cu::Sprite  sel_box({ L"\x2501\x253c\x2501" }, -1.f, -1.f);

	cu::GSprite  box({ L"\x250f\x2501\x2501\x2513", L"\x2503  \x2503", L"\x2517\x2501\x2501\x251b" });
	box.set_controle_key();
	box.set_gravity(2.f, 3.7f);

	// lambda function that create one instance randomly

	// Menu : ----------------------------------------------------
	std::wstring wstrMenu = LR"(P : Added one box
O : Remove selected box
S : Sellect Controle next Box
R : Remove any Controle
U : Speeding up box
D : Speeding down box
I : Pause Move
ESCAP : Exit 
)";

	std::wstring wstrMenu_ = LR"(P:added O:Remove 
S:Sellect R:Desellect U:Speeding D:Unspeeding I:Pause)";
	Container::replace_all_by(wstrMenu_, L'\n', L' ');

	// timer to calculate frame per seconds
	Game_Timer g_timer;
	bool b_pause = false;
	int collision_count{};

	// elapsed time variable, maximum value and minimul value
	float elapsed_time{ 1.f };
	// string wchar_t buffer for title
	wchar_t s[150]{};
	std::wstringstream ss;



	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Main Loop
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;

		int xx{};
		if (!vm_box.empty()) xx = vm_box[0].getY();

		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per sec[%5.2f]|Boxes[%d]", frames, xx
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


		// Release moving one moving boxes
		if (KeyReleased(_u('P'))) {
			rng::iRG<int> irand;
			generate_box(4, 4, irand(1,_length - 10), irand(1,_width - 5));
		}

		// Remove one box
		if (KeyReleased(_u('O'))) {
		}

		// Controle vector of boxes
		if (KeyReleased(_u('S')) && !ptr) {
			rng::fRG<float> frand;
			box.set_position(frand(3.f, 120.f), 15.f);
			box.set_speed(0.01f, 0.01f);
			ptr = &box;
		}

		// speed up
		if (KeyReleased(_u('U'))) {
			if (ptr) {
				ptr->set_speed(ptr->get_speed().x + 0.01f, ptr->get_speed().y + 0.01f);
			}
		}

		// speed down
		if (KeyReleased(_u('D'))) {
			if (ptr) {
				ptr->set_speed(ptr->get_speed().x - 0.003f, ptr->get_speed().y - 0.003f);
			}
		}

		// Remove one Particule
		if (KeyReleased(_u('R'))) {
			ptr = nullptr;
		}

		// move particule
		if (ptr) {
			ptr->move();
		}

		// move box
		m_box.move();

		stars.move();

		// move boxes
		for (auto& box : vm_box) box.pmove({ 1,1 }, { _length - 4, _width - 4 });


		// Collision
		if (ptr) {
			if (cu::is_collid(m_box, *ptr)) {
				ptr = nullptr;
			}			
		}

		if (ptr) {
			for (auto& box : vm_box)
				if (cu::is_collid(box, *ptr)) {
					ptr = nullptr;
					break;
				}
		}

		if (!vm_box.empty()) {
			for(auto& box1 : vm_box)
				for(auto& box2 : vm_box)
					if (box1.get_id() != box2.get_id()) {
						if (cu::is_collid(box1, box2)) {
							cu::collision_process(box1, box2, 5.f);
							++collision_count;
						}
					}
		}


		// 
		if (ptr) {
			ss << ptr->getX() << L" , " << ptr->getY();
			// printing some text at : 
			cu::print_text_at(ss.str(), _length - 20, 2);
			ss.str(L"");
		}


		///////////// drawing section priority  /.///////
		stars.draw();
		for (auto& box : vm_box) box.draw();
		m_box.draw();

		if (ptr) ptr->draw();
		sel_box.draw();
		// draw rectangle scene
		cu::draw_rectangle({ 0,0,_length - 1, _width - 1 });
		cu::print_text_at(wstrMenu_, 0, _width - 1);

	near_end:

		cu::console.display();
		elapsed_time = g_timer.get_elapsed_time();
		if (cu::get_out()) break;
	}

	return 0;
}
