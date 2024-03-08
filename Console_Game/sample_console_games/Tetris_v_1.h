#pragma once
#include "shape_v_1.hpp"


namespace tetris {


    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //     CLASS SPRITE   
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    const std::wstring  wstrTetrisPiece[7]
                          = { L"..X."
                              L"..X."
                              L"..X."
                              L"..X.",
                              
                              L"...."
                              L".XX."
                              L".XX."
                              L"....",

                              L"..X."
                              L".XX."
                              L".X.."
                              L"....",

                              L".X.."
                              L".XX."
                              L"..X."
                              L"....",

                              L".X.."
                              L"XXX."
                              L"...."
                              L"....",

                              L"...."
                              L"XX.."
                              L".X.."
                              L".X..",

                              L"...."
                              L".XX."
                              L".X.."
                              L".X.."
    };
                                  
    template<int _dimX, int _dimY> // pre-dimension of buffer in 2 dimension 
    class Sprite {

    protected:
        std::wstring                      _buffer;     // tetromino 

        float                              _x;         // position
        float                              _y;         // position

        wchar_t                            _glyph{ L'\x2588'};      // new glyph

        void init_tetris(const std::wstring& text) {
            _buffer = text;
            // checking for some 
            if (_buffer.size() < _dimX * _dimY - 1) {
                std::cout << "Error in string size : " << _buffer.size();
                std::terminate();
            }
            replace(_glyph);
        }

        // retation formulas
        int _rot(int n, int l) {
            int k = n % l;
            return l*(l - 1) - l * k + (n - k) / l;
        }

    public:

        Sprite()
            :_buffer{_dimX * _dimY, L' '}
            , _x{}, _y{}
        {}

        Sprite(const std::wstring& text, float x = 10.f, float y = 10.f)
            : _x{ x }, _y{ y }
             , _buffer{text}
        {
            // checking for some 
            if (_buffer.size() < _dimX * _dimY - 1 ) {
                std::cout << "Error in string size : " << _buffer.size();
                std::terminate();
            }
        }

		void replace(wchar_t c = L'\x2588') {
				Str::replace_all_by(_buffer, L'X', c);
		}

        void change(const std::wstring new_text) {
            _buffer = new_text;
            replace();
        }

        // coordinate of active cell


		// rotata tetromino
		void rotate() {
            std::wstring str(_dimX*_dimY,L'.');
            for (int n = 0; n != _dimX * _dimY; ++n) {
                str[n] = _buffer[_rot(n, _dimX)];
            }
            _buffer = str;
		}

        cgu::fPoint2d get_position() const {
            return cgu::fPoint2d{ _x,_y };
        }

        cgu::iPoint2d get_dimension() const {
            return cgu::iPoint2d{ _dimX, _dimY };
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
            return cgu::iRect{ int(_x), int(_y), _dimX, _dimY};
        }

        void draw(int x, int y, wchar_t c = L'.') const {
            for(int i = 0; i != _dimX ; ++i)
                for (int j = 0; j != _dimY; ++j)
                {
                    if (_buffer[i + j * _dimX] != c)
                    cgu::console(i + x, j + y, _buffer[i + j * _dimX]);
                }
        }

        void draw() const {
            draw(_x, _y);
        }
    };


    class Field : public Sprite<40,34> {
    public:
		Field(int x, int y)
			:Sprite<40, 34>(std::wstring(40 * 34, L' '), float(x), float(y))
		{
			for (int y = 0; y < 33; ++y) {
				for (int x = 0; x < 2; ++x)
					_buffer[x*40 + y * 40] = _glyph;
			}

            for (int x = 0; x < 40; ++x)
                _buffer[x + 33 * 40] = _glyph;
		}

		wchar_t operator[](int n) const {
            if ( n < _buffer.size() )
            return _buffer[n];
		}
    };

    using tetromino = Sprite<4, 4>;
    using field     = Field;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Motion definition
    //    
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

    class Motion {
        cgu::fPoint2d   m_speed;
        cgu::iRect      m_field;
    public:
        Motion(cgu::fPoint2d speed = {1.f, 1.f}
               ,cgu::iRect   field = {1, 1 , 144, 34 })
            :m_speed{speed}
            ,m_field{field}
        {}

        template<typename T>
        void operator() (T* ptr, float f) {
            cgu::fPoint2d  p = ptr->get_position();
            cgu::fPoint2d  dim = ptr->get_dimension();
            //if (KeyPressed(VK_DOWN)) {
            //    p.y += m_speed.y * f;
            //}
             p.y += f * m_speed.y / 2.5f;

            if (KeyPressed(VK_UP)) {
                p.y -= m_speed.y * f;
            }

            if (KeyPressed(VK_LEFT)) {
                p.x -= m_speed.x * f;
            }

            if (KeyPressed(VK_RIGHT)) {
                p.x += m_speed.x * f;
            }

            // ajust speed 
            if (KeyReleased(_u('U'))) {
                m_speed.x += 0.1f;
                m_speed.y += 0.1f;
            }
            if (KeyReleased(_u('D'))) {
                m_speed.x -= 0.1f;
                m_speed.y -= 0.1f;
            }

            p.clamped({ 1.f, float(m_field.dx) - dim.x }, { 1.f , float(m_field.dy) - dim.y });

            ptr->set_position(p);
        }
    };

    class Collision {
    public:

        bool is_collid(tetromino& t, field& f) {
            cgu::fPoint2d pt = t.get_position();
            cgu::iPoint2d dimt = t.get_dimension();
            cgu::iPoint2d dimf = f.get_dimension();

            for (int i = 0; i != dimt.x; ++i)
            {
                for (int j = 0; j != dimt.x; ++j)
                {
                    int n = pt.x + pt.y * dimf.x;
                    if (n < dimf.x * dimf.y)
                    {
                        if (f[n] != L' ') {
                            return true;
                        }
                    }
                }
            }

            return false;
        }
    };
}