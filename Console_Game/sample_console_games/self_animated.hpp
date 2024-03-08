#pragma once
#include "../include/Console_utility_1.hpp"



namespace cgu {

    class AnimatedSprite;
    
    class ISpriteVisitor {
    public:
        virtual ~ISpriteVisitor() = default;

        virtual void visit(AnimatedSprite*, float ) = 0;
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

    class AnimatedSprite : public ISprite {

        cgu::wstrTexture                 _image;     // bidimentional texture 
        int                              _dimX;       // dimension in direction of x
        int                              _dimY;       // dimension in direction of y

        // initialize string image ; _image, _dimX, _dimY
        // create image from multi lines texts
        void init_sprite(const cgu::wstrTexture& list) {
            for (const auto& wstr : list) {
                _image.push_back(wstr);
            }
        }

        auto get_rect(int& dx, int& dy) {
            dx = 0; dy = 0;
            for (const auto& s : _image) {
                auto [_dx, _dy] = _GET_BOUNDS(s);
                dx = std::max<int>(dx, _dx);
                dy = std::max<int>(dy, _dy);
            }
            return std::pair{ dx,dy };
        }

		void draw(int x, int y, const std::wstring& image) const {
			int j{};
			int i{};
			for (const auto& w : image) {
				if (w == '\n') { ++j; i = 0; }
				else {
					cgu::console(i + x, j + y, w);
					++i;
				}
			}
		}


    protected:
        uint32_t                          _id;         // identification of object
        float                              _x;         // position
        float                              _y;         // position

        float                              _speed;     // speed of animation
        float                              _next{};    // next frame 
        int                                _size;      // element in _image.

    public:

        AnimatedSprite()
            :_image{}
            , _id{ 0 }
            , _dimX{}, _dimY{}
            , _x{}, _y{}
            , _speed{}
            , _size{}
        {}

        AnimatedSprite(const cgu::wstrTexture& text, float x = 10.f, float y = 10.f, float speed = 0.1f)
            :_id{}, _x{ x }, _y{ y }
            , _speed{speed}
        {
            init_sprite(text);
            _size = _image.size();
            get_rect(_dimX, _dimY);
        }

        // add one image frame
        void add_frame_image(const std::wstring& wstr) {
            _image.push_back(wstr);
            _size = _image.size();
        }

        // override this 
        void accept(ISpriteVisitor& v, float f) override {
            v.visit(this, f);
        }

        void set_id(int id) {
            _id = id;
        }

        void set_speed(float speed) {
            _speed = speed;
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


        void animate(float f) {
            _next += _speed * 60.f / f;
            int next = int(_next) % _size;
            draw(int(_x), int(_y), _image[next]);
        }

        void draw(float f) {
            draw(int(_x), int(_y), _image[int(_next) % _size]);
        }

    };


}