#include <iostream>
#include <queue>
#include <array>
#include "mylib/chrono/Game_timer.h"
#include "shipes.h"
#include "sprite.h"
#include "MyLib/container_utility.h"

/*
		   Box Collision detection
		   Author : KADDA Aoues
		   Version: 1.0.0.0
		   Collision detection by creating vector of boxes with deferente size
		   added Selection + to move any boxes
		   color the boxe
		   destroy any boxe

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



	cu::Sprite  sel_box({ L"\x2501\x253c\x2501" }, -1.f, -1.f);

	// make vector of boxes;
	std::vector<cu::Sprite> vboxes;

	// make select shape handle
	cu::Sprite   handle({L"\x2501\x253c\x2501"}, 20.f, 20.f);
	handle.set_controle_key();

	// lambda function that create one instance randomly
	uint32_t num_id{};
	auto generate_one = [&](float dimx, float dimy) {
		rng::fRG<float> frand;
		rng::iRG<int>   irand;

		cu::make_dim_boxes(wstr, int(dimx), int(dimy));

		vboxes.emplace_back(wstr, frand(1.f, _length), frand(1.f, _width));
		vboxes[vboxes.size() - 1].set_id(++num_id);
		vboxes[vboxes.size() - 1].set_controle_key();

		wstr.clear();
		};

	// Menu : ----------------------------------------------------
	std::wstring wstrMenu = LR"(P : Added 400 particule
O : Remove one box
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
	Time::Game_Timer g_timer;
	bool b_pause = false;

	// elapsed time variable, maximum value and minimul value
	float elapsed_time{ 1.f };
	// string wchar_t buffer for title
	wchar_t s[150]{};
	std::wstringstream ss;
	int select_id{};

	float dimx{ 3.f }, dimy{ 3.f };
	int num_particules{ 100 };

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Main Loop
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;

		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per sec[%5.2f]|Boxes[%d]|id[%d]", frames, vboxes.size()
			,select_id
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


		// Generate 400 particule box
		if (KeyReleased(_u('P'))) {
			if (vboxes.size() < num_particules) {
				for (int i = 0; i < num_particules; ++i)
					generate_one(dimx, dimy);
			}
		}

		// Remove one box
		if (KeyReleased(_u('O'))) {
			if (!vboxes.empty()) {
				vboxes.pop_back();
			}
		}

		// create moving particules of boxes
		if (KeyReleased(_u('S'))) {
			ptr = &handle;
			select_id = 0;
		}

		// create
		if (KeyPressed(VK_CONTROL) && KeyReleased(_u('A'))) {
			if (ptr) {
				for (auto& box : vboxes) {
					if (cu::is_collid(*ptr, box)) {
						ptr = &box;
						select_id = box.get_id();
						break;
					}
				}
			}
		}

		// delete box
		if (KeyReleased(VK_DELETE)) {
			if (ptr) {
				if (!vboxes.empty()) {
					for (auto it = vboxes.begin(); it != vboxes.end(); ++it) {
						if (it->get_id() == ptr->get_id())
						{
							ptr = &handle;
							it = vboxes.erase(it);
							select_id = 0;
							break;
						}
					}
				}
			}
		}

		// speed up
		if (KeyReleased(_u('U'))) {
			if (ptr) {
				ptr->set_speed(ptr->get_speed().x + 0.005f, ptr->get_speed().y + 0.005f);
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

		// Collision
		for (auto& box : vboxes) {
			for (auto& box2 : vboxes) {
				if (box.get_id() != box2.get_id()) {
					if (cu::is_collid(box, box2)) {
						cu::collision_process(box, box2, 3.f);
					}
				}
			}
		}

		// Get point
		if (ptr) {
			ss << L"[" <<  ptr->getX() << L" , " << ptr->getY() << "]";
			ss << L"[" << ptr->get_speed().x << "|" << ptr->get_speed().y << L"]";
			// printing some text at : 
			cu::print_text_at(ss.str(), _length - 30, 2);
			ss.str(L"");
		}
		

		///////////// drawing section priority  /.///////

		for (auto& box : vboxes) box.draw();
		sel_box.draw();
		handle.draw();
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
