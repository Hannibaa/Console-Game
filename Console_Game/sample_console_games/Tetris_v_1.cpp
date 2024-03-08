#include "shape_v_1.hpp"
#include "Tetris_v_1.h"
#include "MyLib/random_generator.h"
#include "MyLib/chrono/Game_timer.h"
#include <variant>



int main() {

	const int G_length = 145;
	const int G_height = 35;

	const int G_MidleX = 145 / 2;
	const int G_MidleY = 35 / 2;

	const int L_length = 60;
	const int L_height = 34;

	bool      G_pause{ false };
	int       G_id{};

	cgu::console.construct_console(G_length, G_height);
	cgu::console.ajust_frames_per_sec(1.3f);

	// tetris 
	cgu::ISprite*          ptr{};

	cgu::Box<10, 10>     v1box;
	cgu::LineBox<10, 10>  lbox;
	using Box = std::variant<cgu::LineBox<3, 3>, cgu::DynLineBox>;
	std::vector<Box> vl1box;

	auto generate_box = [&]<typename Tbox>(Tbox box){
		rng::iRG<int>          irand;
		Tbox                   lbox(irand(1, G_length - 2), irand(1, G_height - 2));

		lbox.set_id(++G_id);
		vl1box.push_back(lbox);
	};

	tetris::Motion   move;
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Menu set up
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::wstring wsMenuHead = LR"(Menu)";
	std::wstring wsMenuBody = LR"(P: Pause and show menu
U: increase speed
D: decrease speed
ESC: Exit game)";

	std::wstring wsMenuTail = L"P:Pause Esc:Exit U:Incr D:decr ";


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Main Loop
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	Time::Game_Timer             Timer;
	wchar_t                      title[255];
	int                          rot{2};

	while (true) {
		Timer.reset();
		swprintf_s(title, 255, L"Frame/sec [%5.2f] "
			L"Pos[%3.2f,%3.2f]",
			0.f, 0.f, cgu::fps
		);
		TITLE(title);

		cgu::console.clear_screen();


		// Pause and Show menu
		if (KeyReleased(_u('P'))) {
			G_pause = !G_pause;
			++rot;
		}

		if (G_pause) {

			cgu::message_box(G_MidleX, G_MidleY, wsMenuHead, wsMenuBody);
			goto __pause;
		}

		if (KeyReleased(_u('N'))) {
			// change a set of tetromino
			v1box.set_glyph(L'\x25c3');
		}

		if (KeyReleased(_u('G'))) {
			generate_box(cgu::DynLineBox{});
		}

		if (KeyReleased(_u('D'))) {
			generate_box(cgu::LineBox<3, 3>{});
		}

		if (KeyReleased(_u('S'))) {
			ptr = &v1box;
		}
		if (KeyReleased(_u('A'))) {
			ptr = &lbox;
		}
		


		// action
		if (ptr)
		   move(ptr, 1.7f * 60.f / cgu::fps);

		// Drawing

		v1box.draw();
		lbox.draw();
		for (auto& box : vl1box) std::visit([](auto arg) {arg.draw(); }, box);
	__pause:
		cgu::draw_rectangle({ 0 , 0 , G_length -1 , G_height -1 });
		cgu::print_text_at(wsMenuTail, 1, G_height - 1);
		cgu::console.display();

		if (cgu::get_out()) break;
		cgu::fps = 1.f / Timer.get_elapsed_time();
	}






	std::cout << "End Game...\n";
	std::cin.get();
	return 0;
}



