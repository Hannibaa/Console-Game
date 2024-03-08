#include "../include/sprite_v_3.h"
#include "MyLib/chrono/Game_timer.h"
#include "MyLib/random_generator.h"




int main() {
	int g_lx = 145;
	int g_ly = 35;

	cgu::console.construct_console(g_lx, g_ly);


	cgu::ISprite* ptr{};
	using TSprite = cgu::StaticSprite<4, 4>;

	Str::replace_all_by(cgu::wstrTetrisPiece[0], L'.', L' ');
	TSprite                       stext{ cgu::wstrTetrisPiece[0], 30, 13};
	cgu::DynSprite                dtext{ L"hello is there any think like cpp here",10,3,50,15 };

	cgu::Motion                   move;
	cgu::Collision                collision;

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
	Time::Game_Timer   timer;
	bool     bPause{ false };
	wchar_t  title[255]{};

	while (true) {
		// title
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

		if (KeyReleased(_u('S'))) {
			if (!ptr)
			 ptr = (&stext);
			else {
				rng::iRG<int>   irand;
				int i = irand(0, 6);
				Str::replace_all_by(cgu::wstrTetrisPiece[i], L'.', L' ');
				dynamic_cast<TSprite*>(ptr)->set_buffer(cgu::wstrTetrisPiece[i]);
			}

		}

		if (KeyReleased(_u('D'))) {
			//ptr = dynamic_cast<cgu::DynSprite*>(&dtext) ;
			ptr = nullptr;
		}

		// Cinamatics

		if (ptr) move(dynamic_cast<TSprite*>(ptr), 1.78f * 60.f / cgu::fps );

		cgu::draw_rectangle({ 0,0, g_lx - 1, g_ly - 1 });

		// drawing function
		if (ptr) {
			ptr->draw();
		}

	__next:
		cgu::console.display();

		if (cgu::get_out()) {
			break;
		}

		cgu::fps = 1.f / timer.get_elapsed_time();
	}




}