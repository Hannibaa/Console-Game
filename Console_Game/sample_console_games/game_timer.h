#pragma once
#include <chrono>



class Game_Timer {
	std::chrono::steady_clock::time_point _start;
	std::chrono::steady_clock::time_point _start0;

public:
	Game_Timer()
		:_start{std::chrono::steady_clock::now()}
		, _start0(_start)
	{
		
	}

	float get_elapsed_time() {
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - _start).count();
	}

	void reset() {
		_start = std::chrono::steady_clock::now();
	}

	float time_since_begin() {
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - _start0).count();
	}
};