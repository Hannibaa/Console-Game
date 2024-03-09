#pragma once
#include <string>

namespace cgu {

	namespace ascii {
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    structure for image text 
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

		struct AsciiImage {
			std::wstring          text;
			int                   dx;
			int                   dy;
		};

		void AsciiAjust(AsciiImage& txImg) {
			txImg.text.resize(txImg.dx * txImg.dy, L' ');
		}

		template<typename TConsole>
		void draw_image(const AsciiImage& tx, TConsole& console, int x, int y) {

			if (tx.text.size() > tx.dx * tx.dy) return;

			for (int i = 0; i != tx.dx; ++i)
				for (int j = 0; j != tx.dy; ++j)
					console(x + i, y + j, tx.text[i + j * tx.dx]);
		}

	}


}