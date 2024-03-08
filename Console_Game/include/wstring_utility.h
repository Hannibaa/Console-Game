#pragma once
#include <iostream>
#include <sstream>
#include <utility>
#include <string>
#include <vector>

/*
           Library for wstring function utility
           Only Function associated with wstring only
           under namespace Str
*/

namespace Str {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 
    //    get the dimention of boxes that can contain this text with return to line rl
    // 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
#define  _GET_BOUNDS     Str::get_boxed_text_dimension
	std::pair<int, int>  get_boxed_text_dimension(const std::wstring& text, wchar_t rl = L'\n') {

		int max = 0;
		int lines = 0;

		for (int i = 0; i < text.size(); ++i) {
			int count{};
			while (text[i] != rl) { ++count; ++i; if (i >= text.size()) break; }
			max = (count >= max) ? count : max;
			++lines;
		}

		return std::pair{ max, lines };

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Function make vector of wstring for boxing drawing
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	void make_dim_boxes(std::vector<std::wstring>& wstr, int l, int h) {
		if (l < 2) {
			std::cout << "Error : l > 2 \n";
			return;
		}

		wstr.resize(h, L"");

		std::wstring wstr_lines(l - 2, L'\x2501');
		std::wstring wstr_spaces(l - 2, L' ');

		wstr[0] = L"\x250f" + wstr_lines + L"\x2513";

		for (int j = 1; j < h - 1; ++j)
			wstr[j] = L"\x2503" + wstr_spaces + L"\x2503";

		wstr[h - 1] = L"\x2517" + wstr_lines + L"\x251b";
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    convert vector of string to texture and string to texture.
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	void convert_to_texture(const std::wstring& text, std::vector<std::wstring>& texture) {
		std::wstringstream wss;
		wss.str(text);
		std::wstring line;

		while (std::getline(wss, line)) {
			texture.push_back(line);
		}
	}

	// Inverse function :
	void convert_to_wstring(const std::vector<std::wstring>& texture, std::wstring text) {
		for (const auto& str : texture) {
			text += str;
			text.push_back(L'\n');
		}
	}

	// function return bool test a text is clean of char c = '\n'
    // return yes if there no char c in text otherwise return flase :
	bool is_clean_of_char(const std::wstring& text, char c = '\n') {
		for (const auto& Char : text)
			if (Char == c) return false;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 
	//    Function repace all cooresponding value to old_value by new_value
	//    any container that equiped with operator[] and value_type with operator
	//    = and == ;
	// 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename Container, typename T = Container::value_type >
	void replace_all_by(Container& container, T old_value, T new_value) {

		for (auto& c : container) {
			if (c == old_value) c = new_value;
		}

	}


}