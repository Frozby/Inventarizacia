#include "Tools.h"

namespace Tools {

	bool isintparsable(std::string& input) {
		if (!all_of(input.begin(), input.end(), ::isdigit)) {
			std::cout << "zly vstup, vstup musi byt cislo\n";
			return false;
		}
		return true;
	}

}
