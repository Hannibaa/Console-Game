#pragma once
#include "../include/sprite_v_3.h"
#include "../include/Cinematic.hpp"




class tetro4 : public cgu::StaticSprite<4, 4> {
public:
    using cgu::StaticSprite<4, 4>::StaticSprite;

    void rotate() {
        std::wstring str;
        str.resize(16, L' ');
        for (int n = 0; n != 16; ++n) {
            str[n] = _buffer[cgu::_rot(n, 4)];
        }

        _buffer = str;
    }

    wchar_t  operator[](int n) const {
        return _buffer[n];
    }

    virtual void draw() const override {
        for (int i = 0; i != 4; ++i)
            for (int j = 0; j != 4; ++j)
            {
				if (_buffer[i + j * 4] != L' ')
					cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * 4]);
            }
    }
};


template<int lx, int ly>
class Field : public cgu::StaticSprite<lx, ly> {
public:
    using cgu::StaticSprite<lx,ly>::StaticSprite;

    wchar_t operator[](int n) const {
        return  cgu::StaticSprite<lx,ly>::_buffer[n];
    }
    
    wchar_t& operator[](int n) {
        return cgu::StaticSprite<lx, ly>::_buffer[n];
    }

    void set_last_line() {
        for (int i = 0; i != lx; ++i) {
            (*this)[i + (ly - 1)*lx] = 0x2588;
        }
    }

};


class Motion : public cgu::cin::IMotion, public cgu::cin::IControle {
public:
    template<typename Tsprite>
    void operator()(Tsprite* ptr, float Dt) {
        cgu::fPoint2d  p = ptr->get_position();
        cgu::fPoint2d  dim = ptr->get_dimension();

        p.y += m_speed.y * Dt ;

        if (KeyPressed(key_up)) {
            p.y -= m_speed.y * Dt;
        }

        if (KeyPressed(key_left)) {
            p.x -= m_speed.x * Dt;
        }

        if (KeyPressed(key_right)) {
            p.x += m_speed.x * Dt;
        }

        // ajust speed 
        if (KeyReleased(key_up_1)) {
            m_speed.x += 0.1f;
            m_speed.y += 0.1f;
        }
        if (KeyReleased(key_down_1)) {
            m_speed.x -= 0.1f;
            m_speed.y -= 0.1f;
        }

        //p.clamped({ float(m_field.x), float(m_field.dx) - dim.x }, { float(m_field.y) , float(m_field.dy) - dim.y });

        ptr->set_position(p);
    }
};


class Collision {
public:
    template<int lx, int ly>
    bool is_collid(tetro4* ptr_t, Field<lx,ly>* ptr_f) {

        auto p   = ptr_t->get_position();
        auto dim = ptr_f->get_bounds();
        p.x = p.x - dim.x ;
        p.y = p.y - dim.y;
        for (int i = 0; i != 4; ++i) {
            for (int j = 0; j != 4; ++j) {
                if ((*ptr_f)[p.x + i + (p.y + j) * dim.x] == (*ptr_t)[i + j * 4])
                {
                    return true;
                    p.y = p.y - 1;
                    ptr_t->set_position(p);
                    break;
                }
            }
        }

        return false;
    }


    template<typename T>
    void operator()(tetro4* ptr_t,T xmin, T xmax, T ymin, T ymax ) {
        auto p = ptr_t->get_position();

        p.clamped(xmin, xmax, ymin, ymax);

        ptr_t->set_position(p);
    }
};