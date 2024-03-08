#include <iostream>
#include <queue>
#include <array>
#include <variant>
#include "Console_utility_1.hpp"
#include "mylib/chrono/Game_timer.h"
#include "MyLib/Console_Library/escape_code.h"
#include "sprite_v_2.h"

/*
		   New designe Technique v2... 
		   Implementing visitor pattern in sprite_v_2.h
		   Author : KADDA Aoues
		   Version: 1.0.0.0
		   Console Font Size Functions test.
*/




int main() {
	// Console : -------------------------------------------------
	const int      _length = 145;
	const int      _width = 35;

	const int      _glength = 100;
	const int      _gwidth = 34;

	uint32_t       _uid{};

	const cgu::iRect     _miniRect{ 0,0,_glength, _gwidth };

	cgu::console.construct_console(_length, _width);

	COORD  fontSize{ 7,7 };
	cgu::Sprite* ptr{};
	cgu::Dt60 = 2.25f;
	// Implement methode ========================================================================

	class Move : public cgu::ISpriteMotionVisitor {
	public:
		using cgu::ISpriteMotionVisitor::ISpriteMotionVisitor;

		virtual void visit(cgu::Sprite* s, float f) override {
			Dtf = cgu::Dt60 * 60.f / f;
			auto pdim = s->get_dimension();
			auto p = s->get_position();

			if (KeyPressed(VK_UP)) {
				p.y -= _v.y * Dtf;
			}
			if (KeyPressed(VK_DOWN)) {
				p.y += _v.y * Dtf;
			}
			if (KeyPressed(VK_RIGHT)) {
				p.x += _v.x * Dtf;
			}
			if (KeyPressed(VK_LEFT)) {
				p.x -= _v.x * Dtf;
			}

			p.clamped({ _zone.x , _zone.x + _zone.dx - pdim.x - 1.f },
				{ _zone.y , _zone.y + _zone.dy - pdim.y - 1.f });

			s->set_position(p);
		}
	};

	// Sprite : ===================================================
	cgu::Dt60 = 2.25f;

	cgu::wstrTexture texture;
	Str::make_dim_boxes(texture, 3, 3);

	// principale shipe
	cgu::Sprite  shipe{ L"|*:*|" , 20.f, 20.f };
	shipe.set_id(++_uid);

	std::vector<cgu::Sprite> vShipes;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// lambda function generate boxes and shipes ---------------------////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	auto generate_shipe = [&](const std::wstring& imageStr) {
		rng::fRG<float> frand;
		vShipes.emplace_back(imageStr);
		float x_limit = float(_glength) - 1.f;
		float y_limit = float(_gwidth) - 1.f;
		int current_index = vShipes.size() - 1;
		vShipes[current_index].set_position(frand(1.f, x_limit), frand(1.f, y_limit));
		vShipes[current_index].set_id(++_uid);
		};

	auto generate_boxe = [&]() {
		rng::fRG<float> frand;
		vShipes.emplace_back(texture, frand(1.f, _glength - 1.f), frand(1.f, _gwidth - 1.f));
		vShipes[vShipes.size() - 1].set_id(++_uid);
		};


	// Menu : -----------------------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------
	std::wstring wstrMenu = LR"(P : Added one box
O : Remove all shipes
S : Add Shipe
R : Select shipe
U : Speeding up box
D : Speeding down box
I : Pause Move
ESCAP : Exit 
)";

	// let

	std::wstring wstrMenu_ = LR"(P:added O:Remove 
S:Sellect R:Desellect U:Speeding D:Unspeeding I:Pause)";
	Str::replace_all_by(wstrMenu_, L'\n', L' ');

	// timer to calculate frame per seconds
	Time::Game_Timer g_timer;
	bool b_pause = false;

	// elapsed time variable, maximum value and minimul value
	float elapsed_time{ 1.f };
	// string wchar_t buffer for title
	wchar_t s[255]{};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Main Loop
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	float x{ 10.f }, y{ 10.f };
	Move move({ 1.f,  1.f }, { _glength - 1.f, _gwidth - 1.f}, {0.1f,0.1f});
	

	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;

		swprintf_s(s, 255,
			L"Console ReSizing  %d][%d|[%5.2f]|[%3.2f,%3.2f][%d]"
			, fontSize.X, fontSize.Y, frames, 0.f, 0.f
			, vShipes.size()
		);
		// title
		TITLE(s);

		// Show the Menu
		if (KeyReleased(_u('I'))) {
			b_pause = !b_pause;
		}

		if (b_pause) {
			cgu::message_box(_length / 2, _width / 2, L"Menu", wstrMenu);
			goto near_end;
		}

		cgu::console.clear_screen();

		// generate shipes
		if (KeyReleased(_u('S'))) {
			rng::iRG<int> irand;
			std::wstring wstr[4] = { L"{\x2588}", L"<\x25c4\x25ba>",L"\x2590::\x258c", L"|<\x25a0>|" };
			generate_shipe(wstr[irand(0, 3)]);
		}

		// generate boxes 
		if (KeyReleased(_u('P'))) {
			generate_boxe();
		}

		// remove all shipes
		if (KeyReleased(_u('O'))) {
			vShipes.clear();
		}

		// select 
		if (KeyReleased(_u('R'))) {
			ptr = &shipe;
		}

		if (KeyReleased(_u('U'))) {
			if (ptr) {
				move.set_speed(move.get_speed().x + 0.001f, move.get_speed().y + 0.001f);
			}
		}

		if (KeyReleased(_u('D'))) {
			if (ptr) {
				move.set_speed(move.get_speed().x - 0.001f, move.get_speed().y - 0.001f);
			}
		}


		// start moving

		//fontSize = cgu::console_control_size(cgu::console);
		
		if (ptr) ptr->accept(move, frames);
		//for (auto& s : vShipes) s.accept(move,frames);

		// collision treatement : ================================================================
		if (!vShipes.empty()) {
			for (auto& s : vShipes) {
				for (auto& q : vShipes) {
					if (s.get_id() != q.get_id()) {
						if (cgu::is_collid(s, q)) {
							cgu::collision_process(s, q, 5.f);
						}
					}
				}
			}

			for (auto it = vShipes.begin(); it != vShipes.end(); ++it) {
				if (!_miniRect.contain(it->get_position().x, it->get_position().y)) {
					it = vShipes.erase(it);
				}
			}
		}

		///////////// drawing section priority  /.///////

		for (auto& s : vShipes) s.draw();
		if (ptr) ptr->draw();

		// draw rectangle scene
		cgu::draw_rectangle({ 0, 0 ,_length - 1, _width - 1 });
		cgu::draw_rectangle({ 0,0, _glength - 1, _gwidth - 1 });
		cgu::print_text_at(wstrMenu_, 0, _width - 1);

	near_end:

		cgu::console.display();
		elapsed_time = g_timer.get_elapsed_time();
		if (cgu::get_out()) break;
	}

	return 0;
}










