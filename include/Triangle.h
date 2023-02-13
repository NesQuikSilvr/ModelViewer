#pragma once

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "core.h"

#include <vector>
#include "Vertex.h"

class Triangle {
public:
	Triangle();
	Triangle(Vertex a, Vertex b, Vertex c);
	~Triangle();

	Vertex a;
	Vertex b;
	Vertex c;

	void displayData();
};