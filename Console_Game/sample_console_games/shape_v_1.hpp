#pragma once
/*
       Generale sprite header file
 
 */

#include "Console_utility_1.hpp"
#include "wstring_utility.h"



namespace   cgu {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Interface for sprite in general   
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	class ISprite {
	protected:
		std::wstring    _buffer;
		cgu::fPoint2d   _pos;
	public:
		ISprite()
			:ISprite(L"")
		{}

		ISprite(const std::wstring buffer, int x = 10 , int y = 10)
			: _buffer{buffer}, _pos{float(x),float(y)}
		{}

		virtual ~ISprite() = default;

		virtual void draw() const = 0;   // we don't know how to draw it in screen buffer.

		virtual cgu::iRect    get_bounds() = 0;
		virtual cgu::fPoint2d get_dimension() = 0;

		cgu::fPoint2d get_position() const {
			return _pos;
		}
		void set_position(const cgu::fPoint2d& pos) {
			_pos = pos;
		}

	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Sprite of tetromino
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<int dimX, int dimY>
	class Box : public ISprite {
		static_assert(dimX > 0 && dimY > 0, "dimension should be positive");

		void init_buffer() {
			if (dimX == 1 && dimY == 1)
				_buffer[0] = _glyph;
			else {
				for (int i = 0; i != _dimX; ++i)
				{
					_buffer[i] = _glyph;
					_buffer[i + (_dimY - 1) * dimX] = _glyph;
				}

				for (int i = 1; i != _dimY - 1; ++i) {
					_buffer[i * _dimX] = _glyph;
					_buffer[_dimX - 1 + i * _dimX] = _glyph;
				}
			}
		}
	public:
		Box() : Box(10, 10){}

		Box(int x, int y)
			: ISprite(std::wstring(dimX * dimY, L' '),x,y)
		{
			init_buffer();
		}

		void set_glyph(wchar_t c) {
			_glyph = c;
			init_buffer();
		}

		void set_id(size_t id) {
			_id = id;
		}

		virtual void draw() const override {
			for (int i = 0; i != _dimX; ++i)
				for (int j = 0; j != _dimY; ++j)
				{
					if (_buffer[i + j * _dimX] != L' ')
						cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * _dimX]);
				}

		}

		virtual cgu::iRect    get_bounds() override {
			return cgu::iRect{ int(_pos.x), int(_pos.y), _dimX, _dimY };
		}

		virtual cgu::fPoint2d get_dimension() override {
			return cgu::fPoint2d{ float(_dimX), float(_dimY) };
		}


	private:
		size_t   _id;
		wchar_t  _glyph{ 0x2588 };

		int      _dimX{ dimX };
		int      _dimY{ dimY };
	};

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    line boxes : dimension define at compile time
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<int dimX, int dimY>
	class LineBox : public ISprite {
		static_assert(dimX > 1 && dimY > 1, "dimension should be positive");

		void init_buffer() {
			if (dimX == 2 && dimY == 2) {
				_buffer[0] = _glyphs[0];
				_buffer[1] = _glyphs[2];
				_buffer[2] = _glyphs[4];
				_buffer[3] = _glyphs[5];
			}
			else {
				for (int i = 1; i != _dimX - 1; ++i)
				{
					_buffer[i]                      = _glyphs[1];
					_buffer[i + (_dimY - 1) * dimX] = _glyphs[1];
				}

				for (int i = 1; i != _dimY - 1; ++i) {
					_buffer[i * _dimX]             = _glyphs[3];
					_buffer[_dimX - 1 + i * _dimX] = _glyphs[3];
				}

				// set corner
				_buffer[0]                               = _glyphs[0];
				_buffer[_dimX - 1]                       = _glyphs[2];
				_buffer[(_dimY - 1) * _dimX]             = _glyphs[4];
				_buffer[_dimX - 1 + (_dimY - 1) * _dimX] = _glyphs[5];
			}
		}
	public:
		LineBox() : LineBox(10, 10) {}

		LineBox(int x, int y)
			: ISprite(std::wstring(dimX* dimY, L' '), x, y)
		{
			init_buffer();
		}

		void set_id(size_t id) {
			_id = id;
		}

		// todo 
		void set_glyphes(const std::wstring& set_fo_glyphes) {

		}

		virtual void draw() const override {
			for (int i = 0; i != _dimX; ++i)
				for (int j = 0; j != _dimY; ++j)
				{
					if (_buffer[i + j * _dimX] != L' ')
						cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * _dimX]);
				}

		}

		virtual cgu::iRect    get_bounds() override {
			return cgu::iRect{ int(_pos.x), int(_pos.y), _dimX, _dimY };
		}

		virtual cgu::fPoint2d get_dimension() override {
			return cgu::fPoint2d{ float(_dimX), float(_dimY) };
		}


	private:
		size_t   _id;
		wchar_t  _glyphs[6]{
			cgu::wBox00,       //    0
			cgu::whLine,       //    1
			cgu::wBox01,       //    2
			cgu::wvLine,       //    3
			cgu::wBox10,       //    4
			cgu::wBox11	       //    5
		 };
		int      _dimX{ dimX };
		int      _dimY{ dimY };
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Dynamic LinedBox  define dimension what ever
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////

	class DynLineBox : public ISprite {

		void init_buffer() {
			if (_dimX == 2 && _dimY == 2) {
				_buffer[0] = _glyphs[0];
				_buffer[1] = _glyphs[2];
				_buffer[2] = _glyphs[4];
				_buffer[3] = _glyphs[5];
			}
			else {
				for (int i = 1; i != _dimX - 1; ++i)
				{
					_buffer[i] = _glyphs[1];
					_buffer[i + (_dimY - 1) * _dimX] = _glyphs[1];
				}

				for (int i = 1; i != _dimY - 1; ++i) {
					_buffer[i * _dimX] = _glyphs[3];
					_buffer[_dimX - 1 + i * _dimX] = _glyphs[3];
				}

				// set corner
				_buffer[0] = _glyphs[0];
				_buffer[_dimX - 1] = _glyphs[2];
				_buffer[(_dimY - 1) * _dimX] = _glyphs[4];
				_buffer[_dimX - 1 + (_dimY - 1) * _dimX] = _glyphs[5];
			}
		}
	public:
		DynLineBox() 
			:DynLineBox{ 2,2, 10, 10 }
		{}

		DynLineBox(int x, int y)
			:DynLineBox{2,2, x, y}
		{}

		DynLineBox(int dimX, int dimY, int x, int y)
			: _dimX{dimX}
			, _dimY{dimY}
		{
			if (_dimX < 2)  dimX = 2;
			if (_dimY < 2)  dimY = 2;
			_buffer.resize(_dimX * _dimY, L' ');
			set_position({ float(x), float(y) });
			init_buffer();
		}

		void set_id(size_t id) {
			_id = id;
		}

		// todo 
		void set_glyphes(const std::wstring& set_fo_glyphes) {

		}

		virtual void draw() const override {
			for (int i = 0; i != _dimX; ++i)
				for (int j = 0; j != _dimY; ++j)
				{
					if (_buffer[i + j * _dimX] != L' ')
						cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * _dimX]);
				}

		}

		virtual cgu::iRect    get_bounds() override {
			return cgu::iRect{ int(_pos.x), int(_pos.y), _dimX, _dimY };
		}

		virtual cgu::fPoint2d get_dimension() override {
			return cgu::fPoint2d{ float(_dimX), float(_dimY) };
		}


	private:
		size_t   _id;
		wchar_t  _glyphs[6]{
			cgu::wBox00,       //    0
			cgu::whLine,       //    1
			cgu::wBox01,       //    2
			cgu::wvLine,       //    3
			cgu::wBox10,       //    4
			cgu::wBox11	       //    5
		};
		int      _dimX;
		int      _dimY;
	};

	// Dynamic Box
	class DynBox : public ISprite {

		void init_buffer() {
			if (_dimX == 1 && _dimY == 1)
				_buffer[0] = _glyph;
			else {
				for (int i = 0; i != _dimX; ++i)
				{
					_buffer[i] = _glyph;
					_buffer[i + (_dimY - 1) * _dimX] = _glyph;
				}

				for (int i = 1; i != _dimY - 1; ++i) {
					_buffer[i * _dimX] = _glyph;
					_buffer[_dimX - 1 + i * _dimX] = _glyph;
				}
			}
		}
	public:
		DynBox()
			:DynBox(1,1,10,10)
		{}

		DynBox(int dimX, int dimY, int x, int y)
			: _dimX{dimX}
			, _dimY{dimY}
		{
			if (_dimX < 1) _dimX = 1;
			if (_dimY < 1) _dimY = 1;

			set_position({ float(x), float(y) });
			init_buffer();
		}

		void set_glyph(wchar_t c) {
			_glyph = c;
			init_buffer();
		}

		void set_id(size_t id) {
			_id = id;
		}

		virtual void draw() const override {
			for (int i = 0; i != _dimX; ++i)
				for (int j = 0; j != _dimY; ++j)
				{
					if (_buffer[i + j * _dimX] != L' ')
						cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * _dimX]);
				}

		}

		virtual cgu::iRect    get_bounds() override {
			return cgu::iRect{ int(_pos.x), int(_pos.y), _dimX, _dimY };
		}

		virtual cgu::fPoint2d get_dimension() override {
			return cgu::fPoint2d{ float(_dimX), float(_dimY) };
		}


	private:
		size_t   _id;
		wchar_t  _glyph{ 0x2588 };

		int      _dimX;
		int      _dimY;
	};

}

