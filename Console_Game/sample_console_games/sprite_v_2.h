#pragma once
#include "Console_utility_1.hpp"
#include "MyLib/geometry2d_utility.h"

/*
      USING VISITOR PATTERNS

*/

namespace cgu {

    float Dt60{1.f};

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Visitors  
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Sprite;

    class ISpriteVisitor {
    public:
        virtual ~ISpriteVisitor() = default;
        virtual void visit(Sprite*, float) = 0;
    };

    class ISpriteMotionVisitor : public ISpriteVisitor {
    protected:
        cgu::fRect              _zone;
        cgu::fPoint2d           _v;
                                
        float                   Dtf{};
                                
        int                     v1{ 1 };
        int                     v2{ 1 };

    public:
        ISpriteMotionVisitor()
        {
            _zone.x = 0.f;
            _zone.y = 0.f;
            _zone.dx = (float)cgu::console.get_length() - 1.f;
            _zone.dy = (float)cgu::console.get_width() - 1.f;
        }

        ISpriteMotionVisitor(const cgu::fPoint2d& p_corner, 
                             const cgu::fPoint2d& p_dim,
                             const cgu::fPoint2d& speed = {1.f,1.f})
            :_zone{ p_corner.x, p_corner.y, p_dim.x, p_dim.y }
            , _v{speed}
        {}

        virtual void init_motion(const cgu::fPoint2d& p_corner,
                                 const cgu::fPoint2d& p_dim,
                                 const cgu::fPoint2d& speed) {
            _zone.x = p_corner.x;
            _zone.y = p_corner.y;
            _zone.dx = p_dim.x;
            _zone.dy = p_dim.y;
            _v.x = speed.x;
            _v.y = speed.y;
        }


        void set_speed(float vx, float vy) {
            _v.x = vx;
            _v.y = vy;
        }

        auto get_speed() const {
            return _v;
        }

    };


    class ISprite {
    public:
        virtual void accept(ISpriteVisitor&, float) = 0;
    };



    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //     CLASS SPRITE   
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Sprite : public ISprite {

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
            , _id{ 0 }
            , _dimX{}, _dimY{}
            , _x{}, _y{}
        {}

        Sprite(const std::wstring& wtext, float x = 10.f, float y = 10.f, uint32_t id = 0)
            :_id{ id }
            , _x{ x }, _y{ y }

        {
            init_sprite(wtext);
        }

        Sprite(const cgu::wstrTexture& text, float x = 10.f, float y = 10.f)
            :_id{}, _x{ x }, _y{ y }
        {
            init_sprite(text);
        }

        // override this 
        void accept(ISpriteVisitor& v, float f) override {
            v.visit(this,f);
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

}