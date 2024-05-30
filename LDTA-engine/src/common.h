#ifndef COMMON_H
#define COMMON_H

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

struct ScreenInfo
{
    // settings window mode (1 - fullscren, 0 - window screen)
    unsigned int SCREEN_MODE = 0;
    // settings resolution
    unsigned int SCR_WIDTH = 800;
    unsigned int SCR_HEIGHT = 600;
};
ScreenInfo ScreenValue;

struct WindowsPosition
{
    unsigned int WINDOWS_POS_X;
    unsigned int WINDOWS_POS_Y;
};
WindowsPosition WindowsPos;

// lighting info
// -------------
glm::vec3 lightColor(0.75f, 0.75f, 0.75f);

// meshes
unsigned int planeVAO;

// renderCube() renders a 1x1 3D cube in NDC.
// ------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube();

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad();

// model matrix for object render
// ------------------------------
glm::mat4 model;

// timing
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// light position
glm::vec3 lightPos(3.0f, 30.0f, 3.0f);
bool DynamicPos = true;

// weapon switch
unsigned int weaponsNum = 0;
int weaponsSound = -1;

// player movement
int playerMovement = -1;

#endif