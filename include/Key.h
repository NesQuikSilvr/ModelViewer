#pragma once

#include <stdio.h>
#include "core.h"
#include <iostream>

#include <vector>
#include "Tokenizer.h"

class Key {
public:
	Key();
	~Key();

	float time, value;
	Key* prev;
	Key* next;

	enum Tangent { flat, linear, smooth };
	Tangent tangent_in, tangent_out;

	void displayData();
};