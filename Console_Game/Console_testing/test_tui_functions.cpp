#include <iostream>
#include "MyLib/Console_Library/text_user_interface.h"




int main() {

	cui::Message_Box msg;

	std::string str = "first name : Kadda\nlast name : Aoues\nAdress : 23 Bab ...\ncountry : Algeria\n";

	msg(30, 10, "Aoues Kadda", str);

	wait_;
	return 0;
}