#include "Console_utility_1.hpp"




int main() {

	Console console(145, 35);

	int i{10}, j{10};

	while (true) {
		console.clear_screen();

		if (KeyReleased(VK_UP)) --j;
		if (KeyReleased(VK_DOWN)) ++j;
		if (KeyReleased(VK_LEFT)) --i;
		if (KeyReleased(VK_RIGHT)) ++i;




		console(i, j) = L'A';



		console.draw();
		
		// this for get out of infinit loop
		if (get_out()) break;
	}

	return 0;
}
