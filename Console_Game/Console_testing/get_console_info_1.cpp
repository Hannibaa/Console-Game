#include <windows.h>
#include <iostream>

#define end_     '\n'


int main() {

	LPDWORD  lpmode = new DWORD;
	LPDWORD  lpmodeIn = new DWORD;

	unsigned short s = 0x0001 | 0x0002 | 0x0004 ;
	std::cout << "lpmode before : " << *lpmode << end_;
	BOOL  b_out, b_in;
	HANDLE  hConsole = GetStdHandle(STD_INPUT_HANDLE); 
	HANDLE  hConsoleIn = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD   nFont{ 0 };
	PCONSOLE_FONT_INFO pfont_info = new CONSOLE_FONT_INFO;

	b_out = GetConsoleMode(hConsole, lpmode);
	b_in = GetConsoleMode(hConsoleIn, lpmodeIn);
	GetCurrentConsoleFont(hConsole, 0, pfont_info);
	nFont = pfont_info->nFont;
	COORD h_w_font = GetConsoleFontSize(hConsole, nFont);

	std::cout << "s " << s << end_;
	// ----------------------------------------------------------------------------------//
    // Console out mode info															 //
	std::cout << "handle of console out : " << hConsole << end_;						 //
	std::cout << "succes console out    : " << b_out << end_;							 //
	std::cout << "address lpmode out    : " << lpmode << end_;							 //
	std::cout << "lpmode                : " << *lpmode << end_;				 //
	std::cout << "Height of Font        : " << h_w_font.X << end_ ;                      //
	std::cout << "Width  of Font        : " << h_w_font.Y << end_ ;                      //
	std::cout << "the index of the font : " << nFont << end_ ;	       					 //
	std::cout << "console font info     : " << pfont_info->dwFontSize.X                  //
		                                    << " , " << pfont_info->dwFontSize.Y << end_;//
																						 //
																						 //
	std::cout << end_ << end_;		                                                     //
	// Console in mode info																 //
	std::cout << "handle of console out : " << hConsoleIn << end_;						 //
	std::cout << "succes console out    : " << b_in << end_;							 //
	std::cout << "address lpmode out    : " << lpmodeIn << end_;						 //
	std::cout << "lpmode                : " << std::hex << *lpmodeIn << end_;			 //
																						 //
	// ----------------------------------------------------------------------------------//

	std::cin.get();
	return 0;
}