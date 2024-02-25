#pragma once
#include <thread>
#include <vector>
#include <Windows.h>
#include <wchar.h>
#include <algorithm>
#include <MyLib/Console_Library/Unicode_table.h>

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Some subroutine utility for 
	//    Only Subroutine associated with consoles
	//    cu for console utility
	//    CO for console Object
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KeyOn(Key)           GetAsyncKeyState(Key) & 0x8000
#define KeyReleased(Key)     GetAsyncKeyState(Key) & 0x0001
#define KeyPressed(Key)      GetAsyncKeyState(Key) & 0x8001

#define TITLE(wtitle)        SetConsoleTitle(wtitle)

#define _u(Char)            ((unsigned)Char)
#define InScreen(x,y,l,w)   ((x > -1 && x < l) && (y > -1 && y < w ))
#define _Is_Arithmetic(T)   static_assert(std::is_arithmetic_v<T>, "Type should be arithmetic type")

namespace cu {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //   Constant wchar for boxing 
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Corner 
	const wchar_t  wBox00 = 0x250c;
	const wchar_t  wBox01 = 0x2510;
	const wchar_t  wBox10 = 0x2514;
	const wchar_t  wBox11 = 0x2518;

	// straight line
	const wchar_t  whLine = 0x2500;
	const wchar_t  wvLine = 0x2502;

	// wchar bracket

	const wchar_t wBracket0 = L'[';
	const wchar_t wBracket1 = L']';

	template<typename T>
	struct Point2d {
		static_assert(std::is_arithmetic_v<T>, "Type T should be arithmetic type");
		T x;
		T y;

		using value_type = T;

		Point2d(T _x = T{}, T _y = T{})
			: x{ _x }, y{ _y }
		{}

		template<typename U>
		Point2d<U> as() {
			return Point2d<U>{static_cast<U>(x), static_cast<U>(x)};
		}

		bool operator == (const Point2d p) {
			return this->x == p.x && this->y == p.y;
		}

	};
	
	template<typename T>
	inline Point2d<T> operator - (const Point2d<T>& lhv, const Point2d<T>& rhv) {
		return Point2d<T>{ lhv.x - rhv.x, lhv.y - rhv.y };
	}

	template<typename T>
	struct Rect {
		static_assert(std::is_arithmetic_v<T>, "Type T should be arithmetic type");
		T left;
		T top;
		T width;
		T height;

		Rect() :top{}, left{}, width{}, height{} {}

		Rect(T _left, T _top, T _width, T _height)
			: left{ _left }
			, top{_top}
			, width{ _width }
			, height{ _height }
		{}

		template<typename U>
		explicit Rect(const Rect<U>& rect)
			: top {static_cast<T>(rect.top)}
			, left{ static_cast<T>(rect.left) }
			, width {static_cast<T>(rect.width)}
			, height{static_cast<T>(rect.height)}
		{}

		bool contain(T x, T y) const {
			return (x >= left && x <= left + width) &&
				   (y >= top && y <= top + height);
		}

		bool contain(const Rect<T>& rect) {
			return contain(left, top) && contain(left + width, top)
				&& contain(left, top + height) && contain(left + width, top + height);
		}

	};

	typedef Rect<float>    fRect;
	typedef Rect<int>      iRect;
	typedef Point2d<float> fPoint2d;
	typedef Point2d<int>   iPoint2d;

	template<typename T>
	bool IsPointInRect(const Point2d<T>& p, const Rect<T>& rect)    // Condition in  
	{
		if (p.x > rect.left &&
			p.x < rect.left + rect.width &&
			p.y > rect.top &&
			p.y < rect.top + rect.height) return true;

		return false;
	}

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
			: iscrLength{ l },
			iscrWidth{ w }
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

			// 3. Create screen buffer handle
			hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
				CONSOLE_TEXTMODE_BUFFER, NULL);

			// 4. Set console screen buffer handle to actual console
			SetConsoleActiveScreenBuffer(hConsole);

			// Call this function to write in screen buffer handler.
			DWORD dwByteWritten = 0;
			WriteConsoleOutputCharacter(hConsole, screen, iscrLength * iscrWidth, { 0,0 }, &dwByteWritten);
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
			//SetConsoleMode(hConsoleIn, dwOldMode);
			//CloseHandle(hConsoleIn);
			CloseHandle(hConsole);

		}

	private:
		void construct_console() const {

		}


		HANDLE    hConsole;
		wchar_t* screen;

	};





//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Functions
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw_hline(cu::Console& console, int y, int x0, int x1, wchar_t c = L'\x2588') {

		for (int k = x0; k != x1 + 1; ++k)
			console(k, y, c);
	}

	void draw_vline(cu::Console& console, int x, int y0, int y1, wchar_t c = L'\x2588') {

		for (int k = y0; k != y1 + 1; ++k)
			console(x, k, c);
	}

	void draw_rectangle(cu::Console& console, const cu::iRect& rect, wchar_t c = L'\x2588') {

		draw_hline(console, rect.top, rect.left, rect.left + rect.width, c);
		draw_hline(console, rect.top + rect.height, rect.left, rect.left + rect.width, c);

		draw_vline(console, rect.left, rect.top, rect.top + rect.height, c);
		draw_vline(console, rect.left + rect.width, rect.top, rect.top + rect.height, c);
	}

	void print_text_at(Console& console, const std::wstring& wstr, int x, int y) {
		for (int j = 0; j < wstr.size(); ++j)
		{
			console(x + j, y, wstr[j]);
		}
	}

	std::pair<int, int> get_boxed_text_dimension(const std::wstring& text, wchar_t rl = L'\n') {
		int max = 0;
		int lines = 0;

		for (int i = 0; i < text.size(); ++i) {
			int count{};
			while (text[i] != rl) { ++count; ++i; if (i >= text.size()) break; }
			max = (count >= max) ? count : max;
			++lines;
		}

		return std::pair{ max, lines };
	}

	// Print the text inside the box
	void print_text_in_box(Console& console, int x, int y, const std::wstring text, wchar_t rl = L'\n') {

		for (int i = 0; i < text.size(); ++i) {
			std::wstring str;
			while (text[i] != rl) {
				str.push_back(text[i]);
				++i;
				if (i >= text.size()) break;
			}

			print_text_at(console, str, x, y++);
			str.clear();
		}
	}


	void message_box(Console& console, int x, int y, const std::wstring& sTitle, const std::wstring& sBody)
	{
		// making box for message 
		/* 
		    +--[title]------------------------------+    1
			|										|	 2
			|		body    						|	 3
			+---------------------------------------+	 4
		*/
		int sz_title = (int)sTitle.size();
		auto [max_l, lines] = get_boxed_text_dimension(sBody);

		if (max_l < sz_title + 8) max_l = sz_title + 8;

		int x0 = x + 3 + sz_title;
		
		// first line 1
		console(x    , y, wBox00);
		console(x + 1, y, whLine);
		console(x + 2, y, whLine);
		console(x + 3, y, wBracket0);
		print_text_at(console, sTitle, x + 4  , y);
		console(x0 + 1 , y, wBracket1);
		draw_hline(console, y, x0 + 2 , x + 2 + max_l , whLine);
		console(x + 3 + max_l, y ,  wBox01);

		// two vertical lines 2 and 3 :
		draw_vline(console, x            , y+1, y + lines , wvLine);
		draw_vline(console, x + 3 + max_l, y+1, y + lines , wvLine);

		// last horizontal line 4 :
		console(x, y + lines + 1, wBox10);
		draw_hline(console, y + lines + 1, x + 1, x + 2 + max_l, whLine);
		console(x + 3 + max_l, y + lines + 1, wBox11);

		// put the text inside box 
		print_text_in_box(console, x + 1, y + 1, sBody);
	}
}