#pragma once

#include "CConsole.h"

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

	// ********************** console ****************************...//
	cu::Console console;
	// ********************** console ****************************...//



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


	///  Rectangle object

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
			return (x >= left && x < left + width) &&
				   (y >= top && y < top + height);
		}

		bool contain(const Rect<T>& rect) const {
			return contain(rect.left, rect.top) || 
				   contain(rect.left + rect.width - 1, rect.top)|| 
				   contain(rect.left, rect.top + rect.height - 1)|| 
				   contain(rect.left + rect.width - 1, rect.top + rect.height - 1);
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

	// Overloap
	template<typename O1, typename O2>
	bool is_collid(O1& p1, O2& p2) {

		if (p1.get_bounds().contain(p2.get_bounds())) return true;
		return false;
	}

	template<typename O1, typename O2>
	void collision_process(O1& q1, O2& q2) {

		auto p1 = q1.get_position();
		auto p2 = q2.get_position();
		float lx = int(p1.x - p2.x) < 0 ? -1.0 : int(p1.x - p2.x) == 0 ? 0.f : 1.f;
		float ly = int(p1.y - p2.y) < 0 ? -1.0 : int(p1.y - p2.y) == 0 ? 0.f : 1.f;
		q1.set_position(p1.x + lx / 2.f, p1.y + ly / 2.f);
		q2.set_position(p2.x - lx / 2.f, p2.y - ly / 2.f);

	}


	bool get_out() {

		if (KeyOn(VK_ESCAPE)) return true;
		else return false;

	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Functions
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

	void draw_rectangle(const cu::iRect& rect, wchar_t c = L'\x2588') {

		draw_hline(rect.top, rect.left, rect.left + rect.width, c);
		draw_hline(rect.top + rect.height, rect.left, rect.left + rect.width, c);

		draw_vline(rect.left, rect.top, rect.top + rect.height, c);
		draw_vline(rect.left + rect.width, rect.top, rect.top + rect.height, c);
	}

	void print_text_at(const std::wstring& wstr, int x, int y) {
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

	// message box adem
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
		auto [max_l, lines] = get_boxed_text_dimension(sBody);

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
}