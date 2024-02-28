#include <iostream>
#include <MyLib/random_generator.h>
#include "Console_utility_1.hpp"
#include "game_timer.h"

#include "sprite.h"
#include "shipes.h"

/*
				 Author : KADDA Aoues
                 Space Invader version 0.0.1
				 One Shipe vs invader particules.
				 New Design of Console for 
				 1. Collision
				 2. Menu
				 3. Game strategy ?
*/



int main() {

	// 1. Console parameter -- length -- , -- width -- ;
	int _length{ 150 };
	int _width{ 20 };

	cu::console.construct_console(_length, _width);

	// 2. using Sprite to construct object

	int identity{};

	// use this pointer to set and reset it:
	std::unique_ptr<cu::Sprite>  p1Shipe;
	std::unique_ptr<cu::Sprite>  p2Shipe;

	// First Shipe
	cu::Sprite  shipe({ L"\x2584\x2584\x2584\x2584", L"<\x25ba>", L"\x2580\x2580\x2580\x2580" });
	shipe.set_controle_key();
	shipe.set_fire_parameter(0.011f,0.f, L'\x25cf');
	shipe.create_fires();

	// Seconde shipe
	cu::Sprite  shipe2({ L"\x2580\x2580\x2580\x2580" , L"<\x25c4>", L"\x2584\x2584\x2584\x2584" });
	shipe2.set_controle_key('W','S','A','D');
	shipe2.set_fire_parameter(-0.011f,0.f, L'\x25cf');
	shipe2.create_fires();
	shipe2.set_option_key(0, 0, 'R');
	
	p1Shipe.reset(&shipe);
	p2Shipe.reset(&shipe2);

	// 3. including particule that have perpetual moving
	std::vector<cu::Particule>  vPrt;
	vPrt.emplace_back(100, 10, L'\x25a0', 0);
	vPrt[0].set_speed(0.0055f, 0.0055f);


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

	int count1{};
	int count2{};

	std::wstring strMenu = LR"(P : add one random particule
I : Pause and show Menu
A : Move left D : Move right
S : Move down W : Move up
R : Firing
N : Recreate shipe
ESCAP : Exit)";

	// Start loop for game
	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;
		if (frames > max_frames_) max_frames_ = frames;

		// title of console
		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per second : [%3.2f][%3.2f]| [X=%d ; Y=%d][First %d][Second %d]", frames
			, max_frames_, shipe.get_bounds().left,shipe.get_bounds().top,  count1, count2);
		TITLE(s);

		// Pause game.
		if (KeyReleased(_u('I'))) {
			b_pause = !b_pause;
		}

		if (b_pause) {
			cu::message_box(50, 10, L"Menu", strMenu);
			goto near_end_;
		}

		// clear screen buffer;
		cu::console.clear_screen();


		// Create particule :
		if (KeyReleased(_u('P')) || vPrt.empty()) {
			float xp = frand(1.f, float(_length - 1));
			float yp = frand(1.f, float(_width - 1));
			if (vPrt.empty()) {
				vPrt.emplace_back(xp, yp, L'\x2588', 0);
				vPrt[0].set_speed(0.0055f, 0.0055f);
			}
		}

		// Make 1000 particules
		if (KeyReleased(_u('L'))) {
			for (int i = 0; i < 100; ++i) {
				float xp = frand(0.f, 140.f);
				float yp = frand(0.f, 35.f);	
			}
		}

		if (KeyReleased(_u('N'))) {
			if (p1Shipe == nullptr) {
				p1Shipe.reset(&shipe);
			}

			if (p2Shipe == nullptr) {
				p2Shipe.reset(&shipe2);
			}
		}

		// draw the particule
		if (!vPrt.empty()) {
			for (auto& p : vPrt) {
				p.move();
			}
		}

		// draw rectangle scene
		cu::draw_rectangle( { 0,0,_length - 1, _width - 1 });

		if (p1Shipe) {
			p1Shipe->move();
			p1Shipe->firing();

			// Collision of fire with particules
			for (auto& f : p1Shipe->get_vector_firing_coord()) 
				if (f.is_ready()) {
					if (!vPrt.empty()) {
						if (vPrt[0].get_bounds().contain(f.get_position().x, f.get_position().y)) {
							vPrt.clear();
							++count1;
						}
					}
				}
		}

		if (p2Shipe) {
			p2Shipe->move();
			p2Shipe->firing();

			// Collision of fire with particules
			for (auto& f : p2Shipe->get_vector_firing_coord())
				if (f.is_ready()) {
					if (!vPrt.empty()) {
						if (vPrt[0].get_bounds().contain(f.get_position().x, f.get_position().y)) {
							vPrt.clear();
							++count2;
						}
					}
				}
		}


		// Collision logic for shipe 1:

		if (!vPrt.empty()) {
			if (p1Shipe) {
				if (p1Shipe->get_bounds().contain((int)vPrt[0].getX(), (int)vPrt[0].getY())) {
					vPrt.clear();
					p1Shipe.release();
					p1Shipe.reset(nullptr);
					//count1 = 0;
				}
			}

            // Collision logic for shipe 2 :
			if (p2Shipe) {
				if (p2Shipe->get_bounds().contain((int)vPrt[0].getX(), (int)vPrt[0].getY())) {
					vPrt.clear();
					p2Shipe.release();
					p2Shipe.reset(nullptr);
					//count2 = 0;
				}
			}

		}

		



		near_end_:
		cu::console.draw();
		elapsed_time = g_timer.get_elapsed_time();
		if (cu::get_out()) break;
	}

	p1Shipe.release();
	p2Shipe.release();

	return 0;
}
