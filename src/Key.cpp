#include "Key.h"

Key::Key() {
	Key::time = 0.0;
	Key::value = 0.0;
	Key::tangent_in = flat;
	Key::tangent_out = flat;
	Key::prev = NULL;
	Key::next = NULL;
}

Key::~Key() {

}

void Key::displayData() {
	std::cout << Key::time << " " << Key::value << " " << std::endl;
}