#include <windows.h>
#include <iostream>

#define end_               '\n'


int main() {

	UINT  old_code{}, new_cp{437};
_one_more:
	std::cin >> new_cp;

	old_code = GetConsoleOutputCP();

	std::cout << "code page : " << old_code << end_;

	BOOL b_cp = SetConsoleOutputCP(new_cp);

	std::cout << "hello world" << end_;
	std::cin.get();
	std::cout << "current code page : " << GetConsoleOutputCP() << end_;
	std::cout << "------------------------------------------------";
	std::cout << "try one more : type y ";
	char y{};
	std::cin >> y;
	if (y == 'y' || y == 'Y') goto _one_more;

	SetConsoleOutputCP(old_code);

	return 0;
}