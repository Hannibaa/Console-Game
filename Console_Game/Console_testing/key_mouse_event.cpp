#include <Windows.h>
#include <iostream>
#include <MyLib/Console_Library/escape_code.h>

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
void print_event(KEY_EVENT_RECORD ker) {
	print_ << "Key Event -----------------------------------" << end_;
	print_ << "bKeyDown            : " << ker.bKeyDown << end_;
	print_ << "wRepeatCount        : " << ker.wRepeatCount << end_;
	print_ << "wVirtualKeyCode     : " << ker.wVirtualKeyCode << end_;
	print_ << "wVirtualScanCode    : " << ker.wVirtualScanCode << end_;
    wprint_ << "Wchar               : " << ker.uChar.UnicodeChar << wend_;
	print_ << "Char                 : " << ker.uChar.AsciiChar << end_;
	print_ << "dwControlKeyState    : " << ker.dwControlKeyState << end_;
	print_ << "Key Event -----------------------------------" << end_;
}

// Mouse event record ...
void print_event(MOUSE_EVENT_RECORD mer) {
	print_ << "Mouse Event Structure : -----------------------------------------" << end_;
	print_ << "dwMousePosition       : " << mer.dwMousePosition.X << " , " << mer.dwMousePosition.Y << end_;
	print_ << "dwButtonState         : " << mer.dwButtonState << end_;
	print_ << "dwControlKeyState     : " << mer.dwControlKeyState << end_;
	print_ << "dwEventFlags          : " << mer.dwEventFlags << end_;
	print_ << "Mouse Event Structure : -----------------------------------------" << end_;
}

// Focus event record...
void print_event(FOCUS_EVENT_RECORD fer) {
	print_ << "Focus Event Structure : ------------------------------------------" << end_;
	print_ << "bSetfocus : " << fer.bSetFocus << end_;
}

// Window Buffer Size Record
void print_event(WINDOW_BUFFER_SIZE_RECORD wbsr) {
	print_ << "--------------------Windows buffer Size -------------------------------\n";
	print_ << "DwSize : " << wbsr.dwSize.X << " , " << wbsr.dwSize.Y << end_;
	print_ << "--------------------------------- End ---------------------------------\n";
}

// Menu event record
void print_event(MENU_EVENT_RECORD mmer) {
	print_ << "------------------------------ Menu event start -----------------------------\n";
	print_ << "Menu event : " << mmer.dwCommandId << end_;
	print_ << "End ... " << end_;
	print_ << "----------------------------------- End  ------------------------------------\n";
}

void print_info(CONSOLE_SCREEN_BUFFER_INFO csbi) {
	print_ << "-------------- Console Screen Buffer Information ---------------------\n";
	print_ << "Size of screen buffer : " << csbi.dwSize.X << " , " << csbi.dwSize.Y << end_;
	print_ << "column and row of cursor:  " << csbi.dwCursorPosition.X << " , " 
		                                    << csbi.dwCursorPosition.Y << end_;
	print_ << "Attribute : " << csbi.wAttributes  << end_ ;
	print_ << "Coordinate of windows : " << csbi.srWindow.Top   << ", " << csbi.srWindow.Left   << "|"
										 << csbi.srWindow.Right << ", " << csbi.srWindow.Bottom << end_;
	print_ << "Maximum window Size : " << csbi.dwMaximumWindowSize.X << " , " << csbi.dwMaximumWindowSize.Y << end_;

	print_ << "-------------- Console Screen Buffer Information End ---------------------\n";
}

HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);

int main() {

	DWORD  _modein;
	DWORD  _modeout;
	DWORD  _Number_event;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleMode(hstdin, &_modein);
	GetConsoleMode(hstdout, &_modeout);
	

	DWORD _newmodein = ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT;
	if (! SetConsoleMode(hstdin, _newmodein | _modeout) ) print_ << "Error : SetConsoleMode\n";


	if (!GetConsoleScreenBufferInfo(hstdout, &csbi)) {
		print_ << "Error to execute GetConsoleScreenBufferInfo \n";
	}

	print_info(csbi);
	


	print_ << "handle std in  : " << hstdin  << end_;
	print_ << "handle std out : " << hstdout << end_;

	for (int k = 0; k < 10; ++k) {
		DWORD events{};
		GetNumberOfConsoleInputEvents(hstdin, &events);
		print_ << "events : " << events << end_;
		// reading key event structure 
		if (!ReadConsoleInput(hstdin, _ir, 128, &_Number_event)) {
			print_ << "Error in Read console Input \n";
		}

		// dispatch event 
		print_ << "Number of reading event : " << _Number_event << end_;

		for (int i = 0; i < _Number_event; ++i) {
			if (_ir[i].EventType == KEY_EVENT)
				print_event(_ir[i].Event.KeyEvent);

			if (_ir[i].EventType == MOUSE_EVENT)
				print_event(_ir[i].Event.MouseEvent);

			if (_ir[i].EventType == FOCUS_EVENT)
				print_event(_ir[i].Event.FocusEvent);

			if (_ir[i].EventType == WINDOW_BUFFER_SIZE_EVENT)
				print_event(_ir[i].Event.WindowBufferSizeEvent);

			if (_ir[i].EventType == MENU_EVENT)
				print_event(_ir[i].Event.MenuEvent);
		}

	}



	SetConsoleMode(hstdin,_modein);
	SetConsoleMode(hstdout,_modeout);

	print_ << " end of program ..." << end_;
	wait_;
	return 0;
}