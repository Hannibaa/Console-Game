#include <iostream>
#include <queue>
#include <array>
#include "game_timer.h"
#include "shipes.h"
#include "sprite.h"
#include "gsprite.h"
#include "MyLib/container_utility.h"

/*
		   Ping Pong Old 

		   Author : KADDA Aoues
		   Version: 1.0.0.0		   
		   
		   todo :
		   1. playing Zone and screen bigger 
		   2. show data in screen 

*/





int main() {

	// Console : -------------------------------------------------
	const int _length = 45;
	const int _width = 35;

	cu::console.construct_console(_length, _width);

	// Object : --------------------------------------------------
	// create two object and one pointer that switch between objects
	cu::Sprite*     ptr{};   // pointer to null object:
	std::wstring    plate(6, L'\x2588');

	cu::Particule   ball(_length/2.f, _width/2.f, L'\x2588');
	ball.set_speed(0.005f, 0.005f);
	cu::Sprite      player1({ plate }, 30.f, 2.f);
	cu::Sprite      player2({ plate }, 50.f, float(_width) - 3.f);

	// controle
	player1.set_controle_key(0,0);
	player2.set_controle_key(0, 0, 'Z', 'X');

	int score1{};
	int score2{};


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
			L"[%5.2f]|1[%d]---2[%d]", frames, score1,score2
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
		}

		// Remove one box
		if (KeyReleased(_u('O'))) {
			score1 = 0;
			score2 = 0;
		}

		// Controle vector of boxes
		if (KeyReleased(_u('S')) && !ptr) {
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
			ball.set_position(_length / 2.f, _width / 2.f);
		}

		// move particule
		if (ptr) {
			ptr->move();
		}

		ball.move();
		player1.move();
		player2.move();


		// Collision
		if (cu::is_collid(player1, ball)) {
			ball.revers2();
		}

		if (cu::is_collid(player2, ball)) {
			ball.revers2();
		}

		/// scoring 
		if (ball.getY() < 1.f) {
			++score1;
		}

		if (ball.getY() > _width - 1.f) {
			++score2;
		}

		///////////// drawing section priority  /.///////

		if (ptr) ptr->draw();
		ball.draw();
		player1.draw();
		player2.draw();

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
