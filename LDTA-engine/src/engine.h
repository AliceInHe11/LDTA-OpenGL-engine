#ifndef ENGINE_H
#define ENGINE_H

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

#include <image.h>
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

struct EngineInfo
{
    int MAJOR_VERSION;
    int MINOR_VERSION;
    int SAMPLES_LEVEL;
};

// set up vertex data (and buffer(s)) and configure vertex attributes
   // ------------------------------------------------------------------
float planeVertices[] = {
    // positions           // normals         // texcoords
    25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  50.0f,  0.0f,
   -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
   -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,

    25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  50.0f,  0.0f,
   -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 50.0f,
    25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  50.0f, 50.0f
};

// set up sky vertex data 
// ----------------------
float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

// lighting info
// -------------
float ambientIntensity = 0.75f;
glm::vec3 lightColor(0.75f, 0.75f, 0.75f);
bool renderDepth;
glm::mat4* lightProjection = new glm::mat4;
glm::mat4* lightView = new glm::mat4;
glm::mat4* lightSpaceMatrix = new glm::mat4;

// view info
// ----------
glm::mat4* projection = new glm::mat4;
glm::mat4* view = new glm::mat4;

// render info
// -----------
std::vector <unsigned int> texture;
std::vector <std::string> faces;
std::vector <Shader> Shaderlist;
std::vector <Model> ModelList;

// define shader numners
// ---------------------
#define s_SHADOWMAP     0
#define s_DEPTHMAP      1
#define s_DEBUGQUAD     2
#define s_SKYBOX        3
#define s_NORMALMAP     4
#define s_MODELDRAW     5

void readEnginConfig(const std::string& filename, EngineInfo& value) {
    std::ifstream file(filename);

    if (file.is_open()) 
    {
        std::string line;
        if (std::getline(file, line))
            value.MAJOR_VERSION = std::stoi(line);
        if (std::getline(file, line))
            value.MINOR_VERSION = std::stoi(line);
        if (std::getline(file, line))
            value.SAMPLES_LEVEL = std::stoi(line);
        file.close();

        if (value.MAJOR_VERSION < 3 || value.MAJOR_VERSION > 4) 
            value.MAJOR_VERSION = 3;
        
        if (value.MINOR_VERSION < 3 || value.MINOR_VERSION > 6) 
            value.MINOR_VERSION = 3;

        if (value.SAMPLES_LEVEL < 0 || value.SAMPLES_LEVEL > 16)
            value.SAMPLES_LEVEL = 4;
        
    }
    else
    {
        std::cout << "CAN NOT OPEN CONFIG FILE - USING DEFAULT CONFIG.";
        value.MAJOR_VERSION = 3;
        value.MINOR_VERSION = 3;
        value.SAMPLES_LEVEL = 4;
    }
}

void readVideoConfig(const std::string& filename, ShadowInfo &value) {
    std::ifstream file(filename);
    if (file.is_open()) 
    {
        std::string line;
        if (std::getline(file, line))
            value.SHADOW_RANGE = std::stoi(line);
        if (std::getline(file, line))
            value.SHADOW_WIDTH = std::stoi(line);
        if (std::getline(file, line))
            value.SHADOW_HEIGHT = std::stoi(line);
        file.close();

        if (value.SHADOW_RANGE < 0) {
            std::cout << "INVLID VALUE - RESTORE TO DEFAULT RANGE (10).";
            value.SHADOW_RANGE = 10;
        }
        if (value.SHADOW_WIDTH < 0 || value.SHADOW_HEIGHT < 0) {
            std::cout << std::endl << "INVLID VALUE - RESTORE TO DEFAULT (512x512)." << std::endl;
            value.SHADOW_WIDTH = 512;
            value.SHADOW_HEIGHT = 512;
        }

        std::cout << "SHADOW RANGE: " << value.SHADOW_RANGE;
        std::cout << std::endl << "SHADOW MAP RESOLUTION: " << value.SHADOW_WIDTH << "x" << value.SHADOW_HEIGHT << std::endl;
    }
    else 
    {
        std::cout << "CAN NOT OPEN CONFIG FILE - USING DEFAULT CONFIG.";
        value.SHADOW_RANGE = 10;
        value.SHADOW_WIDTH = 512;
        value.SHADOW_HEIGHT = 512;
    }
}

void engineResource(std::vector <Shader>& Shaderlist, std::vector <Model>& ModelList, std::vector <unsigned int>& Texture, std::vector<std::string>& faces )
{
    // build and compile shaders
    // -------------------------
    std::cout << std::endl;
    Shaderlist.push_back(Shader("resources/shaders/shadow_mapping.vs", "resources/shaders/shadow_mapping.fs"));
    Shaderlist.push_back(Shader("resources/shaders/shadow_mapping_depth.vs", "resources/shaders/shadow_mapping_depth.fs"));
    Shaderlist.push_back(Shader("resources/shaders/debug_quad.vs", "resources/shaders/debug_quad_depth.fs"));
    Shaderlist.push_back(Shader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs"));
    Shaderlist.push_back(Shader("resources/shaders/normal_mapping.vs", "resources/shaders/normal_mapping.fs"));
    Shaderlist.push_back(Shader("resources/shaders/model_loading.vs", "resources/shaders/model_loading.fs"));

    // load models
    // ------------
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    std::cout << std::endl;
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
    ModelList.push_back(Model("resources/objects/weapons/ak47.obj"));
    ModelList.push_back(Model("resources/objects/weapons/StingSword.obj"));

    // load textures
    // -------------
    std::cout << std::endl;
    Texture.push_back(loadTexture("resources/textures/dirt.bmp"));
    Texture.push_back(loadTexture("resources/textures/dirt3.bmp"));
    Texture.push_back(loadTexture("resources/textures/ambatukam.bmp"));
    Texture.push_back(loadTexture("resources/textures/metal.bmp"));
    Texture.push_back(loadTexture("resources/textures/fat.bmp"));

    // load sky box textures
    // ---------------------
    std::cout << std::endl;
    std::vector <std::string> get
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

#endif