#pragma once

#include "core.h"
#include "Triangle.h"

Triangle::Triangle() {
	Triangle::a;
	Triangle::b;
	Triangle::c;
}

Triangle::Triangle(Vertex a, Vertex b, Vertex c) {
	Triangle::a = a;
	Triangle::b = b;
	Triangle::c = c;
}

Triangle::~Triangle() {

}

void Triangle::displayData() {
	Triangle::a.displayData();
	Triangle::b.displayData();
	Triangle::c.displayData();
}