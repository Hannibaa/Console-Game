#include <iostream>
#include <MyLib/random_generator.h>
#include "game_timer.h"
#include "shipes.h"
#include "sprite.h"

/*
				 Author : KADDA Aoues
                 Space Invader version 0.0.1
				 One Shipe vs invader particules.
				 1. Collision
				 2. Menu
*/

template<typename P1, typename P2>
bool Overlap(const P1& p1, const P2& p2) {
	auto pos1 = p1.get_position();
	auto pos2 = p2.get_position();

	return (pos2.x - 1 <= pos1.x && pos1.x <= pos2.x + 1) &&
		(pos2.y - 1 <= pos1.y && pos1.y <= pos2.y + 1);
}


int main() {
	const int _length = 145;
	const int _width = 35;

	cu::Console console(_length, _width);

	// vector of particules and shapes
	std::vector<CO::Particule> vParticules;
	wchar_t wShape[] = { L'\x2588', L'\x258c',L'\x2584', L'\x25a0'};
	int identity{};

	CO::Sprite  shipe(console, { L"*****",L"<|\x25b2|>", L"....." });
	shipe.set_controle_key();
	shipe.set_option_key(_u('Z'), _u('X'), VK_SPACE);
	shipe.set_shield(L"\x25ac\x25ac\x25ac\x25ac\x25ac", L"\x25ac\x25ac\x25ac\x25ac\x25ac");
	shipe.set_fire_parameter();
	shipe.create_fires();


	// timer to calculate frame per seconds
	Game_Timer g_timer;
	// random variable
	rng::fRG<float> frand;
	auto bool_func = [](int _size) { return rng::Random::rand() % _size; };

	// elapsed time variable, maximum value and minimul value
	float elapsed_time{ 1.f };
	float max_frames_{ 1.f };

	// string wchar_t buffer for title
	wchar_t s[150]{};
	bool b_pause = false;

	std::wstring strMenu = LR"(P : add one random particule
L : Add 100 random particule
I : Pause and show Menu
C : Delete all particule
ESCAP : Exit)";

	// Start loop for game
	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;
		if (frames > max_frames_) max_frames_ = frames;

		// title of console
		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per second : [%3.2f][%3.2f]| [X=%3.2f ; Y=%3.2f][particules %d]", frames
			, max_frames_, shipe.getX(), shipe.getY(), vParticules.size());
		TITLE(s);

		// Pause game.
		if (KeyReleased(_u('I'))) {
			b_pause = !b_pause;
		}

		if (b_pause) {
			cu::message_box(console, 50, 10, L"Menu", strMenu);
			goto near_end_;
		}

		// clear screen buffer;
		console.clear_screen();

		// Delete all particule
		if (KeyReleased(_u('C'))) {
			if (!vParticules.empty()) vParticules.clear();
		}

		// Create particule :
		if (KeyReleased(_u('P'))) {
			float xp = frand(0.f, 140.f);
			float yp = frand(0.f, 35.f);

			vParticules.emplace_back(console, xp, yp, wShape[bool_func(4)], ++identity );
		}

		// Make 1000 particules
		if (KeyReleased(_u('L'))) {
			for (int i = 0; i < 100; ++i) {
				float xp = frand(0.f, 140.f);
				float yp = frand(0.f, 35.f);
				vParticules.emplace_back(console, xp, yp, wShape[bool_func(4)], ++identity);
				vParticules[vParticules.size() - 1].set_speed(frand(0.01f, 0.05f), frand(0.01f, 0.05f));
			}
		}

		// draw the particule
		if (!vParticules.empty()) {
			for (auto& p : vParticules) {
				p.move({1,1}, {_length - 1, _width - 1});
			}
		}

		// draw rectangle scene
		cu::draw_rectangle(console, { 0,0,_length - 1, _width - 1 });


		shipe.move();
		shipe.firing_2();

		// Collision logic:
		if (!vParticules.empty()) {
			for (auto& p : vParticules) {
				/*if (Overlap(shipe, p)) {
					p.revers();
				}*/

				for (auto& q : vParticules) {
					if (p._id != q._id) {
						if (Overlap(p, q)) {
							p.set_speed(0.f, 0.f);
							q.set_speed(0.f, 0.f);
						}
					}
				}

			}
		}

		if (!vParticules.empty()) {
			for (int k = 0; k < vParticules.size(); ++k ) {
				if (vParticules[k]._is_static) {
					vParticules.erase(vParticules.begin() + k);
					 continue;
				}
				
			}
		}


		near_end_:
		console.draw();
		elapsed_time = g_timer.get_elapsed_time();
		if (cu::get_out()) break;
	}

	return 0;
}
