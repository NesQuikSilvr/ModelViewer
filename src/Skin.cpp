#include "Skin.h"
#include "Tokenizer.h"
#include <glm/gtx/string_cast.hpp>

Skin::Skin(Tokenizer* tokenizer, Skeleton* skel) {
	Skin::skel = skel;
	Skin::load(tokenizer);
	model = glm::mat4(1.0f);
	color = glm::vec3(0.1f, 0.1f, 0.1f);
}

Skin::~Skin() {

}

bool Skin::load(Tokenizer* tokenizer) {

	if (Skin::skel != NULL) {
		Skin::joints = Skin::skel->joints;
	}

	/* Parse .Skin file */

	int count = 0;

	//
	//Store xyz positions as Vertices and as raw vec3's in Skin
	tokenizer->FindToken("positions");
	count = tokenizer->GetInt();

	tokenizer->FindToken("{");
	for (int i = 0; i < count; i++) {
		float x = tokenizer->GetFloat();
		float y = tokenizer->GetFloat();
		float z = tokenizer->GetFloat();

		Vertex vertex(x, y, z);
		Skin::vertices.push_back(vertex);
		Skin::positions.push_back(glm::vec3(x, y, z));
	}

	//
	//Store xyz normal vecs in Vertices and as raw vec3's in Skin
	tokenizer->FindToken("normals");
	count = tokenizer->GetInt();

	tokenizer->FindToken("{");
	for (int i = 0; i < count; i++) {
		float x = tokenizer->GetFloat();
		float y = tokenizer->GetFloat();
		float z = tokenizer->GetFloat();

		Skin::vertices[i].setNormal(x, y, z);
		Skin::normals.push_back(glm::vec3(x, y, z));
	}

	//
	//For each Vertex, store a weight for each Joint in the form of a mapping <joint_index, weight>
	//In the form: {# of weights} {index of joint} {weight value corresponding to joint} ... {index} {weight}
	tokenizer->FindToken("skinweights");
	count = tokenizer->GetInt();

	tokenizer->FindToken("{");
	int num_weights = 0;
	for (int i = 0; i < count; i++) {
		num_weights = tokenizer->GetInt();

		int joint_index = 0;
		float weight = 0.0;
		for (int j = 0; j < num_weights; j++) {
			joint_index = tokenizer->GetInt();
			weight = tokenizer->GetFloat();
			std::pair<int, float> attachment(joint_index, weight);

			Skin::vertices[i].weights.push_back(attachment);
		}
	}

	//
	//Store each Triangle as an object and as raw indices in Skin
	tokenizer->FindToken("triangles");
	count = tokenizer->GetInt();

	tokenizer->FindToken("{");
	for (int i = 0; i < count; i++) {
		float x = tokenizer->GetInt();
		float y = tokenizer->GetInt();
		float z = tokenizer->GetInt();

		Skin::indices.push_back(x);
		Skin::indices.push_back(y);
		Skin::indices.push_back(z);

		Vertex va = Skin::vertices[x];
		Vertex vb = Skin::vertices[y];
		Vertex vc = Skin::vertices[z];

		Triangle tri(va, vb, vc);
	}

	//
	//Store a binding for each Joint as a list in Skin
	tokenizer->FindToken("bindings");
	count = tokenizer->GetInt();

	
	for (int i = 0; i < count; i++) {
		tokenizer->FindToken("matrix");
		tokenizer->FindToken("{");

		glm::mat4 mat(1.0);

		float a, b, c;

		for (int j = 0; j < 4; j++) {
			a = tokenizer->GetFloat();
			b = tokenizer->GetFloat();
			c = tokenizer->GetFloat();
			mat[0][j] = a;
			mat[1][j] = b;
			mat[2][j] = c;
		}

		mat[3][0] = 0.0;
		mat[3][1] = 0.0;
		mat[3][2] = 0.0;
		mat[3][3] = 1.0;

		mat = glm::transpose(mat);
		Skin::bindings.push_back(mat);
	}

	tokenizer->Close();

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);

	// Bind to the VAO.
	glBindVertexArray(VAO);

	// Bind to the first VBO - We will use it to store the vertices
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO - We will use it to store the normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Generate EBO, bind the EBO to the bound VAO and send the data
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	// Unbind the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void Skin::update() {
	if (Skin::joints.size() == 0) { //Use default bind pose if no Skeleton
		return;
	}

	//Compute matrices for each joint
	//Position: World * (Bind)^-1
	//Normal: ( World * (Bind)^-1 )^-1T
	std::vector<glm::mat4> pos_mats;
	std::vector<glm::mat4> bind_mats;
	std::vector<glm::mat4> norm_mats;
	for (int i = 0; i < Skin::joints.size(); i++) {
		glm::mat4 pos_mat = Skin::joints[i]->world_matrix;
		glm::mat4 bind_mat = Skin::bindings[i];
		pos_mats.push_back(pos_mat * glm::inverse(bind_mat));

		glm::mat4 norm_mat = pos_mat * glm::inverse(bind_mat);
		norm_mat = glm::inverse(glm::transpose(norm_mat));
		norm_mats.push_back(norm_mat);
	}
	
	//Update position and normals for each Vertex
	for (int i = 0; i < Skin::vertices.size(); i++) {
		glm::vec3 orig_pos = Skin::vertices[i].position;
		glm::vec3 orig_norm = Skin::vertices[i].normal;
		
		glm::vec3 accum_pos(0.0, 0.0, 0.0);
		glm::vec3 accum_norm(0.0, 0.0, 0.0);

		for (int j = 0; j < Skin::vertices[i].weights.size(); j++) {
			float weight = Skin::vertices[i].weights[j].second;
			int index = Skin::vertices[i].weights[j].first;
			accum_pos = accum_pos + weight * glm::vec3(pos_mats[index] * glm::vec4(orig_pos, 1));

			accum_norm = accum_norm + weight * glm::vec3(norm_mats[index] * glm::vec4(orig_norm, 0));
		}
		Skin::positions[i] = accum_pos;
		Skin::normals[i] = accum_norm;
	}
	
	//Update vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Skin::positions.size(), Skin::positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//Update normal vectors
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Skin::normals.size(), Skin::normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
}

void Skin::draw(const glm::mat4& viewProjMtx, GLuint shader) {
	// activate the shader program
	glUseProgram(shader);

	// get the locations and send the uniforms to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
	glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

	glm::mat4 camToWorld = glm::inverse(viewProjMtx);
	glm::vec3 camPosition = glm::vec3(camToWorld[3]);
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, &camPosition[0]);

	// Bind the VAO
	glBindVertexArray(VAO);

	// draw the points using triangles, indexed with the EBO
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void Skin::displayData() {
	std::cout << "Positions: " << std::endl;
	for (int i = 0; i < Skin::vertices.size(); i++) {
		std::cout << "	" << Skin::vertices[i].position.x << "	" << Skin::vertices[i].position.y << "	" << Skin::vertices[i].position.z << std::endl;
	}

	std::cout << "Normals: " << std::endl;
	for (int i = 0; i < Skin::vertices.size(); i++) {
		std::cout << "	" << Skin::vertices[i].normal.x << "	" << Skin::vertices[i].normal.y << "	" << Skin::vertices[i].normal.z << std::endl;
	}
	
	std::cout << "Skin Weights: " << std::endl;
	for (int i = 0; i < Skin::vertices.size(); i++) {
		for (int j = 0; j < Skin::vertices[i].weights.size(); j++) {
			std::cout << Skin::vertices[i].weights.size() << " " << Skin::vertices[i].weights[j].first << " " << Skin::vertices[i].weights[j].second << " ";
		}
		std::cout << std::endl;
	}
	
	std::cout << "Indices: " << std::endl;
	for (int i = 0; i < Skin::indices.size(); i+=3) {
		std::cout << Skin::indices[i] << " ";
		std::cout << Skin::indices[i+1] << " ";
		std::cout << Skin::indices[i+2] << " " << std::endl;
	}
	
	std::cout << "Joints: " << std::endl;
	for (int i = 0; i < Skin::joints.size(); i++) {
		std::string name(Skin::joints[i]->name);
		std::cout << name << std::endl;
	}
}