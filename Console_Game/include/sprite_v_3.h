#pragma once
#include "Console_utility_1.hpp"
#include "wstring_utility.h"

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
//    Interface for sprite in general   
// 
//////////////////////////////////////////////////////////////////////////////////////////////////////////

    class ISprite {
    protected:
        std::wstring    _buffer;
        cgu::fPoint2d   _pos;
        int             _id;
    public:
        ISprite()
            :ISprite(L"")
        {}

        ISprite(const std::wstring buffer, int x = 10, int y = 10)
            : _buffer{ buffer }, _pos{ float(x),float(y) }
        {}

        virtual ~ISprite() = default;

        virtual void draw() const = 0;   // we don't know how to draw it in screen buffer.

        virtual cgu::iRect    get_bounds() const = 0;
        virtual cgu::fPoint2d get_dimension() const = 0;

        wchar_t operator[](int n) const {
            return _buffer[n];
        }

        cgu::fPoint2d get_position() const {
            return _pos;
        }
        void set_position(const cgu::fPoint2d& pos) {
            _pos = pos;
        }

        void set_id(int id) {
            _id = id;
        }

        int get_id() const {
            return _id;
        }

    };


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

}