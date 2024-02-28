#pragma once
#include "Console_utility_1.hpp"
#include "MyLib/geometry2d_utility.h"



namespace  cu {    // console object

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    Fire object
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Fire {

		float             _x, _y;
		float             _vx, _vy;
		wchar_t           glyph;
		bool             _isReady;

	public:
		Fire(float x, float y, wchar_t c = L'|')
			: _x{ x }, _y{ y }
			, _vx{ 0.f }
			, _vy{ -0.01f }
			, glyph{ c }
			, _isReady{false}
		{
		}

		auto get_position() const {
			return cu::fPoint2d{ _x,_y };
		}

		void set_fire_parameter(float vx, float vy, wchar_t _glyph = L'|') {
			_vx = vx;
			_vy = vy;
			glyph = _glyph;
		}

		bool move(int offsetX = 0, int offsetY = 0) {

			_y += _vy;
			_x += _vx;

			int x = static_cast<int>(_x);
			int y = static_cast<int>(_y);

			if (y < 1 || y > cu::console.get_width() - 1) { _isReady = false; }
			if (x < 1 || x > cu::console.get_length() - 1) { _isReady = false; }

			cu::console(x + offsetX, y + offsetY, glyph);
			cu::console(x + 1 + offsetX, y + offsetY, glyph);

			return false;
		}

		// fire is ready that mean is not move or is expired;
		bool& is_ready() {
			return _isReady;
		}

		void reset(float x, float y) {
			_x = x;
			_y = y;
		}

		~Fire() { }
	};




	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Sprite Object associated with console 
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	class Sprite {

		std::vector<std::wstring>          _vecImage;
		std::wstring                       temp_0, temp_2;
		std::wstring                       _shield_up, _shield_down;
		int                                _high;
		int                                _width;
		float                              _x;
		float                              _y;
		float                              _vx;
		float                              _vy;

		// Key controle setting.
		int                                _key_up;
		int                                _key_down;
		int                                _key_right;
		int                                _key_left;
		int                                _key_shield0;
		int                                _key_shield1;
		int                                _key_fire;

		// activation of shield and protection
		bool                               b_back_shield;
		bool                               b_side_shield;

		// vector of firing 
		std::vector<Fire>                  vFires;
		float                              _fire_vx;
		float                              _fire_vy;
		wchar_t                            _fire_glyph;

		// initialize image vector, high and width
		void Create(std::initializer_list<std::wstring> list) {
			_width = 0;
			for (const auto wstr : list) {
				_vecImage.push_back(wstr);
				_width = (_width > int(wstr.size())) ? _width : int(wstr.size());
			}

			_high = (int)_vecImage.size();
		}

		void draw(int x, int y) const {
			int j{};
			for (const auto& wstr : _vecImage) {
				int i{};
				for (const auto& wchar : wstr) {
					cu::console(i + x, j + y, wchar);
					++i;
				}
				++j;
			}
		}

	public:
		void draw() const {
			draw(_x, _y);
		}

	public:
		Sprite(std::initializer_list<std::wstring> list, float x = 10.f, float y = 10.f)
			:_x{ x }, _y{ y }
			, _vx{ 0.01f }, _vy{ 0.01f }
			, b_back_shield{ false }
			, b_side_shield{ false }
			, _shield_down{}, _shield_up{}
		{
			if (list.size() == 3) {
				Create(list);
				temp_0 = _vecImage[0];
				temp_2 = _vecImage[2];
			}
			else {
				std::terminate();
			}

		}

		// this methode should be initialized after set fire parameter.
		void create_fires() {
			_key_fire = VK_SPACE;

			vFires.reserve(45);
			// create vector of fire of 40 object
			for (int i = 0; i < 40; ++i) {
				vFires.emplace_back(_x, _y);
				vFires[vFires.size() - 1].set_fire_parameter(_fire_vx, _fire_vy, _fire_glyph);
			}
		}

		// get bound 
		cu::iRect get_bounds() {
			return cu::iRect(_x, _y, _width, _high);
		}

		auto getX() const {
			return _x;
		}

		auto getY() const {
			return _y;
		}

		auto get_position() const {
			return cu::fPoint2d{ _x, _y };
		}

		void set_position(float x, float y) {
			_x = x; _y = y;
		}

		void set_controle_key(int up = VK_UP, int down = VK_DOWN, 
			                  int left = VK_LEFT , int right = VK_RIGHT
		                     )
		{
			_key_up = up;
			_key_down = down;
			_key_left = left;
			_key_right = right;
		}

		void set_option_key(int shield0, int shield1, int firing) {
			_key_shield0 = shield0;
			_key_shield1 = shield1;
			_key_fire = firing;
		}

		void set_shield(const std::wstring_view shieldUp, const std::wstring_view shieldDown)
		{
			_shield_down = shieldDown;
			_shield_up = shieldUp;
		}

		void set_fire_parameter(float vx = 0.f, float vy = - 0.01f, wchar_t _glyph = L'|') {
			_fire_vx = vx;
			_fire_vy = vy;
			_fire_glyph = _glyph;
		}

		void move() {
			int x{}, y{};

			if (KeyPressed(_key_up))     _y -= _vy;
			if (KeyPressed(_key_down))   _y += _vy;
			if (KeyPressed(_key_left))   _x -= _vx;
			if (KeyPressed(_key_right))  _x += _vx;

			if (KeyReleased(_key_shield0)) b_side_shield = !b_side_shield;
			if (KeyReleased(_key_shield1)) b_back_shield = !b_back_shield;

			if (b_back_shield) {
				_vecImage[0] = _shield_down; // L"\x25ac\x25ac\x25ac\x25ac\x25ac";
			}
			else {
				_vecImage[0] = temp_0;
			}

			if (b_side_shield) {
				_vecImage[2] = _shield_up; // L"-----";
			}
			else {
				_vecImage[2] = temp_2;
			}

			_x = std::clamp(_x, 1.f, float(cu::console.get_length() - _width - 1));
			_y = std::clamp(_y, 1.f, float(cu::console.get_width() - _high -1 ));

			/*x = static_cast<int>(_x);
			y = static_cast<int>(_y);

			draw(x, y);*/
		}

		// get vector coordinate of firing
		auto& get_vector_firing_coord() {
			return vFires;
		}

		// firing function 
		void firing() {
			if (KeyReleased(_key_fire)) {
				for (auto& f : vFires) {
					if (!f.is_ready()) {
						f.is_ready() = true;
						f.reset(_x, _y);
						break;
					}
				}
			}

			for (auto& f : vFires)
				if (f.is_ready()) f.move(2,1);
		}

	};


}