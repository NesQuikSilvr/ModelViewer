#pragma once

#include <stdio.h>
#include "core.h"

#include <vector>
#include "Tokenizer.h"
#include "Channel.h"

class Animation {
public:
	Animation();
	~Animation();

	float time_start, time_end;
	int num_channels;
	std::vector<Channel*> channels;

	bool load(Tokenizer& tokenizer);
	void update(float time);
};