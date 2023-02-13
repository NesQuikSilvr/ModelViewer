#pragma once

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <string>

#include "core.h"

#include <vector>
#include "Tokenizer.h"
#include "Triangle.h"
#include "Vertex.h"
#include "Skeleton.h"

class Skin {
public:
	Skin(Tokenizer* tokenizer, Skeleton* skel);
	~Skin();

	Skeleton* skel;
	std::vector<glm::mat4> world_mats;

	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;

	std::vector<glm::mat4> bindings;
	std::vector<Joint*> joints;

	//Draw info
	glm::mat4 model;
	glm::vec3 color;

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;

	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	bool load(Tokenizer* tokenizer);
	void update();
	void draw(const glm::mat4& viewProjMtx, GLuint shader);
	void Skin::displayData();
};