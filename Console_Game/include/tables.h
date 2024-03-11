#pragma once
#include <algorithm>
#include <ranges>
#include "sprite_v_3.h"

namespace cgu {

	namespace table {

		struct Cell {
			std::wstring        s_text;
			cgu::iPoint2d       s_pos;
			cgu::iPoint2d       s_dim;
		};

		void draw_cell(const Cell& cell) {
			for (int i = 0; i < cell.s_dim.x; ++i) {
				for (int j = 0; j < cell.s_dim.y; ++j) {
					cgu::console(i + cell.s_pos.x, j + cell.s_pos.y, cell.s_text[i + j * cell.s_dim.x]);
				}
			}
		}

		void resize_cell(Cell& cell) {
			cell.s_text.resize(cell.s_dim.x * cell.s_dim.y, L' ');
		}

		using VecCells = std::vector<Cell>;

		void make_cells(const std::vector<int>& vecx, const std::vector<int>& vecy, VecCells& vecCells)
		{
			// resize vecCells
			vecCells.reserve((vecx.size() - 1) * (vecy.size() - 1) + 5);

			for (int i = 0; i < vecx.size() - 1; ++i) {
				for (int j = 0; j < vecy.size() - 1; ++j) {
					Cell cell;
					cell.s_pos.x = vecx[i] + 1;
					cell.s_pos.y = vecy[j] + 1;
					cell.s_dim.x = vecx[i + 1] - vecx[i] - 1;
					cell.s_dim.y = vecy[j + 1] - vecy[j] - 1;
					resize_cell(cell);
					vecCells.push_back(cell);
				}
			}
		}

		class Table : public cgu::ISprite {
			std::vector<int>     t_x;
			std::vector<int>     t_y;
			VecCells             t_cells;
			int                  t_min_space{2};
			int                  t_sizeX;
			int                  t_sizeY;

			int                  t_lx;
			int                  t_ly;

			void sort() {
				// sorting t_x and t_y
				std::sort(t_x.begin(), t_x.end());
				std::sort(t_x.begin(), t_x.end());
			}

			void check_equidistance(std::vector<int>& vec, int min) {
				for (int i = 0; i < vec.size() - 1; ++i) {
					if (vec[i + 1] - vec[i] < min) {
						vec[i + 1] = vec[i] + min;
					}
				}
			}

			void make_table() {
				// Lines horizontal
				for (int k = 0; k != t_sizeY; ++k) {
					for (int x = 0; x != t_lx; ++x) {
						_buffer[x + (t_y[k] - t_y[0]) * t_lx] = cgu::whLine;
					}
				}
				// Lines verticales
				for (int k = 0; k != t_sizeX; ++k) {
					for (int y = 0; y != t_ly; ++y) {
						_buffer[t_x[k] - t_x[0] + y * t_lx] = cgu::wvLine;
					}
				}

				// Corner
				_buffer[(0)                         + (0)*t_lx]          = wBox00;
				_buffer[(t_lx - 1) + (0) * t_lx]                         = wBox01;
				_buffer[(0)                         + (t_ly - 1) * t_lx] = wBox10;
				_buffer[(t_lx - 1) + (t_ly - 1) * t_lx]                  = wBox11;

				// Tee
				for (int k = 1; k < t_sizeX - 1; ++k) {
					_buffer[(t_x[k] - t_x[0]) + (0) * t_lx]        = wT00;
					_buffer[(t_x[k] - t_x[0]) + (t_ly - 1) * t_lx] = wT11;
				}

				for (int k = 1; k < t_sizeY - 1; ++k) {
					_buffer[(0) + (t_y[k] - t_y[0]) * t_lx] = wT10;
					_buffer[(t_lx - 1) + (t_y[k] - t_y[0]) * t_lx] = wT01;
				}

				// X cross
				for (int i = 1; i < t_sizeX - 1; ++i) {
					for (int j = 1; j < t_sizeY - 1; ++j) {
						_buffer[(t_x[i] - t_x[0]) + (t_y[j] - t_y[0]) * t_lx] = wX;
					}
				}
			}

		public:
			Table(const std::vector<int>& vecX, const std::vector<int>& vecY)
				: t_x{ vecX },
				  t_y{ vecY }
			{
				// size of vector should be more than one
				if (t_x.size() < 2)
					t_x.push_back(t_x[0] + t_min_space);

				if (t_y.size() < 2)
					t_y.push_back(t_y[0] + t_min_space);

				// sorting these vector
				sort();

				// set size 
				t_sizeX = t_x.size();
				t_sizeY = t_y.size();

				// check equidistance
				check_equidistance(t_x, t_min_space);
				check_equidistance(t_y, t_min_space);

				// set position 
				set_position({(float)t_x[0] , (float)t_y[0] });

				// set size of buffer 
				_buffer.resize((t_x[t_sizeX - 1] - t_x[0] + 1) * (t_y[t_sizeY - 1] - t_y[0] + 1),
					L' ');

				 t_lx = t_x[t_sizeX - 1] - t_x[0] + 1;
				 t_ly = t_y[t_sizeY - 1] - t_y[0] + 1;

				 // construct table
				 make_table();
				 make_cells(t_x, t_y, t_cells);
			}

			// set text in cell 
			void set_cell(int x, int y, const std::wstring text) {
				t_cells[x + y * (t_sizeX - 1)].s_text = text;
				resize_cell(t_cells[x + y * (t_sizeX - 1)]);
			}

			// draw function
			virtual void draw() const override {
				for (int i = 0; i != t_lx; ++i)
					for (int j = 0; j != t_ly; ++j)
					{
						cgu::console(i + _pos.x, j + _pos.y, _buffer[i + j * t_lx]);
					}

				for (const auto& c : t_cells) draw_cell(c);
			}

			virtual cgu::iRect get_bounds() const {
				return cgu::iRect{ int(_pos.x), int(_pos.y) , t_lx, t_ly };
			}

			virtual cgu::fPoint2d get_dimension() const {
				return cgu::fPoint2d{ float(t_lx), float(t_ly) };
			}

		};





	} // namespace table


} // namespace table