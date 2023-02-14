#include "Channel.h"

Channel::Channel() {
	Channel::extrap_in = constant;
	Channel::extrap_out = constant;
}

Channel::~Channel() {

}

/*
 * Return value of Channel at the given time
 * 4 cases: 
 * t is before the first key (use extrapolation)
 * t is after the last key (use extrapolation)
 * t falls exactly on some key (return key value)
 * t falls between two keys (evaluate cubic equation)
 * 
 */
float Channel::eval(float time) {
	if (Channel::keys.size() == 0) { return 0.0; }
	if (Channel::keys.size() == 1) { return Channel::keys[0]->value; }


	//Find the spanning keyframes surrounding given time, listed keys are assumedly ordered by time from file
	int mid = Channel::keys.size() / 2;
	int low = 0;
	int high = Channel::keys.size() - 1;
	Key* temp = Channel::keys[mid];

	while (low < high) {
		if		(temp->time < time) { low = mid + 1; }
		else if (temp->time > time) { high = mid - 1; }
		else						{ return temp->value; } //Time lands exactly on a keyframe

		mid = (high + low) / 2;
		temp = Channel::keys[mid];
	}

	//Specify previous and next keyframes
	Key* prev = NULL;
	Key* next = NULL;
	if (temp->time < time) {
		prev = temp;
		next = prev->next;
	}
	else if (temp->time > time) {
		next = temp;
		prev = next->prev;
	}
	else { return temp->value; }
	

	//Time is before first keyframe, use Channel extrap_in
	if (prev == NULL) {

		return -6.9;
	}

	//Time is after last keyframe, use Channel extrap_out
	if (next == NULL) {

		return 6.9;
	}

	//Time is between two keyframes
	switch (prev->tangent_out) {
		case Key::Tangent::flat:
			break;
		case Key::Tangent::linear:
			break;
		case Key::Tangent::smooth:
			break;
	}
	
	return 13.37;
}