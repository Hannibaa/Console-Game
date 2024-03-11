/* 
    Particule collision of one points
    Author : Kadda Aoues
    Date   : 3 / 9 /2024
*/

#include "particule_collision_v_2.hpp"
#include "MyLib/chrono/Game_timer.h"
#include "MyLib/random_generator.h"

template<typename F>
void execute_if_key_released(int key, F  f) {
    if (KeyReleased(key)) {
        f();
    }
}

int main() {
    // console global variable 
    int g_lx = 145;
    int g_ly = 35;

    int g_fieldx = 80;
    int g_fieldy = 34;

    int id{};

    cgu::iRect  field_rect{ 1,1,g_fieldx, g_fieldy };

    cgu::console.construct_console(g_lx, g_ly);

    // object initialization

    // lambda generator
	std::vector<FreeParticule>            vp;
    FreeParticule*                        ptr{};

	auto generate = [&](wchar_t c = 0x2588) {
		FreeParticule     p3;
        rng::fRG<float>     frand;
        rng::iRG<int>       irand;
		p3._p.set_position({ frand(3.f, g_fieldx - 5),frand(3.f, g_fieldy -5)});
        p3._p.set_id(++id);
		p3._p.set_glyph(L"\x2593\x2592\x2591"[irand(0,2)]);
		p3._motion.set_speed(frand(0.5f, 0.05f), frand(0.5f,0.05f));
		p3._motion.set_bounds(2, 2, g_fieldx - 1, g_fieldy - 1);
        vp.push_back(p3);
		};


    // Menu definition setup
    std::wstring  wstrMenuTail = LR"(M:Menu ESC:Quite G:Gener R:Rmv)";
    std::wstring  wstrMenuTitle = L"Menu";
    std::wstring  wstrMenuBody = LR"(M: Menu
G: Create one element free
R: Pop back one element.
ESC: Quite Game)";

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Main Loop  
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
    Time::Game_Timer   timer;
    bool               b_pause{ false };
    cgu::Dt60 = 1.5f;

    while (true) {
        // header timer, frame calculation, title
        cgu::Dtime = cgu::Dt60 * 60.f * cgu::elps;

        timer.reset();
        swprintf_s(cgu::title, 255, L"Fps[%5.2f][%4d]{%3.2f, %3.2f}", cgu::fps, vp.size(),
            0.f, 0.f);
        TITLE(cgu::title);

        // window clear
        cgu::console.clear_screen();
        
        // Menu affich
        if (KeyReleased(_u('M'))) {
            b_pause = !b_pause;
        }
        if (b_pause) {
            cgu::message_box(20 + g_lx / 2, g_ly / 2, wstrMenuTitle, wstrMenuBody);
            goto __next;
        }

        // Generation and Selection
        if (KeyReleased(_u('G'))) {
            generate();
        }

        execute_if_key_released(_u('R'), [&] {
            if (!vp.empty()) vp.pop_back();
            });

        execute_if_key_released(_u('A'), [&] {
            for (int i = 0; i < 100; ++i) generate();
            });



        // Cinematics And controle
        for(auto& p : vp) p.move(cgu::Dtime);


        // Collision ---------------------------------------------
        if (!vp.empty())
        {
            for (auto it = vp.begin(); it != vp.end(); ++it) {
                for (auto it2 = vp.begin(); it2 != vp.end(); ++it2)
                {
                    if (it->_p.get_id() != it2->_p.get_id()) {
                        if (cgu::is_collid(it->_p, it2->_p)) {
                            cgu::collision_process(it->_p, it2->_p, 4.f);
                        }
                    }
                }
            }
        }

    __next:
        // drawing
        for(auto& p : vp) p.draw();



        // console display
        cgu::draw_rectangle({ 1,1, g_fieldx - 2, g_fieldy - 2 }, 0x2592);
        cgu::draw_rectangle({ 0, 0, g_lx - 1, g_ly - 1 });
        cgu::print_text_at(wstrMenuTail, 1, 34);
        cgu::console.display();

        // Quite by escape
        if (cgu::get_out()) break;
        // calculate time frame per second
        cgu::elps = timer.get_elapsed_time();
        cgu::fps = 1.f / cgu::elps;
    }
}