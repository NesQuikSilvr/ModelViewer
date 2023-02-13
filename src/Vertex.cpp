#pragma once

#include "Vertex.h"

Vertex::Vertex() {
	Vertex::setPosition(0, 0, 0);
	Vertex::setNormal(0, 0, 0);
}

Vertex::Vertex(float x, float y, float z) {
	Vertex::setPosition(x, y, z);
	Vertex::setNormal(0, 0, 0);
}

Vertex::~Vertex() {

}

void Vertex::setPosition(float x, float y, float z) {
	Vertex::position.x = x;
	Vertex::position.y = y;
	Vertex::position.z = z;
}

void Vertex::setNormal(float x, float y, float z) {
	Vertex::normal.x = x;
	Vertex::normal.y = y;
	Vertex::normal.z = z;
}

void Vertex::displayData() {
	std::cout << Vertex::position.x << " " << Vertex::position.y << " " << Vertex::position.z << std::endl;
}