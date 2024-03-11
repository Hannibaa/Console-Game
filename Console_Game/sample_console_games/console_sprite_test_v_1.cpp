#include "MyLib/chrono/Game_timer.h"
#include "MyLib/random_generator.h"
#include "../include/animation_sprite.hpp"
#include "../include/Cinematic.hpp"



int main_animation() {

	int g_lx = 145;
	int g_ly = 35;

	cgu::console.construct_console(g_lx, g_ly);


	// declare object
	cgu::AnimatedSprite      anime{ 50,13 };
	cgu::cin::Motion         move;

	move.set_speed(0.6f, 0.7f);
	move.set_bounds(1, 1, g_lx, g_ly - 5);
	move.set_auxillary_key({ _u('U'), _u('D'), 0 });

	anime.add_image(LR"( /**"________|*****-------(*****))","roling", 13, 3);
	anime.add_image(LR"( /**"________|*****-------(**  *))","roling", 13, 3);
	anime.add_image(LR"( /**"________|*****-------(*  **))","roling", 13, 3);
	anime.add_image(LR"( /**"________|*****-------(  ***))","roling", 13, 3);
	anime.set_speed(0.08f);

	anime.add_image(LR"( /**"_______ |*****------ (*****))", "fire", 13, 3);
	anime.add_image(LR"( /**"______  |*****------ (*****))", "fire", 13, 3);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Menu ..
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::wstring strMenuTitle{ L"Menu" };
	std::wstring strMenuBody = LR"(M: Pause and Show Menu
)";



	//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//  Main Loop  
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer     timer;
	bool                 bPause{ false };
	wchar_t              title[255]{};
	std::string          strAction;

	cgu::Dt60 = 1.4f;

	while (true) {
		// title
		cgu::Dtime = cgu::Dt60 * 60.f / cgu::fps;
		timer.reset();
		swprintf_s(title, 255, L"Console fps[%5.2f]", cgu::fps);
		TITLE(title);

		// clear screen
		cgu::console.clear_screen();

		// Menu methode
		if (KeyReleased(_u('M'))) {
			bPause = !bPause;
		}

		if (bPause) {
			cgu::message_box(g_lx / 2, g_ly / 2, strMenuTitle, strMenuBody);
			goto __next;
		}


		// firing
		if ( KeyPressed(_u('F'))) {
			strAction = "fire";
		}
		else {
			strAction = "roling";
		}
		// cinematic : 
		move(&anime,  cgu::Dtime  );

		// drawing
		anime.animate(strAction, cgu::Dtime);
		anime.draw(strAction);


	__next:
		cgu::draw_rectangle({ 0,0, g_lx - 1, g_ly - 1 });

		cgu::console.display();

		if (cgu::get_out()) {
			break;
		}

		cgu::elps = timer.get_elapsed_time();
		cgu::fps = 1.f / cgu::elps;
	}



	return 0;
}


int main() {
	
	main_animation();

	return 0;
}