#pragma once

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "core.h"

#include <vector>
#include "Tokenizer.h"

/*
offset	    	x y z	    	(joint offset vector)
boxmin	    	x y z	    	(min corner of box to draw)
boxmax	    	x y z	    	(max corner of box to draw)
rotxlimit	    min max	    	(x rotation DOF limits)
rotylimit	    min max	    	(y rotation DOF limits)
rotzlimit	    min max	    	(z rotation DOF limits)
pose	    	x y z	    	(values to pose DOFs)
balljoint	    name { }	    (child joint)
*/

class Joint {
public:
	Joint(const char* name);
	~Joint();

	const char* name;

	glm::mat4 local_matrix;
	glm::mat4 world_matrix;

	glm::vec3 offset;
	glm::vec3 boxmin;
	glm::vec3 boxmax;

	glm::vec2 rotxlimit;
	glm::vec2 rotylimit;
	glm::vec2 rotzlimit;
	
	glm::vec3 pose;

	Joint* parent;
	std::vector<Joint*> children;

	void load(Tokenizer& tokenizer);
	void update();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void recurseDraw(const glm::mat4& viewProjMtx, GLuint shader);
	void Joint::displayData();
	void Joint::recurseDisplayData();
};