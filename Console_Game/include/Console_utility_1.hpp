#pragma once

#include "CConsole.h"
#include "wstring_utility.h"

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Some subroutine utility for 
	//    Only Subroutine associated with consoles
	//    cgu for console game utility
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KeyOn(Key)           GetAsyncKeyState(Key) & 0x8000
#define KeyReleased(Key)     GetAsyncKeyState(Key) & 0x0001
#define KeyPressed(Key)      GetAsyncKeyState(Key) & 0x8001

#define TITLE(wtitle)        SetConsoleTitle(wtitle)

#define _u(Char)            ((unsigned)Char)
#define InScreen(x,y,l,w)   ((x > -1 && x < l) && (y > -1 && y < w ))
#define _Is_Arithmetic(T)   static_assert(std::is_arithmetic_v<T>, "Type should be arithmetic type")


namespace cgu {

	// ********************** console ****************************...//
	//												
	cgu::Console console;												
	//												
	// ********************** console ****************************...//

	// define texture in mode text in dynamic vector
	using wstrTexture = std::vector<std::wstring>;

	template<size_t N>
	using warrTexture = std::array<std::wstring,N>;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Console Controle Font Size
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	COORD console_control_size(cgu::Console& _console, int keyA = _u('A'), int keyB = _u('B'),
		int key_up = VK_UP, int key_down = VK_DOWN) {

		auto cfi = _console.get_console_font_info();

		static COORD oldFontSize = cfi.dwFontSize;
		static COORD fontSize{ 7 , 7 };

		bool static b_dirX{ false };
		bool static b_dirY{ false };

		// make size as square shape
		if (KeyReleased(keyA)) {
			cfi.dwFontSize.Y = oldFontSize.X;
			cgu::console.set_console_font_info(cfi);
		}

		// return to old stat
		if (KeyReleased(keyB)) {
			cfi.dwFontSize.Y = oldFontSize.Y;
			cfi.dwFontSize.X = oldFontSize.X;
			cgu::console.set_console_font_info(cfi);
		}

		// activate which direction will be change
		if (KeyReleased(_u('X'))) {
			b_dirX = !b_dirX;
		}
		// activate direction Y:
		if (KeyReleased(_u('Y'))) {
			b_dirY = !b_dirY;
		}

		if (KeyReleased(key_up)) {
			if (b_dirX)++fontSize.X;
			if (b_dirY)++fontSize.Y;
			cgu::console.set_console_font_size(fontSize);
		}

		if (KeyReleased(key_down)) {
			if (b_dirX)--fontSize.X;
			if (b_dirY)--fontSize.Y;
			cgu::console.set_console_font_size(fontSize);
		}

		return fontSize;
	}


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

	// 
	const wchar_t  wT00   = 0x252c;
	const wchar_t  wT10   = 0x251c;
	const wchar_t  wT01   = 0x2524;
	const wchar_t  wT11   = 0x2534;
	const wchar_t  wX     = 0x253c;

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

		void clamped(const Point2d<T>& x_limit, const Point2d<T>& y_limit) {
			x = std::clamp(x, x_limit.x, x_limit.y );
			y = std::clamp(y, y_limit.x, y_limit.y );
		}

		bool operator == (const Point2d p) {
			return this->x == p.x && this->y == p.y;
		}

	};
	
	template<typename T>
	inline Point2d<T> operator - (const Point2d<T>& lhv, const Point2d<T>& rhv) {
		return Point2d<T>{ lhv.x - rhv.x, lhv.y - rhv.y };
	}


	///  Rectangle object

	template<typename T>
	struct Rect {
		static_assert(std::is_arithmetic_v<T>, "Type T should be arithmetic type");
		T x;
		T y;
		T dx;
		T dy;

		Rect() :x{}, y{}, dx{}, dy{} {}

		Rect(T _x, T _y, T _dx, T _dy)
			: x{ _x }
			, y{ _y }
			, dx{ _dx }
			, dy{ _dy }
		{}

		template<typename U>
		explicit Rect(const Rect<U>& rect)
			: x {static_cast<T>(rect.x)}
			, y{ static_cast<T>(rect.y) }
			, dx {static_cast<T>(rect.dx)}
			, dy{static_cast<T>(rect.dy)}
		{}

		bool contain(T _x, T _y) const {
			return (_x >= x && _x < x + dx) &&
				   (_y >= y && _y < y + dy);
		}

		bool contain(const Point2d<T>& p) const {
			return contain(p.x, p.y);
		}

		bool contain(const Rect<T>& rect) const {
			return contain(rect.x, rect.y) || 
				   contain(rect.x + rect.dx - 1, rect.y)|| 
				   contain(rect.x, rect.y + rect.dy - 1)|| 
				   contain(rect.x + rect.dx - 1, rect.y + rect.dy - 1);
		}

	};

	///////// Rectangle 
	typedef Rect<float>    fRect;
	typedef Rect<int>      iRect;
	typedef Point2d<float> fPoint2d;
	typedef Point2d<int>   iPoint2d;


	/////// Method check point x,y if it's interiour rectangle.
	template<typename T>
	bool IsPointInRect(const Point2d<T>& p, const Rect<T>& rect)    // Condition in  
	{
		if (p.x > rect.left &&
			p.x < rect.left + rect.width &&
			p.y > rect.top &&
			p.y < rect.top + rect.height) return true;

		return false;
	}

	// second one 
	template<typename T>
	bool IsPointInRect(T x, T y, const Rect<T>& rect) {
		return IsPointInRect(Point2d<T>(x, y), rect);
	}

	// Overlap two int particule
	template<typename P1, typename P2>
	bool Overlap(const P1& p1, const P2& p2) {
		auto pos1 = p1.get_position();
		auto pos2 = p2.get_position();

		return (pos2.x - 1 <= pos1.x && pos1.x <= pos2.x + 1) &&
			(pos2.y - 1 <= pos1.y && pos1.y <= pos2.y + 1);
	}

	// Overloap?
	template<typename O1, typename O2>
	bool is_collid(O1& p1, O2& p2) {

		if (p1.get_bounds().contain(p2.get_bounds())) return true;
		return false;
	}

	// treatment of collision between two object
	template<typename O1, typename O2>
	void collision_process(O1& q1, O2& q2, float aproxi = 2.f) {

		auto p1 = q1.get_position();
		auto p2 = q2.get_position();
		float lx = int(p1.x - p2.x) < 0 ? -1.0 : int(p1.x - p2.x) == 0 ? 0.f : 1.f;
		float ly = int(p1.y - p2.y) < 0 ? -1.0 : int(p1.y - p2.y) == 0 ? 0.f : 1.f;
		q1.set_position(p1.x + lx / aproxi, p1.y + ly / aproxi);
		q2.set_position(p2.x - lx / aproxi, p2.y - ly / aproxi);

	}

	// get out font :|:
	bool get_out(int Key_Escape = VK_ESCAPE) {

		if (KeyOn(Key_Escape)) return true;
		else return false;

	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Functions of drawing lines and text
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void draw_hline(int y, int x0, int x1, wchar_t c = L'\x2588') {

		for (int k = x0; k != x1 + 1; ++k)
			console(k, y, c);
	}

	void draw_vline(int x, int y0, int y1, wchar_t c = L'\x2588') {

		for (int k = y0; k != y1 + 1; ++k)
			console(x, k, c);
	}

	void draw_rectangle(const cgu::iRect& rect, wchar_t c = L'\x2588') {

		draw_hline(rect.x, rect.y, rect.x + rect.dx, c);
		draw_hline(rect.x + rect.dy, rect.x, rect.x + rect.dx, c);

		draw_vline(rect.x, rect.y, rect.y + rect.dy, c);
		draw_vline(rect.x + rect.dx, rect.y, rect.y + rect.dy, c);
	}

	void print_text_at(const std::wstring& wstr, int x, int y) {
		for (int j = 0; j < wstr.size(); ++j)
		{
			console(x + j, y, wstr[j]);
		}
	}


	// Print the text inside the box
	void print_text_in_box(int x, int y, const std::wstring text, wchar_t rl = L'\n') {

		for (int i = 0; i < text.size(); ++i) {
			std::wstring str;
			while (text[i] != rl) {
				str.push_back(text[i]);
				++i;
				if (i >= text.size()) break;
			}

			print_text_at(str, x, y++);
			str.clear();
		}
	}


	// message box 
	void message_box(int x, int y, const std::wstring& sTitle, const std::wstring& sBody)
	{
		// making box for message 
		/* 
		    +--[title]------------------------------+    1
			|										|	 2
			|		body    						|	 3
			+---------------------------------------+	 4
		*/
		int sz_title = (int)sTitle.size();
		auto [max_l, lines] = Str::get_boxed_text_dimension(sBody);
		
		if (max_l < sz_title + 8) max_l = sz_title + 8;

		int x0 = x + 3 + sz_title;
		
		// first line 1
		console(x    , y, wBox00);
		console(x + 1, y, whLine);
		console(x + 2, y, whLine);
		console(x + 3, y, wBracket0);
		print_text_at(sTitle, x + 4  , y);
		console(x0 + 1 , y, wBracket1);
		draw_hline(y, x0 + 2 , x + 2 + max_l , whLine);
		console(x + 3 + max_l, y ,  wBox01);

		// two vertical lines 2 and 3 :
		draw_vline(x            , y+1, y + lines , wvLine);
		draw_vline(x + 3 + max_l, y+1, y + lines , wvLine);

		// last horizontal line 4 :
		console(x, y + lines + 1, wBox10);
		draw_hline(y + lines + 1, x + 1, x + 2 + max_l, whLine);
		console(x + 3 + max_l, y + lines + 1, wBox11);

		// put the text inside box 
		print_text_in_box(x + 1, y + 1, sBody);
	}


	void progress_bar(std::wstring name ,
		int x, int y, int length, int value, int value_max = 30) 
	{
		if (value > value_max) value = value_max;
		int lv = float(length) * (float(value) / float(value_max));

		std::wstring wspace(length - lv, 0x2591);
		std::wstring wblank(lv, 0x2588);
		name.resize(10, L' ');
		std::wstring bar = name + wblank + wspace;

		print_text_at(bar, x, y);
	}
}