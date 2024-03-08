#pragma once
#include "sprite.h"


namespace cu {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Gravity affected object
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	class GSprite : public Sprite {

		float          _g;
		float          _up_g;

	public: 
		using Sprite::Sprite;

		void set_gravity(float g, float upg) {
			_g = g;
			_up_g = upg;
		}

		virtual void move() override {

			if (KeyPressed(_key_up))         _y -= _up_g * _vy;
			/*if (KeyPressed(_key_down))*/   _y += _g * _vy;
			if (KeyPressed(_key_left))       _x -= _vx;
			if (KeyPressed(_key_right))      _x += _vx;

			_x = std::clamp(_x, 1.f, float(cu::console.get_length() - _width - 1));
			_y = std::clamp(_y, 1.f, float(cu::console.get_width()  - _high  - 1));

		}

	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //   Moving Sprite  
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	class MSprite : public Sprite {
		int          _param1{ 1 };
		int          _param2{ 1 };
		

	public:
		using Sprite::Sprite;

		void set_param(int par1, int par2) {
			_param1 = par1;
			_param2 = par2;
		}

		virtual void move() override {
			_x -= _vx;

			if (_x < 0.f) _x = cu::console.get_length();
		}

		void pmove(const cu::iPoint2d& p1, const cu::iPoint2d& p2) {
			_x += float(_param1) * _vx;
			_y += float(_param2) * _vy;

			if (_x < p1.x || _x > p2.x)  _param1 *= -1;
			if (_y < p1.y || _y > p2.y)  _param2 *= -1;

		}
	};
}