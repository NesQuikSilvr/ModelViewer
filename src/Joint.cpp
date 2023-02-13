#include "Joint.h"
#include "Cube.h"

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

Joint::Joint(const char* name) {
	Joint::name = name;

	Joint::local_matrix = glm::mat4(1.0);
	Joint::world_matrix = glm::mat4(1.0);

	Joint::offset = glm::vec3(0, 0, 0);
	Joint::boxmin = glm::vec3(0, 0, 0);
	Joint::boxmax = glm::vec3(0, 0, 0);

	Joint::rotxlimit = glm::vec2(-100000, 100000);
	Joint::rotylimit = glm::vec2(-100000, 100000);
	Joint::rotzlimit = glm::vec2(-100000, 100000);

	Joint::pose = glm::vec3(0, 0, 0);

	Joint::parent = NULL;
	Joint::children;
}

Joint::~Joint() {

}

/* Recursively load values into Joint properties using Tokenizer */
void Joint::load(Tokenizer &tokenizer) {
	tokenizer.FindToken("{");
	bool neof = true;
	char text[256];

	while (neof) {
		neof = tokenizer.GetToken(text);
		
		if (strcmp(text, "offset") == 0) {
			Joint::offset.x = tokenizer.GetFloat();
			Joint::offset.y = tokenizer.GetFloat();
			Joint::offset.z = tokenizer.GetFloat();
		}
		else if (strcmp(text, "boxmin") == 0) {
			Joint::boxmin.x = tokenizer.GetFloat();
			Joint::boxmin.y = tokenizer.GetFloat();
			Joint::boxmin.z = tokenizer.GetFloat();
		}
		else if (strcmp(text, "boxmax") == 0) {
			Joint::boxmax.x = tokenizer.GetFloat();
			Joint::boxmax.y = tokenizer.GetFloat();
			Joint::boxmax.z = tokenizer.GetFloat();
		}
		else if (strcmp(text, "rotxlimit") == 0) {
			Joint::rotxlimit.x = tokenizer.GetFloat();
			Joint::rotxlimit.y = tokenizer.GetFloat();
		}
		else if (strcmp(text, "rotylimit") == 0) {
			Joint::rotylimit.x = tokenizer.GetFloat();
			Joint::rotylimit.y = tokenizer.GetFloat();
		}
		else if (strcmp(text, "rotzlimit") == 0) {
			Joint::rotzlimit.x = tokenizer.GetFloat();
			Joint::rotzlimit.y = tokenizer.GetFloat();
		}
		else if (strcmp(text, "pose") == 0) {
			Joint::pose.x = tokenizer.GetFloat();
			Joint::pose.y = tokenizer.GetFloat();
			Joint::pose.z = tokenizer.GetFloat();
		}
		else if (strcmp(text, "balljoint") == 0) {
			neof = tokenizer.GetToken(text);

			Joint* joint = new Joint(text);
			Joint::children.push_back(joint);
			joint->parent = this;
			joint->load(tokenizer);
		}

		else if (strcmp(text, "}") == 0) break;
	}
}

void Joint::update() {
	Joint::local_matrix = glm::mat4(1.0);
	Joint::local_matrix = glm::translate(Joint::local_matrix, Joint::offset);

	float posex = glm::clamp(Joint::pose.x, Joint::rotxlimit.x, Joint::rotxlimit.y);
	float posey = glm::clamp(Joint::pose.y, Joint::rotylimit.x, Joint::rotylimit.y);
	float posez = glm::clamp(Joint::pose.z, Joint::rotzlimit.x, Joint::rotzlimit.y);

	Joint::local_matrix = glm::rotate(Joint::local_matrix, posez, glm::vec3(0, 0, 1));
	Joint::local_matrix = glm::rotate(Joint::local_matrix, posey, glm::vec3(0, 1, 0));
	Joint::local_matrix = glm::rotate(Joint::local_matrix, posex, glm::vec3(1, 0, 0));

	if (Joint::parent == NULL) {
		Joint::world_matrix = Joint::local_matrix;
	}
	else {
		Joint::world_matrix = Joint::parent->world_matrix * Joint::local_matrix;
	}
}

/* Draw this individual Joint */
void Joint::draw(const glm::mat4& viewProjMtx, GLuint shader) {
	Cube* box = new Cube(Joint::boxmin, Joint::boxmax);

	box->draw(viewProjMtx * Joint::world_matrix, shader);
}

/* Recursively draw this Joint and its hierarchy */
void Joint::recurseDraw(const glm::mat4& viewProjMtx, GLuint shader) {
	Joint::draw(viewProjMtx, shader);

	Joint* child;
	for (int i = 0; i < Joint::children.size(); i++) {
		child = Joint::children[i];
		if (child == NULL) {
			continue;
		}
		child->recurseDraw(viewProjMtx, shader);
	}
}

/* Display data for this individual Joint */
void Joint::displayData() {
	std::cout << "	Joint name: " << Joint::name << " {" << std::endl;
	std::cout << "	Joint offset: "
			  << Joint::offset.x << " "
			  << Joint::offset.y << " "
			  << Joint::offset.z << " "
			  << std::endl;
	std::cout << "	Joint boxmin: "
			  << Joint::boxmin.x << " "
			  << Joint::boxmin.y << " "
			  << Joint::boxmin.z << " "
			  << std::endl;
	std::cout << "	Joint boxmax: "
			  << Joint::boxmax.x << " "
			  << Joint::boxmax.y << " "
			  << Joint::boxmax.z << " "
			  << std::endl;
	std::cout << "	Joint rotxlimit: "
			  << Joint::rotxlimit.x << " "
			  << Joint::rotxlimit.y << " "
			  << std::endl;
	std::cout << "	Joint rotylimit: "
			  << Joint::rotylimit.x << " "
			  << Joint::rotylimit.y << " "
			  << std::endl;
	std::cout << "	Joint rotzlimit: "
			  << Joint::rotzlimit.x << " "
			  << Joint::rotzlimit.y << " "
			  << std::endl;
	std::cout << "	Joint pose: "
			  << Joint::pose.x << " "
			  << Joint::pose.y << " "
			  << Joint::pose.z << " "
			  << std::endl;

	std::cout << "	Joint children: " << std::endl;
	for (int i = 0; i < Joint::children.size(); i++) {
		std::cout << "	" << Joint::children[i]->name << std::endl;
	}

	std::cout << "	Joint parent: ";
	if (Joint::parent == NULL) {
		std::cout << "JOINT HAS NO PARENT" << std::endl;
	}
	else {
		std::cout << Joint::parent->name << std::endl;
	}
	std::cout << "	}" << std::endl;
}

/* Recursively display data for this Joint and its hierarchy */
void Joint::recurseDisplayData() {
	Joint::displayData();

	Joint* child;
	for (int i = 0; i < Joint::children.size(); i++) {
		child = Joint::children[i];
		if (child == NULL) {
			continue;
		}
		child->recurseDisplayData();
	}
}