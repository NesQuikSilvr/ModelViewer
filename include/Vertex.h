#pragma once

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "core.h"
#include "Joint.h"

#include <vector>

class Vertex {
public:
	Vertex();
	Vertex(float x, float y, float z);
	~Vertex();

	glm::vec3 position;
	glm::vec3 normal;
	std::vector<std::pair<int, float>> weights;

	void setPosition(float x, float y, float z);
	void setNormal(float x, float y, float z);
	void displayData();
};