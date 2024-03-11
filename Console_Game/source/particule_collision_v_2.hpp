#pragma once
#include "../include/particules.h"
#include "../include/Cinematic.hpp"

// class for free parpitual movement
class FreeMotion : public cgu::cin::IMotion {

	float v1{ 1.0f };
	float v2{ 1.0f };

public:

	void operator()(cgu::Particule* ptr, float Dt)
	{
		cgu::fPoint2d  p = ptr->get_position();

		p.x += m_speed.x * Dt * v1;
		p.y += m_speed.y * Dt * v2;

		if ((int)p.x < m_field.x || (int)p.x > m_field.dx) v1 *= -1.f;
		if ((int)p.y < m_field.y || (int)p.y > m_field.dy) v2 *= -1.f;

		ptr->set_position(p);

	}

};



// class for free particule
class FreeParticule {
public:
	cgu::Particule    _p;
	FreeMotion        _motion;

	void move(float Dt) {
		_motion(&_p, Dt);
	}

	void draw() const {
		_p.draw();
	}

};

