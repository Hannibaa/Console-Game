#pragma once
#include "shape_v_1.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//           General Class for sprite 
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace cgu {

    int _rot(int n, int lx) {
        int k = n % lx;
        return lx * (lx - 1) - lx * k + (n - k) / lx;
    }

    int _rot90d(int N, int lx, int ly) {
        int k = N % ly;
        return (N - k) / ly + (ly - 1 - k) * lx;
    }


    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //     CLASS SPRITE   
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::wstring  wstrTetrisPiece[7]
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
                                  
   // pre-dimension of buffer in 2 dimension 
    template<int _dimX, int _dimY> 
    class StaticSprite : public cgu::ISprite {

    public:
        StaticSprite()
            :StaticSprite(L"", 10.f, 10.f)
        {}

        StaticSprite(const std::wstring& text, int x, int y)
        {
            _buffer = text;
            _buffer.resize(_dimX * _dimY, L' ');
            set_position({ float(x), float(y) });
        }

        void set_buffer(const std::wstring new_text) {
            _buffer = new_text;
        }

		void rotate() {
            std::wstring str(_dimX*_dimY,L'.');
            for (int n = 0; n != _dimX * _dimY; ++n) {
                str[n] = _buffer[_rot90d(n, _dimX, _dimY)];
            }
            _buffer = str;
		}

        virtual cgu::iRect get_bounds() const override {
            return cgu::iRect{ int(_pos.x), int(_pos.y), _dimX, _dimY};
        }

        virtual cgu::fPoint2d get_dimension() const override {
            return cgu::fPoint2d{ _dimX, _dimY };
        }

        virtual void draw() const override {
            for(int i = 0; i != _dimX ; ++i)
                for (int j = 0; j != _dimY; ++j)
                {
                    cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * _dimX]);
                }
        }

    };

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
//    Dynamic sprite
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

    class DynSprite : public cgu::ISprite {
        int _dimX, _dimY;
    public:
        DynSprite()
            :DynSprite(L"",1,1, 10.f, 10.f)
        {}

        DynSprite(const std::wstring& text,int dimx, int dimy, int x, int y)
            :_dimX{dimx}
            ,_dimY{dimy}
        {
            if (_dimX < 1 || _dimY < 1) {
                std::cout << "Error";
                _dimX = 1;
                _dimY = 1;
            }

            _buffer = text;
            _buffer.resize(_dimX * _dimY, L' ');
            set_position({ float(x), float(y) });
        }

        void set_buffer(const std::wstring new_text) {
            _buffer = new_text;
        }

        void rotate() {
            std::wstring str(_dimX * _dimY, L' ');
            for (int n = 0; n != _dimX * _dimY; ++n) {
                str[n] = _buffer[_rot90d(n, _dimX, _dimY)];
            }
            _buffer = str;
        }

        virtual cgu::iRect get_bounds() const override {
            return cgu::iRect{ int(_pos.x), int(_pos.y), _dimX, _dimY };
        }

        virtual cgu::fPoint2d get_dimension() const override {
            return cgu::fPoint2d{ float(_dimX), float(_dimY) };
        }

        virtual void draw() const override {
            for (int i = 0; i != _dimX; ++i)
                for (int j = 0; j != _dimY; ++j)
                {
                    cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * _dimX]);
                }
        }

    };


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

            if (KeyReleased(_u('R'))) {
                ptr->rotate();
            }

            p.clamped({ 1.f, float(m_field.dx) - dim.x }, { 1.f , float(m_field.dy) - dim.y });

            ptr->set_position(p);
        }
    };

    class Collision {
    public:
        template<typename TSprite>
        bool is_collid(TSprite& t) {
            cgu::fPoint2d pt = t.get_position();
            cgu::fPoint2d dimt = t.get_dimension();

            for (int i = 0; i != dimt.x; ++i)
            {
                for (int j = 0; j != dimt.y; ++j)
                {
                    int n = i + j * dimt.x;
                    if (t[n] != L' ')
                    {
                        if (cgu::console[pt.x + i +(pt.y + j) * cgu::console.get_length()] != L' ')
                        {
                            return true;
                            break;
                        }
                    }
                }
            }

            return false;
        }
    };
}