#include <iostream>
#include <queue>
#include "game_timer.h"
#include "shipes.h"
#include "sprite.h"

/*
  TODO : firing direction
         costom space invader
		 consom collision concept with

*/



int main() {
	const int _length = 145;
	const int _width  = 35;

	std::wstring name1{};
	std::wstring name2{};

	std::cout << "get first name  : ";
	std::wcin >> name1;
	std::cout << "get second name : ";
	std::wcin >> name2;

	cu::Console console(_length, _width);

	// particule elements
	CO::Particule p(console, 12, 15, L'\x2588');
	CO::Particule p2(console, 10, 20, L'\x258c');

	CO::Sprite  sprite(console, {L"*****",L"<|\x25b2|>", L"....."});
	sprite.set_controle_key();
	sprite.set_option_key(_u('Z'), _u('X'), VK_SPACE);
	sprite.set_shield(L"\x25ac\x25ac\x25ac\x25ac\x25ac", L"\x25ac\x25ac\x25ac\x25ac\x25ac");
	sprite.set_fire_parameter();
	
	CO::Sprite ship1(console, { L"^^", L"AA", L"++" });
	ship1.set_controle_key(_u('W'), _u('S'), _u('A'), _u('D'));
	ship1.set_option_key(_u('Q'), _u('E'), _u('T'));
	ship1.set_fire_parameter(0.f, 0.01f);

	// timer to calculate frame per seconds
	Game_Timer g_timer;

	// elapsed time variable, maximum value and minimul value
	float elapsed_time{ 1.f };
	float max_frames_{1.f};
	float min_frames_{10000.f};

	// string wchar_t buffer for title
	wchar_t s[150]{};
	
	int collision_count{};

	int count1{};
	int count2{};

	// Start loop for game
	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;
		if (frames > max_frames_) max_frames_ = frames;
		if (frames < min_frames_) min_frames_ = frames;
		swprintf_s(s, 150,
			L"Game Console v1.0 | Frames per second : [%3.2f][%3.2f][%3.2f]| [X=%3.2f ; Y=%3.2f][%s[1]%6d|%s[2]%6d] ", frames
		, min_frames_, max_frames_, p.getX(), p.getY(), name1.c_str(), count1, name2.c_str(), count2);
		TITLE(s);
		console.clear_screen();





		// draw rectangle scene
		cu::draw_rectangle(console, { 0,0,_length - 1, _width - 1 });

		// particule
		p.move({10,10}, {30, 20});
		p2.move();


		sprite.move();
		sprite.firing();

		ship1.move();
		ship1.firing();

		if (KeyReleased(_u('O'))) {
			collision_count = 0;
			count1 = 0;
			count2 = 0;
		}


		//if (geom2d::IsCloser(ship1.get_position(), sprite.get_position(), 2.f)) {
		//	++collision_count;
		//}

		if (geom2d::Collision(ship1.get_vector_firing_coord(), sprite, 2.f)) {
			++count1;
		}

		if (geom2d::Collision(sprite.get_vector_firing_coord(), ship1, 2.f)) {
			++count2;
		}



		console.draw();
		elapsed_time = g_timer.get_elapsed_time();
		if (cu::get_out()) break;
	}

	return 0;
}
