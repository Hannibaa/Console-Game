#pragma once
#include "../include/shape_v_1.hpp"
#include "../include/Cinematic.hpp"
#include "MyLib/chrono/Game_timer.h"


// move object

class PlayerMove : public cgu::cin::IMotion, public cgu::cin::IControle {
public:
	template<typename Player>
	void operator()(Player* player, float Dt) {

		cgu::fPoint2d p = player->get_position();

		if (KeyPressed(key_left)) {
			p.x -= m_speed.x * Dt;
		}

		if (KeyPressed(key_right)) {
			p.x += m_speed.x * Dt;
		}

		p.clamped({ float(m_field.x), float(m_field.dx) }, { float(m_field.y), float(m_field.dy) });

		player->set_position(p);
	}
};


// Ball
struct Life_time { float life_time{}; };

class Ball : public cgu::TISprite<wchar_t>  {
	Time::Game_Timer     timer;
	float*               m_life_time;
public:

	cgu::cin::FreeMotion       motion;

	Ball(wchar_t c = 0x2588 , float x = 10.f , float y = 10.f)
		: TISprite(c,x,y)
	{
		timer.reset();
	}

	void move(float Dt) {
		motion(this, Dt);
	}

	virtual void draw() const override {
		cgu::console(_pos.x, _pos.y, _buffer);
	}

	virtual cgu::iRect get_bounds() const override {
		return cgu::iRect{ int(_pos.x), int(_pos.y) , 1 , 1 };
	}

	virtual cgu::fPoint2d get_dimension() const override {
		return cgu::fPoint2d{ 1.f ,1.f };
	}

	void reverse(int _case = 0) {
		motion.reverse(_case);
	}
};