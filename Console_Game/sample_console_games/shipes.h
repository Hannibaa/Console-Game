#pragma once

#include "Console_utility_1.hpp"

/* 
       Author : Kadda Aoues
	   Date   : 2024 january

	   Shipes that run in console
	   using namespace for console utility

*/

namespace cu {

	class sParticule {
		cu::Console& _console;
		float x;
		float y;

		float speedX;
		float speedY;

		wchar_t glyph;

	public:
		sParticule(cu::Console& console, wchar_t c = L'\x25a0')
			:_console{ console }
			, x{ 10 }
			, y{ 10 }
			, speedX{ 0.1f }
			, speedY{ 0.1f }
			, glyph{c}
		{}

		~sParticule() {}

		void set_speed(float vx, float vy) {
			speedX = vx;
			speedY = vy;
		}

		auto getX() const {
			return x;
		}

		auto getY() const {
			return y;
		}

		auto get_position() const {
			return cu::fPoint2d{ x,y };
		}

		void draw() {
			int _x{}, _y{};

			if (KeyPressed(VK_UP))     y -= speedY;
			if (KeyPressed(VK_DOWN))   y += speedY;
			if (KeyPressed(VK_LEFT))   x -= speedX;
			if (KeyPressed(VK_RIGHT))  x += speedX;

			x = std::clamp(x, 0.f, float(_console.get_length() - 1));
			y = std::clamp(y, 0.f, float(_console.get_width() - 1));

			_x = static_cast<int>(x);
			_y = static_cast<int>(y);


			_console(_x, _y, glyph );
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Particule Object
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	class Particule {
	public:

		float             _x, _y;
		float             _speedX, _speedY;
		int               v1{ 1 }, v2{ 1 };
		wchar_t           glyph;

		const int         _length;
		const int         _width;
	public:
		int               _id;
		bool              _is_static;

	public:

		Particule( float x, float y, wchar_t  w, int id = 0)
			: _x{ x }, _y{ y }
			, _speedX{ 0.01f }, _speedY{ 0.01f }
			, glyph(w)
			, _id{id}
			, _length{cgu::console.get_length()}
			, _width{cgu::console.get_width()}
			, _is_static{false}
		{}

		~Particule() {}

		void set_identity(int id) {
			_id = id;
		}

		void set_position(float x, float y) {
			_x = x;
			_y = y;
		}

		void set_position(const cgu::fPoint2d& p) {
			_x = p.x;
			_y = p.y;
		}

		void set_speed(float vx, float vy) {
			_speedX = vx;
			_speedY = vy;

			if (vx == 0.f && vy == 0.f) _is_static = true;
		}

		auto get_position() const {
			return cgu::fPoint2d{ _x, _y };
		}

		cgu::iRect get_bounds() const {
			return cgu::iRect(_x - 1, _y - 1, 3, 3);
		}

		auto getX() const {
			return _x;
		}

		auto getY() const {
			return _y;
		}

		void revers() {
			v1 = -v1;
			v2 = -v2;
		}

		void revers2() {
			v2 = -v2;
		}

		void move(const cgu::iPoint2d& p1, const cgu::iPoint2d& p2) {
			_x += _speedX * v1;
			_y += _speedY * v2;

			if (_x < p1.x || _x > p2.x)  v1 *= -1;
			if (_y < p1.y || _y > p2.y)  v2 *= -1;

		}

		void move() {
			move(cgu::iPoint2d{ 1,1 }, cgu::iPoint2d{ _length - 1, _width - 1 });
		}


		void draw() const {
			cgu::console(int(_x), int(_y), glyph);
		}

	};

}


