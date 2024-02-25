#pragma once
#include "Console_utility_1.hpp"
#include <algorithm>

/* 
       Author : Kadda Aoues
	   Date   : 2024 january

	   Shipes that run in console
	   CO mean console object

*/

namespace CO {

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

		cu::Console& _console;
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

		Particule(cu::Console& console, float x, float y, wchar_t  w, int id = 0)
			:_console{ console }
			, _x{ x }, _y{ y }
			, _speedX{ 0.01f }, _speedY{ 0.01f }
			, glyph(w)
			, _id{id}
			, _length{ console.get_length() }
			, _width{ console.get_width() }
			, _is_static{false}
		{}

		Particule& operator = (const Particule&) = default;

		~Particule() {}

		void set_identity(int id) {
			_id = id;
		}

		void set_position(float x, float y) {
			_x = x;
			_y = y;
		}

		void set_position(const cu::fPoint2d& p) {
			_x = p.x;
			_y = p.y;
		}

		void set_speed(float vx, float vy) {
			_speedX = vx;
			_speedY = vy;

			if (vx == 0.f && vy == 0.f) _is_static = true;
		}

		auto get_position() const {
			return cu::fPoint2d{ _x, _y };
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

		void move(const cu::iPoint2d& p1, const cu::iPoint2d& p2) {
			_x += _speedX * v1;
			_y += _speedY * v2;

			int x = static_cast<int>(std::ceil(_x));
			int y = static_cast<int>(std::ceil(_y));

			if (x < p1.x || x > p2.x)  v1 *= -1;
			if (y < p1.y || y > p2.y)  v2 *= -1;

			_console(x, y, glyph);
		}

		void move() {
			move(cu::iPoint2d{ 0,0 }, cu::iPoint2d{ _length - 1, _width - 1 });
		}

		void move(int energy) {
			_x += _speedX * v1;
			_y += _speedY * v2;

			if (energy > 0) --energy; else return;

			_speedX -= 0.0001f;
			_speedY -= 0.0001f;

			int x = static_cast<int>(_x);
			int y = static_cast<int>(_y);

			if (x < 0 || x > _length - 1)  v1 *= -1;
			if (y < 0 || y > _width - 1)  v2 *= -1;

			_console(x, y, glyph);
		}

	};


}


