#include <iostream>
#include <queue>
#include <array>
#include "game_timer.h"
#include "shipes.h"
#include "sprite.h"
#include "MyLib/container_utility.h"

/*
		   Box Collision detection
		   Author : KADDA Aoues
		   Version: 1.0.0.0
		   Collision detection by creating vector of boxes with deferente size
*/





int main() {

	// Console : -------------------------------------------------
	const int _length = 145;
	const int _width = 35;

	cu::console.construct_console(_length, _width);

	// Object : --------------------------------------------------
	// create two object and one pointer that switch between objects
	cu::Sprite* ptr{};   // pointer to null object:
	std::vector<std::wstring> wstr;



	cu::Sprite  sel_box({  L"\x2501\x253c\x2501"}, -1.f, -1.f);

	// make vector of boxes;
	std::vector<cu::Sprite> vboxes;

	// lambda function that create one instance randomly
	uint32_t num_id{};
	auto generate_one = [&]() {
		rng::fRG<float> frand;
		rng::iRG<int>   irand;

		cu::make_dim_boxes(wstr, irand(3, 7), irand(3, 6));

		vboxes.emplace_back(wstr, frand(10.f, _length), frand(10.f, _width));
		vboxes[vboxes.size() - 1].set_id(++num_id);
		vboxes[vboxes.size() - 1].set_controle_key();

		wstr.clear();
		};

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

		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per sec[%5.2f]|Boxes[%d]", frames, vboxes.size()
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
			ptr = nullptr;
			generate_one();
		}

		// Remove one box
		if (KeyReleased(_u('O'))) {
			
			if (!vboxes.empty()) {
				if (ptr) {
					size_t pos = cu::get_position_vector(vboxes, ptr->get_id());
					if (pos != size_t(-1)) {
						vboxes.erase(vboxes.begin() + pos);
						ptr = nullptr;
					}
				}
				else
					vboxes.pop_back(); 
			}
		}

		// Controle vector of boxes
		if (KeyReleased(_u('S'))) {
			static int i{};
			if (!vboxes.empty()) {
				ptr = &vboxes[i % (int(vboxes.size()))];
				++i;
				sel_box.set_position(ptr->getX()-1.f, ptr->getY()-1.f);
			}
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

		// draw particule
		if (ptr) {
			ptr->move();
		} 

		// Collision
		for (auto& box : vboxes) {
			for (auto& box2 : vboxes) {
				if (box.get_id() != box2.get_id()) {
					if (cu::is_collid(box, box2)) {
						cu::collision_process(box, box2, 10.f);
					}
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

		for (auto& box : vboxes) box.draw();
		sel_box.draw();
		// draw rectangle scene
		cu::draw_rectangle({ 0,0,_length - 1, _width - 1 });
		cu::print_text_at(wstrMenu_, 0, _width - 1);

	near_end:

		cu::console.draw();
		elapsed_time = g_timer.get_elapsed_time();
		if (cu::get_out()) break;
	}

	return 0;
}
