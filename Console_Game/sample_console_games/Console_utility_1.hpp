#pragma once
#include <thread>
#include <vector>
#include <Windows.h>
#include <wchar.h>


    //////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Some subroutine utility for 
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KeyOn(Key)           GetAsyncKeyState(Key) & 0x8000
#define KeyReleased(Key)     GetAsyncKeyState(Key) & 0x0001
#define KeyPressed(Key)      GetAsyncKeyState(Key) & 0x8001

#define TITLE(wtitle)         SetConsoleTitle(wtitle)

#define _u(Char)            ((unsigned)Char)

bool get_out() {

	if (KeyOn(VK_ESCAPE)) return true;
	else return false;

}



class IConsole {
public:
	virtual void construct_console() const = 0;

	virtual void draw() const = 0;

	virtual ~IConsole() {}

};

class DConsole {

public:

	DConsole(int l, int w)
		: iscrLength{l},
		  iscrWidth{w}
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

class Console : public IConsole, private DConsole {
public:
	Console(int length, int width)
		: DConsole(length, width)
	{
		// 1. allocate space for screen pointer in type wchar_t 
		screen = new wchar_t[iscrLength * iscrLength];

		// 2. fill up screen with blank
		wmemset(screen, L' ', iscrLength * iscrWidth);

		// 3. Create screen buffer 
		hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
			CONSOLE_TEXTMODE_BUFFER, NULL);

		// 4. Set console screen buffer
		SetConsoleActiveScreenBuffer(hConsole);
		
		// Call this function to write in screen buffer.
		DWORD dwByteWritten = 0;
		WriteConsoleOutputCharacter(hConsole, screen, iscrLength * iscrWidth, { 0,0 }, &dwByteWritten);
	}

	wchar_t operator()(int i, int j) const {
		if ( i > 0 && i < iscrLength && j >0 && j < iscrWidth)
		return screen[i + j * iscrLength];
	}

	wchar_t& operator()(int i, int j) {
		if (i > -1 && i < iscrLength && j > -1 && j < iscrWidth)
		return screen[i + j * iscrLength];
	}

	virtual void set_parameter() override {

	}

	auto get_width() const {
		return this->iscrWidth;
	}

	auto get_length() const {
		return this->iscrLength;
	}

	// drawing in screen 
	virtual void draw() const override {
		DWORD dwByteWritten = 0;
		WriteConsoleOutputCharacter(hConsole, screen, iscrLength * iscrWidth, { 0,0 }, &dwByteWritten);

	}

	// clear screen
	void clear_screen(wchar_t char_fill = L' ') {
		wmemset(screen, char_fill, iscrLength * iscrWidth);
	}

	~Console() {
		delete[] screen;
	}

private:
	void construct_console() const {
	
	}


	HANDLE    hConsole;
	wchar_t*  screen;
	
};


//class Sprite {
//	int                                _high;
//	int                                _width;
//	std::vector<std::wstring>          _vecImage;
//	Console&                           _console;
//
//	void Create(const std::wstring& image) {
//		for (int kj = 0; kj < _high; ++kj) {
//
//			for (int ki = 0; ki < _width; ++ki) {
//				if ( ki + kj > )
//				_vecImage[kj].push_back(image[ki+kj]);
//			}
//		}
//	}
//
//public:
//	Sprite(int high, int width,const std::wstring& image, Console& console) 
//		: _high{high},
//		  _width{_width},
//		  _console{console}
//	{
//		Create();
//	}
//
//	void operator()(int i, int j) {
//
//	}
//
//};