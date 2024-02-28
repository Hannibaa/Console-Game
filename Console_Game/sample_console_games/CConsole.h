#pragma once
#include <thread>
#include <vector>
#include <Windows.h>
#include <wchar.h>
#include <algorithm>
#include <MyLib/Console_Library/Unicode_table.h>

/*
                             Console Class  
        
		Author   : KADDA Aoues
		Date     : 02 / 25 / 2024
		Version  : 0.0.1

*/




namespace cu {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Console class Implementation   
	//    1. Normal class
	//    2. Advanced Class Console that support colors.
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////


	class DConsole {

	public:

		DConsole(int l, int w)
			: iscrLength{ l }
			 ,iscrWidth{ w }
		{}

		virtual void set_parameter() = 0;

		int  iscrLength;
		int  iscrWidth;

	};



	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Class that handle Console construction of screen object 'wchar_t' 
	//    1. create screen object
	//    2. set parameter of the object. <color><widht,heigh[or length]>
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Console : private DConsole {
	public:
		Console()
			: DConsole(145, 35)
		{
			// 3. Create screen buffer handle
			hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
				CONSOLE_TEXTMODE_BUFFER, NULL);

			if (hConsole == INVALID_HANDLE_VALUE)
				Error(L"Bad Handle");

			// 3.1 get oldmode saved
			GetConsoleMode(hConsole, &dwOldmode);

			// 4. Set console screen buffer handle to actual console
			if (!SetConsoleActiveScreenBuffer(hConsole) ) 
				Error(L"SetConsoleActiveScreenBuffer");

		}

		// buffer function object 
		void operator()(int i, int j, wchar_t w) {
			if ((i > -1 && i < iscrLength) && (j > -1 && j < iscrWidth))
				screen[i + j * iscrLength] = w;
		}

		virtual void set_parameter() override {

		}

		constexpr auto get_width() const {
			return this->iscrWidth;
		}

		constexpr auto get_length() const {
			return this->iscrLength;
		}

		// drawing in screen 
		virtual void draw() const  {
			DWORD dwByteWritten = 0;
			WriteConsoleOutputCharacter(hConsole, screen, iscrLength * iscrWidth, { 0,0 }, &dwByteWritten);

		}

		// clear screen
		void clear_screen(wchar_t char_fill = L' ') {
			wmemset(screen, char_fill, iscrLength * iscrWidth);
		}

		~Console() {
			delete[] screen;
			SetConsoleMode(hConsole, dwOldmode);
			//CloseHandle(hConsoleIn);
			CloseHandle(hConsole);
		}

		void construct_console(int length, int width) {

			iscrLength = length;
			iscrWidth = width;

			// 1. allocate space for screen pointer in type wchar_t 
			screen = new wchar_t[iscrLength * iscrLength];

			// 2. fill up screen with blank
			wmemset(screen, L' ', iscrLength * iscrWidth);


			// 5. Sizing Screen Buffer :
			// 5.1. Shrink console to minimum 
			SMALL_RECT temp_rect = { 0,0,1,1 };
			SetConsoleWindowInfo(hConsole, TRUE, &temp_rect);

			// 5.2 Set The Size of the screen buffer
			COORD coord = { (short)iscrLength, (short)iscrWidth };
			if (!SetConsoleScreenBufferSize(hConsole, coord))
				Error(L"SetConsoleScreenBufferSize");

			// 6. Set Physical Console Window Size
			temp_rect = { 0,0,(short)(iscrLength - 1), (short)(iscrWidth - 1) };
			if (!SetConsoleWindowInfo(hConsole, TRUE, &temp_rect))
				Error(L"SetConsoleWindowInfo");


			// 7. Set flags to allow mouse input
			// Not yet include...

			// Set Mothod to close handler -- will add in future --.
			//SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);

			// Call this function to write in screen buffer handler.
			DWORD dwByteWritten = 0;
			WriteConsoleOutputCharacter(hConsole, screen, iscrLength * iscrWidth, { 0,0 }, &dwByteWritten);

		}

	private:

		int Error(const wchar_t* msg)
		{
			wchar_t buf[256];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
			//SetConsoleActiveScreenBuffer(m_hOriginalConsole);
			SetConsoleMode(hConsole, dwOldmode);
			wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
			return 0;
		}

		HANDLE    hConsole;
		DWORD     dwOldmode;
		wchar_t*  screen;

	};



}