#include <iostream>
#include "BoolVector.h"
#include "Enum.h"


int main() {
	try {
	}
	catch (Mistakes mistake) {
		switch (mistake) {
		case Invalid_size:
			std::cout << "Invalid_size";
			exit(0);
		case Invalid_index:
			std::cout << "Invalid_index";
			exit(1);
		default:
			std::cout << "Something went wrong";
			exit(-1);
		}
	}
	return 0;
}