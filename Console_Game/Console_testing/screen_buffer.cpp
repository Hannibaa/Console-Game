#include <windows.h>
#include <iostream>

#define end_     '\n'


int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	BOOL b_mode = GetConsoleDisplayMode(&mode);


	std::cout << "is it success : " << b_mode << end_;
	std::cout << "mode is " << mode << end_;
	COORD  coord; 
	b_mode = SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_HARDWARE, &coord);

	std::cout << "is it success : " << b_mode << end_;
	std::cout << "coord " << coord.X << " , " << coord.Y << end_;
	std::cout << "code page : " << GetConsoleCP() << end_;
	return 0;
}