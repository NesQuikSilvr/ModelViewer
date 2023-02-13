#pragma once

#include <stdio.h>
#include "core.h"

#include <vector>
#include "Tokenizer.h"
#include "Key.h"

class Channel {
public:
	Channel();
	~Channel();

	enum Extrapolation { constant, linear, cycle, cycle_offset, bounce };
	Extrapolation extrap_in, extrap_out;
	std::vector<Key*> keys;

	float eval(float time);
};