#include <glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <stb_image.h>

#include <camera.h>
#include <model.h>
#include <shader.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "texture.cpp"
void readVideoConfig(const std::string& filename, unsigned int& shadowRange, unsigned int& shadowWidth, unsigned int& shadowHeight) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line))
            shadowRange = std::stoi(line);
        if (std::getline(file, line))
            shadowWidth = std::stoi(line);
        if (std::getline(file, line))
            shadowHeight = std::stoi(line);
        file.close();

        if (shadowRange < 0) {
            cout << "INVLID VALUE - RESTORE TO DEFAULT RANGE (20).";
            shadowRange = 20;
        }
        if (shadowWidth < 0 || shadowHeight < 0) {
            cout << endl << "INVLID VALUE - RESTORE TO DEFAULT (2048)." << endl;
            shadowWidth = 2048;
            shadowHeight = 2048;
        }

        cout << "SHADOW RANGE: " << shadowRange;
        cout << endl << "SHADOW MAP RESOLUTION: " << shadowWidth << "x" << shadowHeight << endl;
    }
    else {
        cout << "CAN NOT OPEN CONFIG FILE - USING DEFAULT CONFIG.";
    }
}

void engineResource(vector <Shader>& Shaderlist, vector <Model>& ModelList, vector <unsigned int>& Texture, vector<string>& faces )
{
    // build and compile shaders
// -------------------------
    Shaderlist.push_back(Shader("resources/shaders/shadow_mapping.vs", "resources/shaders/shadow_mapping.fs"));
    Shaderlist.push_back(Shader("resources/shaders/shadow_mapping_depth.vs", "resources/shaders/shadow_mapping_depth.fs"));
    Shaderlist.push_back(Shader("resources/shaders/debug_quad.vs", "resources/shaders/debug_quad_depth.fs"));
    Shaderlist.push_back(Shader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs"));
    Shaderlist.push_back(Shader("resources/shaders/normal_mapping.vs", "resources/shaders/normal_mapping.fs"));

    // load models
    // ------------
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
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
    Texture.push_back(loadTexture("resources/textures/dirt.bmp"));
    Texture.push_back(loadTexture("resources/textures/dirt3.bmp"));
    Texture.push_back(loadTexture("resources/textures/ambatukam.bmp"));

    // load sky box textures
    // ---------------------
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

