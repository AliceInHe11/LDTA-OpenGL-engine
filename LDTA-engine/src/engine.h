#include <iostream>
#include <vector> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>

#include <glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utility.h>
#include <camera.h>
#include <model.h>
#include <shader.h>

#include "texture.cpp"
#include "render.cpp"
#include "input.cpp"

struct ShadowInfo
{
    unsigned int SHADOW_WIDTH;
    unsigned int SHADOW_HEIGHT;
    unsigned int SHADOW_RANGE;
};

// lighting info
// -------------
glm::vec3 lightPos(3.0f, 10.0f, 3.0f);
float ambientIntensity = 0.75f;
glm::vec3 lightColor(0.75f, 0.75f, 0.75f);
bool renderDepth;
glm::mat4* lightProjection = new glm::mat4;
glm::mat4* lightView = new glm::mat4;
glm::mat4* lightSpaceMatrix = new glm::mat4;

// camera info
// -----------
glm::mat4* projection = new glm::mat4;
glm::mat4* view = new glm::mat4;

// render info
// -----------
vector <unsigned int> Texture;
vector <string> faces;
vector <Shader> Shaderlist;
vector <Model> ModelList;

void readVideoConfig(const std::string& filename, ShadowInfo &value) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line))
            value.SHADOW_RANGE = std::stoi(line);
        if (std::getline(file, line))
            value.SHADOW_WIDTH = std::stoi(line);
        if (std::getline(file, line))
            value.SHADOW_HEIGHT = std::stoi(line);
        file.close();

        if (value.SHADOW_RANGE < 0) {
            cout << "INVLID VALUE - RESTORE TO DEFAULT RANGE (20).";
            value.SHADOW_RANGE = 20;
        }
        if (value.SHADOW_WIDTH < 0 || value.SHADOW_HEIGHT < 0) {
            cout << endl << "INVLID VALUE - RESTORE TO DEFAULT (2048)." << endl;
            value.SHADOW_WIDTH = 2048;
            value.SHADOW_HEIGHT = 2048;
        }

        cout << "SHADOW RANGE: " << value.SHADOW_RANGE;
        cout << endl << "SHADOW MAP RESOLUTION: " << value.SHADOW_WIDTH << "x" << value.SHADOW_HEIGHT << endl;
    }
    else 
    {
        cout << "CAN NOT OPEN CONFIG FILE - USING DEFAULT CONFIG.";
    }
}

void engineResource(vector <Shader>& Shaderlist, vector <Model>& ModelList, vector <unsigned int>& Texture, vector<string>& faces )
{
    // build and compile shaders
    // -------------------------
    cout << endl;
    Shaderlist.push_back(Shader("resources/shaders/shadow_mapping.vs", "resources/shaders/shadow_mapping.fs"));
    Shaderlist.push_back(Shader("resources/shaders/shadow_mapping_depth.vs", "resources/shaders/shadow_mapping_depth.fs"));
    Shaderlist.push_back(Shader("resources/shaders/debug_quad.vs", "resources/shaders/debug_quad_depth.fs"));
    Shaderlist.push_back(Shader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs"));
    Shaderlist.push_back(Shader("resources/shaders/normal_mapping.vs", "resources/shaders/normal_mapping.fs"));

    // load models
    // ------------
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    cout << endl;
    stbi_set_flip_vertically_on_load(true);
    ModelList.push_back(Model("resources/objects/backpack/backpack.obj"));
    stbi_set_flip_vertically_on_load(false);
    ModelList.push_back(Model("resources/objects/cottage/cottage.obj"));
    ModelList.push_back(Model("resources/objects/birch_tree/birch_tree.obj"));
    ModelList.push_back(Model("resources/objects/tree/tree.obj"));
    ModelList.push_back(Model("resources/objects/rock/rock.obj"));
    ModelList.push_back(Model("resources/objects/cyborg/cyborg.obj"));
    ModelList.push_back(Model("resources/objects/planet/planet.obj"));
    ModelList.push_back(Model("resources/objects/nanosuit/nanosuit.obj"));
    ModelList.push_back(Model("resources/objects/sponza/sponza.obj"));
    ModelList.push_back(Model("resources/objects/woodentower/woodentower.obj"));
    ModelList.push_back(Model("resources/objects/container/container.obj"));

    // load textures
    // -------------
    cout << endl;
    Texture.push_back(loadTexture("resources/textures/dirt.bmp"));
    Texture.push_back(loadTexture("resources/textures/dirt3.bmp"));
    Texture.push_back(loadTexture("resources/textures/ambatukam.bmp"));
    Texture.push_back(loadTexture("resources/textures/metal.bmp"));
    Texture.push_back(loadTexture("resources/textures/fat.bmp"));

    // load sky box textures
    // ---------------------
    cout << endl;
    vector <string> get
    {
        "resources/textures/skybox/right.jpg",
        "resources/textures/skybox/left.jpg",
        "resources/textures/skybox/top.jpg",
        "resources/textures/skybox/bottom.jpg",
        "resources/textures/skybox/front.jpg",
        "resources/textures/skybox/back.jpg"
    };
    faces = get;

}

