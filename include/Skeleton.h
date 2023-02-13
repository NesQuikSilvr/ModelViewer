/* A Skeleton is represented by a tree/hierarchy of connected Joints */
#pragma once

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "core.h"
#include "Joint.h"
#include <vector>


class Skeleton {
public:
	Skeleton();
	Skeleton(const char* filepath, const char* name);
	~Skeleton();

	std::vector<Joint*> joints;

	void load(const char* filepath);
	void display();
	void update();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Skeleton::listJoints(Joint* joint);

	void setName(const char* name);
	const char* getName();

	void setRoot(Joint* joint);
	Joint* getRoot();

private:
	const char* name;
	Joint* root;
};