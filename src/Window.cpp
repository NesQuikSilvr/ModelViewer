#include "Window.h"
#include "Skeleton.h"
#include "Skin.h"
#include "Animation.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
Skeleton* skel;
Skin* skin;
Animation* anim;
bool loaded_skel;
bool loaded_skin;
bool loaded_anim;

int jointIndex = 0;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects(const char* skel_file, const char* skin_file, const char* anim_file) {

    //Load the assets to be displayed
   
    skel = new Skeleton(skel_file, "skeleton");

    Tokenizer tokenizer;
    loaded_skin = tokenizer.Open(skin_file);
    if (!loaded_skin) {
        std::cout << "Skin::load() ERROR: Tokenizer could not open the file: " << skin_file << std::endl;
    }
    else {
        skin = new Skin(&tokenizer, skel);
    }

    loaded_anim = tokenizer.Open(anim_file);
    if (loaded_anim) {
        anim = new Animation();
        anim->load(tokenizer);
        anim->update(0.0);
    }
    else {
        std::cout << "Anim file could not be opened: " << anim_file << std::endl;
    }


    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    delete skel;
    delete skin;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    skel->update();
    if (loaded_skin) {
        skin->update();
    }
    
    if (loaded_anim) {
        //anim->update(1.0);
    }
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    // Draw skel if there is no skin
    if (!loaded_skin) {
        skel->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }
    else {
        skin->draw(Cam->GetViewProjectMtx(), Window::shaderProgram);
    }

    // feed inputs to dear imgui, start new frame
    //ImGui_ImplOpenGL3_NewFrame();
    //ImGui_ImplGlfw_NewFrame();
    //ImGui::NewFrame();

    // render your GUI
    //ImGui::Begin("Joint Control");
    //ImGui::Text("Hello, world %s", skin->skel->getRoot()->name);
    //ImGui::Button("Hello!");
    //ImGui::End();

    // Render dear imgui into screen
    //ImGui::Render();
    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();

    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;
            case GLFW_KEY_W:
                skel->joints[jointIndex]->pose += glm::vec3(0.1, 0, 0);
                break;
            case GLFW_KEY_A:
                skel->joints[jointIndex]->pose += glm::vec3(0.0, -0.1, 0);
                std::cout << "moving left" << std::endl;
                break;
            case GLFW_KEY_S:
                skel->joints[jointIndex]->pose += glm::vec3(-0.1, 0, 0);
                std::cout << "moving back" << std::endl;
                break;
            case GLFW_KEY_D:
                skel->joints[jointIndex]->pose += glm::vec3(0.0, 0.1, 0);
                std::cout << "moving right" << std::endl;
                break;
            case GLFW_KEY_RIGHT:
                jointIndex++;
                break;
            case GLFW_KEY_LEFT:
                jointIndex--;
                break;
            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}