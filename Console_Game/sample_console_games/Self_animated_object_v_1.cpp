#include <iostream>
#include "mylib/chrono/Game_timer.h"
#include "self_animated.hpp"

/*
            Author : Kadda Aoues
			Object : Self animated object
			Date   : 6 / 03 / 2024

*/


class Motion : public cgu::ISpriteVisitor {
public:
	bool         _is_moving{false};

	virtual void visit(cgu::AnimatedSprite* anime, float f) override {
		auto p = anime->get_position();

		short key_left  = KeyPressed(VK_LEFT );
		short key_right = KeyPressed(VK_RIGHT);

		
		if (key_left) {
			p.x -= 1.f * 60.f / f;
			_is_moving = true;
		}
		else _is_moving = false;

		if (key_right) {
			p.x += 1.f * 60.f / f;
			_is_moving = true;
		}
        
		if (KeyPressed(VK_UP)) {
			p.y -= 1.f * 60.f / f;
		}

		p.y += 0.5f * 60.f / f;
		p.y = std::clamp(p.y , 1.f, cgu::console.get_width() - anime->get_dimension().y - 1.f);

		anime->set_position(p);
	}
};


int main() {

	// Console : -------------------------------------------------
	const int _length = 145;
	const int _width = 35;

	cgu::console.construct_console(_length, _width);


	// Object : ---------------------------------------------------
	/*
                |::|    |::|    |::| 
				\::\    /::/    |::|
	*/
	auto s1 = LR"(|::|
\::\)";
	auto s2 = LR"(|::|
/::/)";
	auto s3 = LR"(|::|
|::|)";

	cgu::AnimatedSprite     bird({s1,s2,s3}, 70.f);
	bird.set_speed(0.7f);
	Motion                  motion;

	std::wstring S[4] = {
		LR"(O
||
/\)",   LR"( O
/\
||)",   LR"(O
||
//)",   LR"( O
||
\\)"
	};

	cgu::AnimatedSprite     man({S[0],S[1],S[2],S[3]}, 70.f);
	man.set_speed(0.73f);
	Motion                  walk;

	cgu::AnimatedSprite*    ptr = &man;
	Motion*                 p_motion = &walk;

	// Menu : ----------------------------------------------------
	std::wstring wstrMenu = LR"(M : Select a Man
B : Select a Bird
I : Pause And Show Menu
ESCAP : Exit 
)";
	std::wstring wstrMenu_ = LR"(I:Pause Escap:Exit)";



	// timer to calculate frame per seconds
	Time::Game_Timer g_timer;
	float elapsed_time{ 1.f };
	bool b_pause = false;
	// string wchar_t buffer for title
	wchar_t s[150]{};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Main Loop
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (true) {
		g_timer.reset();
		float frames = 1.f / elapsed_time;

		swprintf_s(s, 150,
			L"Game Console v1.0 |Frame//sec[%5.2f][%3.2f,%3.2f]", frames , bird.get_position().x
			, bird.get_position().y
		);
		// title
		TITLE(s);

		// Show the Menu
		if (KeyReleased(_u('I'))) {
			b_pause = !b_pause;
		}

		if (b_pause) {
			cgu::message_box(30, 10, L"Menu", wstrMenu);
			goto near_end;
		}

		cgu::console.clear_screen();

		// selection 

		if (KeyReleased(_u('M'))) {
			ptr = &man;
		}

		if (KeyReleased(_u('B'))) {
			ptr = &bird;
		}





		// controle movement by visitor class

		if (ptr)
		ptr->accept(*p_motion, frames);




		///////////// drawing section priority  /.//////////////////////////////////////////////////
		// object
		if (ptr) {
			if (p_motion->_is_moving)
				ptr->animate(frames);
			else
				ptr->draw(frames);
		}

		// draw rectangle scene
		cgu::draw_rectangle({ 0,0,_length - 1, _width - 1 });
		cgu::print_text_at(wstrMenu_, 0, _width - 1);

	near_end:

		cgu::console.display();
		elapsed_time = g_timer.get_elapsed_time();
		if (cgu::get_out()) break;
	}

	return 0;
}
