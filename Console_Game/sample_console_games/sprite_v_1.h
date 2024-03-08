#pragma once
#include "Console_utility_1.hpp"
#include "MyLib/geometry2d_utility.h"


/*
        Adopting new name for namespace Console Game Utility cgu::
*/

namespace cgu {

    // global variable dalta time elapsed
    float Dt60{};

    class IMoving {
    protected:
        cgu::fRect     _zone;
        cgu::fPoint2d  _v;

        float          Dtf{};

        int            v1{ 1 };
        int            v2{ 1 };

    public:
        IMoving()
        {
            _zone.x = 0.f;
            _zone.y = 0.f;
            _zone.dx = (float)cgu::console.get_length() - 1.f;
            _zone.dy = (float)cgu::console.get_width() - 1.f;
        }

        IMoving(const cgu::fPoint2d& p_corner, const cgu::fPoint2d& p_dim)
            :_zone{ p_corner.x, p_corner.y, p_dim.x, p_dim.y }
        {}

        virtual void init_waving(const cgu::fPoint2d& p_corner, const cgu::fPoint2d& p_dim) {
            _zone.x = p_corner.x;
            _zone.y = p_corner.y;
            _zone.dx = p_dim.x;
            _zone.dy = p_dim.y;
        }


        void set_speed(float vx, float vy) {
            _v.x = vx;
            _v.y = vy;
        }

        auto get_speed() const {
            return _v;
        }
    };

    class Waving : public IMoving {
    public:
        template<typename T>
        void operator()(T& s , float f) {

            cgu::fPoint2d p = s.get_position();
            auto pdim = s.get_dimension();
            Dtf = 60.f * Dt60 / f;
            p.x += _v.x * Dtf * v1;
            p.y += _v.y * Dtf * v2;

            if (p.x < _zone.x + 1 || p.x > _zone.x + _zone.dx - pdim.x + 1)  v1 *= -1;
            if (p.y < _zone.y + 1 || p.y > _zone.y + _zone.dy - pdim.y + 1)  v2 *= -1;


            s.set_position(p);
        }

    };

    class Sprite {

        cgu::wstrTexture                 _image;     // bidimentional texture 
        int                              _dimX;       // dimension in direction of x
        int                              _dimY;       // dimension in direction of y

        // initialize string image ; _image, _dimX, _dimY
        // create image from multi lines texts
        void init_sprite(const cgu::wstrTexture& list) {
            _dimX = 0;
            for (const auto& wstr : list) {
                _image.push_back(wstr);
                _dimX = (_dimX > int(wstr.size())) ? _dimX : int(wstr.size());
            }

            _dimY = (int)_image.size();
        }

        // create image from one line text
        void init_sprite(const std::wstring& text) {
            Str::convert_to_texture(text, _image);
            auto [dimX, dimY] = Str::get_boxed_text_dimension(text);
            _dimX = dimX;
            _dimY = dimY;
        }

    protected:
        uint32_t                          _id;         // identification of object
        float                              _x;         // position
        float                              _y;         // position

    public:

        Sprite()
            :_image{}
            ,_id{0}
            , _dimX{}, _dimY{}
            , _x{}, _y{}
        {}

        Sprite(const std::wstring& wtext, float x = 10.f, float y = 10.f, uint32_t id = 0) 
            :_id{id}
            , _x{ x }, _y{y}

        {
            init_sprite(wtext);
        }

        Sprite(const cgu::wstrTexture& text, float x = 10.f, float y = 10.f)
            :_id{}, _x{x}, _y{y}
        {
            init_sprite(text);
        }

        void set_id(int id) {
            _id = id;
        }

        int get_id() const {
            return _id;
        }

        cgu::fPoint2d get_position() const {
            return cgu::fPoint2d{ _x,_y };
        }

        cgu::fPoint2d get_dimension() const {
            return cgu::fPoint2d{ float(_dimX), float(_dimY) };
        }

        void set_position(const cgu::fPoint2d& p) {
            _x = p.x;
            _y = p.y;
        }

        void set_position(float x, float y) {
            _x = x;
            _y = y;
        }

        cgu::iRect get_bounds() const {
            return cgu::iRect{ (int)_x, (int)_y, _dimX, _dimY };
        }

        void draw(int x, int y) const {
            int j{};
            for (const auto& wstr : _image) {
                int i{};
                for (const auto& wchar : wstr) {
                    cgu::console(i + x, j + y, wchar);
                    ++i;
                }
                ++j;
            }
        }

        void draw() const {
            draw(int(_x), int(_y));
        }

    };

    class WSprite : public Sprite, public Waving {
    public:
        using Sprite::Sprite;

        void move(float f) {
            (*this)(*this, f);
        }
    };

    class Move : public IMoving {
    public:
        template<typename T>
        void operator()(T& s, float f) {
            Dtf =Dt60 * 60.f / f ;
            auto pdim = s.get_dimension();
            auto p    = s.get_position();

            if (KeyPressed(VK_UP)) {
                 p.y -= _v.y * Dtf ;
            }
            if (KeyPressed(VK_DOWN)) {
                p.y += _v.y * Dtf;
            }
            if (KeyPressed(VK_RIGHT)) {
                p.x += _v.x * Dtf;
            }
            if (KeyPressed(VK_LEFT)) {
                p.x -= _v.x * Dtf;
            }

            p.clamped({ _zone.x , _zone.x + _zone.dx - pdim.x - 1.f },
                      { _zone.y , _zone.y + _zone.dy - pdim.y - 1.f });

            s.set_position(p);
        }
    };

    class MSprite : public Sprite, public Move {
    public:
        using Sprite::Sprite;

        void move(float f) {
            (*this)(*this, f);
        }
    };

}