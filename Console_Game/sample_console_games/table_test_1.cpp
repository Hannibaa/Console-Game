#include "../include/tables.h"
#include "MyLib/chrono/Game_timer.h"





int main() {
	int g_lx = 145;
	int g_ly = 35;

	cgu::console.construct_console(g_lx, g_ly);

	// table 
	cgu::table::Table    table{ {2,15,25, 45, 100},{7, 15, 20,25,30} };

	table.set_cell(1, 1, L"hello the world and than where");
	table.set_cell(0, 0, L"hello the world and than where");

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    LOOP
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
	Time::Game_Timer  timer;
	cgu::Dt60         = 1.4f;


	while (true) {
		cgu::Dtime = cgu::Dt60 * 60.f * cgu::elps;
		timer.reset();
		swprintf_s(cgu::title, 255, L"Fps[%5.2f]", cgu::fps);
		TITLE(cgu::title);

		cgu::console.clear_screen();



		// draw
		table.draw();

__next:
		// display screen
		cgu::draw_rectangle({ 0, 0, g_lx - 1, g_ly - 1});
		cgu::console.display();

		if (cgu::get_out()) break;
		cgu::elps = timer.get_elapsed_time();
		cgu::fps = 1.f / cgu::elps;
	}

}