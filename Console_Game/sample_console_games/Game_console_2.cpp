#include <iostream>
#include <queue>
#include <array>
#include "game_timer.h"
#include "shipes.h"
#include "sprite.h"

/*
           Particule running:
		   Author : KADDA Aoues
		   Version: 1.0.0.0
		   elastic collision
*/



int main() {
	const int _length = 120;
	const int _width  = 35;

	cu::console.construct_console(_length, _width);

	// Menu : 
	std::wstring wstrMenu = LR"(P : Create one random particule
R : Remove one particule
I : Pause and Show The Menu
ESCAP : Exit 
)";

	// particule parameter
	int    number_particules = 1000;
	size_t p_capacity = 110;

	std::array<wchar_t, 4> wShape{ L'\x2588', L'\x258c',L'\x2584', L'\x25a0' };

	std::vector<cu::Particule> vp;
	vp.reserve(p_capacity);

	// lambda that generate vector of 
	auto gen_particule = [&]() {
		rng::fRG<float> frand;
		rng::iRG<int>   irand;
		float xp = frand(1.f, float(_length - 1));
		float yp = frand(1.f, float(_width - 1)); 
		vp.emplace_back(xp, yp, wShape[irand(0, 3)], vp.size());
		};

	
	// timer to calculate frame per seconds
	Game_Timer g_timer;
	bool b_pause = false;

	// elapsed time variable, maximum value and minimul value
	float elapsed_time{ 1.f };
	float max_frames_{1.f};
	int count{};
	// string wchar_t buffer for title
	wchar_t s[150]{};
	
	// Start loop for game
	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;
		if (frames > max_frames_) max_frames_ = frames;

		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per second : [%3.2f][%3.2f]| [Np=%d; Clsion=%d] ", frames
		, max_frames_, vp.size(), count);
		TITLE(s);

		// Show the Menu
		if (KeyReleased(_u('I'))) {
			b_pause = !b_pause;
		}

		if (b_pause) {
			//cu::message_box(30, 10, L"Menu", wstrMenu);
			goto near_end;
		}




		cu::console.clear_screen();


		// Generate One particules
		if (KeyReleased(_u('P'))) {
			if (vp.size() < number_particules) {
				gen_particule();
			}
		}

		// Remove one Particule
		if (KeyReleased(_u('R'))) {
			if (!vp.empty()) vp.pop_back();
		}

		// draw particule
		for (auto& p : vp) {
			p.move();
			for (auto& q : vp) {
				if (p._id != q._id) {
					if (cu::IsPointInRect<int>((int)p.getX(), (int)p.getY(),
						cu::iRect(q.getX() - 1, q.getY() - 1, 2, 2))) {
						b_pause = true;
					}
				}
			}
		}

		// Collision Removal
		


		// draw rectangle scene
		cu::draw_rectangle( { 0,0,_length - 1, _width - 1 });

near_end:

		cu::console.draw();
		elapsed_time = g_timer.get_elapsed_time();
		if (cu::get_out()) break;
	}

	return 0;
}
