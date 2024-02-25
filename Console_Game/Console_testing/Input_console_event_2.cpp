#include <Windows.h>
#include <iostream>
#include <thread>
#include <MyLib/Console_Library/text_user_interface.h>
#include <MyLib/chrono/Game_timer.h>

/*
			   Console function testing
			   1. console structure
			   2. console get standart stdin and stdout
			   3. console get information about screen buffer info
			   4. console stdin event treatment .
*/


CHAR_INFO                          _ci{};

INPUT_RECORD                       _ir[128]{};
KEY_EVENT_RECORD                   _ker{};
MOUSE_EVENT_RECORD                 _mer{};
WINDOW_BUFFER_SIZE_RECORD          _wfsr{};

// key event record ... 
void print_event(KEY_EVENT_RECORD ker, POINT pos = {1,1}) {
	std::stringstream ss;
	ss << "bKeyDown            : " << ker.bKeyDown << end_;
	ss << "wRepeatCount        : " << ker.wRepeatCount << end_;
	ss << "wVirtualKeyCode     : " << ker.wVirtualKeyCode << end_;
	ss << "wVirtualScanCode    : " << ker.wVirtualScanCode << end_;
	ss << "Char                 : " << ker.uChar.AsciiChar << end_;
	ss << "dwControlKeyState    : " << ker.dwControlKeyState << end_;
	cui::Message_Box msg;

	msg(pos.x, pos.y, "Key Event", ss.str());

	//wprint_ << "Wchar               : " << ker.uChar.UnicodeChar << wend_;
}

// Mouse event record ...
void print_event(MOUSE_EVENT_RECORD mer, POINT pos = {1,1}) {
	std::stringstream ss;
	ss << "Mouse Event Structure : -----------------------------------------" << end_;
	ss << "dwMousePosition       : " << mer.dwMousePosition.X << " , " << mer.dwMousePosition.Y << end_;
	ss << "dwButtonState         : " << mer.dwButtonState << end_;
	ss << "dwControlKeyState     : " << mer.dwControlKeyState << end_;
	ss << "dwEventFlags          : " << mer.dwEventFlags << end_;
	ss << "Mouse Event Structure : -----------------------------------------" << end_;

	cui::Message_Box msg;
	msg(pos.x, pos.y, "Mouse Event", ss.str());
}

// Focus event record...
void print_event(FOCUS_EVENT_RECORD fer, POINT pos = {1,1}) {
	std::stringstream ss;
	ss << "bSetfocus : " << fer.bSetFocus << end_;

	cui::Message_Box msg;
	msg(pos.x, pos.y , "Focus Event", ss.str());
}

// Window Buffer Size Record
void print_event(WINDOW_BUFFER_SIZE_RECORD wbsr, POINT pos = {1,1}) {
	std::stringstream ss;
	ss << "DwSize : " << wbsr.dwSize.X << " , " << wbsr.dwSize.Y << end_;

	cui::Message_Box msg;
	msg(pos.x,pos.y, "Window Size", ss.str());
}

// Menu event record
void print_event(MENU_EVENT_RECORD mmer , POINT pos = {1,1}) {
	std::stringstream ss;
	ss << "Menu event : " << mmer.dwCommandId << end_;
	cui::Message_Box msg;
	msg(pos.x, pos.y , "Menu Event", ss.str());
}

void print_info(CONSOLE_SCREEN_BUFFER_INFO csbi,HANDLE _hstdin, HANDLE _hstdout, POINT pos = {1,1}) {
	std::stringstream ss;
	ss << "Size of screen buffer : " << csbi.dwSize.X << " , " << csbi.dwSize.Y << end_;
	ss << "column and row of cursor:  " << csbi.dwCursorPosition.X << " , "
		<< csbi.dwCursorPosition.Y << end_;
	ss << "Attribute : " << csbi.wAttributes << end_;
	ss << "Coordinate of windows : " << csbi.srWindow.Top << ", " << csbi.srWindow.Left << "|"
		<< csbi.srWindow.Right << ", " << csbi.srWindow.Bottom << end_;
	ss << "Maximum window Size : " << csbi.dwMaximumWindowSize.X << " , " << csbi.dwMaximumWindowSize.Y << end_;
	ss << "handle std in  : " << _hstdin << end_;
	ss << "handle std out : " << _hstdout << end_;

	cui::Message_Box msg;
	msg(pos.x, pos.y , "Console Screen Buffer Info", ss.str());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Thread to handle the input event runing in back ground
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD  _modein;
bool   b_program_end{ false };
DWORD  _Number_event;

void ThreadEvent() {

	GetConsoleMode(hstdin, &_modein);

	DWORD _newmodein = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hstdin, _newmodein)) print_ << "Error : SetConsoleMode\n";

	while (true) {
		if (b_program_end) break;

		// read event 
		ReadConsoleInput(hstdin, _ir, 128, &_Number_event);

	}

	SetConsoleMode(hstdin, _modein);
	print_ << "thread end ...\n";
}

int main() {

	esc::Init_Cursor init_cursor;

	DWORD  _modeout;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleMode(hstdout, &_modeout);


	std::thread thrStdIn(ThreadEvent);

	if (!GetConsoleScreenBufferInfo(hstdout, &csbi)) {
		print_ << "Error to execute GetConsoleScreenBufferInfo \n";
	}

	print_info(csbi, hstdin, hstdout);


	Time::Game_Timer timer{};
	wchar_t s[128]{};
	wait_;
	esc::cls();
	

	// Loop for intersept Event in Console
	while (true) {
		timer.reset();
		printm_(120, 2) << "Number of event : " << _Number_event << end_;
		// wait for event 
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));

		// dispatch event 
		esc::clear_line(120, 2);

		for (int i = 0; i < _Number_event; ++i) {
			if (_ir[i].EventType == KEY_EVENT)
				print_event(_ir[i].Event.KeyEvent);

			if (_ir[i].EventType == MOUSE_EVENT)
				print_event(_ir[i].Event.MouseEvent);

			if (_ir[i].EventType == FOCUS_EVENT)
				print_event(_ir[i].Event.FocusEvent, {60,1});

			if (_ir[i].EventType == WINDOW_BUFFER_SIZE_EVENT)
				print_event(_ir[i].Event.WindowBufferSizeEvent, {15,60});

			if (_ir[i].EventType == MENU_EVENT)
				print_event(_ir[i].Event.MenuEvent);
		}

		if (GetAsyncKeyState(VK_ESCAPE)) {
			b_program_end = true;
			break;
		}

		float frequency = 1.f / timer.get_elapsed_time();
		swprintf_s(s, 120, L"Elapsed Time is [%8.2f]", frequency);
		SetConsoleTitle(s);
	}



	SetConsoleMode(hstdout, _modeout);

	print_ << " end of program ..." << end_;
	wait_;
	thrStdIn.detach();
	return 0;
}