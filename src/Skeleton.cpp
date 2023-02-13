#include <string>
#include "Skeleton.h"
#include "core.h"
#include "Tokenizer.h"

Skeleton::Skeleton() {
    Skeleton::name = "skel";
    Skeleton::root = NULL;
}

Skeleton::Skeleton(const char* filepath, const char* name) {
    Skeleton::name = name;
    Skeleton::root = NULL;
    Skeleton::load(filepath);
}

Skeleton::~Skeleton() {

}

/*
 * Given a .skel file, create a tree of connected Joints
 * parsing the file for the joint names, degrees of freedom, transforms, and parent/child relations
 */
void Skeleton::load(const char* filepath) {
    /* Parse the .skel file by token for each Joint component */
    Tokenizer* tokenizer = new Tokenizer();
    bool success = tokenizer->Open(filepath);
    if (!success) {
        std::cout << "loadSkel() ERROR: Tokenizer could not open the file: " << filepath << std::endl;
        return;
    }

    //Load joints starting from the root
    char text[256];
    tokenizer->FindToken("balljoint");
    tokenizer->GetToken(text);
    
    std::string str = text;
    char* name = new char[str.size()];
    strcpy(name, str.c_str());

    Joint* root = new Joint(name);
    root->load(*tokenizer);

    tokenizer->Close();

    Skeleton::root = root;


    //Gather all the loaded Joints into an indexable list (for skinning weights later)
    Skeleton::listJoints(root);

    /*for (int i = 0; i < Skeleton::joints.size(); i++) {
        std::cout << Skeleton::joints[i]->name << std::endl;
    }*/

}

/*
 * Read out the skeleton's Joint hierarchy and data to stdout starting from root
 */
void Skeleton::display() {
    std::cout << "Displaying skeleton: " << Skeleton::name << std::endl;

    Joint* curr = root;
    if (curr != NULL) {
        root->recurseDisplayData();
    }
}

void Skeleton::update() {
    for (int i = 0; i < Skeleton::joints.size(); i++) {
        Skeleton::joints[i]->update();
    }
}

/*
 * Draw the entire skeleton in the viewport by iterating over every Joint in the hierarchy starting from the root
 */
void Skeleton::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    Joint* curr = Skeleton::root;
    
    if (curr != NULL) {
        root->recurseDraw(viewProjMtx, shader);
    }
}

/* Recursive down the hierarchy and add every Joint in a listed order for easy indexing (for skin weights later) */
void Skeleton::listJoints(Joint* joint) {
    Skeleton::joints.push_back(joint);

    Joint* child;
    for (int i = 0; i < joint->children.size(); i++) {
        child = joint->children[i];
        if (child == NULL) {
            continue;
        }

        Skeleton::listJoints(child);
    }
}

/* ----- Get/Set -----*/

void Skeleton::setName(const char* name) {
    Skeleton::name = name;
}
const char* Skeleton::getName() {
    return Skeleton::name;
}

void Skeleton::setRoot(Joint* joint) {
    Skeleton::root = joint;
}
Joint* Skeleton::getRoot() {
    return Skeleton::root;
}