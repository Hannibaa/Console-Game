#include <iostream>
#include <Windows.h>
#include <thread>
#include "MyLib/Console_Library/escape_code.h"

using namespace std;


int nScreenWidth = 120;
int nScreenHeight = 40;





int main()
{
	//Create Screen Buffer
	wchar_t* screen = new wchar_t[nScreenHeight * nScreenWidth];
	wmemset(screen, L' ', nScreenWidth * nScreenHeight);
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwByteWriten = 0;

	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwByteWriten);

	 const wchar_t* name = L"how are you?";

	// LOOP GAME
	while (1)
	{
		// Exit Key ESCAPE
		if (GetAsyncKeyState(VK_ESCAPE) & 0x0001) break;
		this_thread::sleep_for(20ms);

		wmemcpy(screen + 20, name, 12);
		
		//Display Frame.
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwByteWriten);
	}


	return 0;
}