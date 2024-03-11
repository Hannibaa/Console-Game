#pragma once
#include "sprite_v_3.h"


namespace cgu{

	class Particule : public cgu::TISprite<wchar_t> {

	public:
		Particule()
			:Particule(10,10)
		{}

		Particule(int x, int y, wchar_t c = 0x2588)
			:TISprite(c, x, y)
		{}

		virtual void draw() const override {
			cgu::console(_pos.x, _pos.y , _buffer);
		}

		virtual cgu::iRect get_bounds() const {
			return cgu::iRect{ int(_pos.x), int(_pos.y) , 2 , 2 };
		}

		virtual cgu::fPoint2d get_dimension() const {
			return cgu::fPoint2d{ 1.f,1.f };
		}

		void set_glyph(wchar_t c) {
			_buffer = c;
		}

	};


}