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
#include <audio.h>
#include <soundinfo.h>

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

// lighting info
// -------------
glm::vec3 lightColor(0.75f, 0.75f, 0.75f);

class Engine
{
public:
    int engineRun()
    {
        // glfw: initialize and configure
        // ------------------------------
        if (!glfwInit())
        {
            SET_COLOR(RED);
            std::cout << "Failed to initializes the GLFW libary !\n";
            SET_COLOR(WHITE);
            system("pause");
            return -1;
        }

        EngineInfo engineConfig;
        readEnginConfig("config/engineconfig.txt", engineConfig);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, engineConfig.MAJOR_VERSION);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, engineConfig.MINOR_VERSION);
        glfwWindowHint(GLFW_SAMPLES, engineConfig.SAMPLES_LEVEL);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        // --------------------
        readWindowConfig("config/windowconfig.txt", ScreenValue);
        GLFWwindow* window = NULL;
        if (ScreenValue.SCREEN_MODE == 1)
            window = glfwCreateWindow(ScreenValue.SCR_WIDTH, ScreenValue.SCR_HEIGHT, "LDTA - OPENGL ENGINE", glfwGetPrimaryMonitor(), NULL); // full screen
        else
        if (ScreenValue.SCREEN_MODE == 0)
        {
             window = glfwCreateWindow(ScreenValue.SCR_WIDTH, ScreenValue.SCR_HEIGHT, "LDTA - OPENGL ENGINE", NULL, NULL); // window screen
             setWindowPosition(ScreenValue, WindowsPos);
             glfwSetWindowPos(window, WindowsPos.WINDOWS_POS_X, WindowsPos.WINDOWS_POS_Y);
        }

        if (!window)
        {
            SET_COLOR(RED);
            std::cout << "Failed to create GLFW window !\n" << std::endl;
            SET_COLOR(WHITE);
            glfwTerminate();
            system("pause");
            return -1;
        }

        GLFWimage images[1];
        images[0].pixels = stbi_load("resources/textures/cute_animal_chicken_cartoon_icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);

        // tell GLFW to capture the mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // configure video 
        // ---------------
        ShadowInfo value;
        readVideoConfig("config/videoconfig.txt", value);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            SET_COLOR(RED);
            std::cout << "Failed to initialize GLAD !\n" << std::endl;
            SET_COLOR(WHITE);
            glfwTerminate();
            system("pause");
            return -1;
        }
        else
        {
            std::cout << "OpenGL functions succesfully loaded." << std::endl;
            std::cout << "Version: " + std::string((char*)glGetString(GL_VERSION)) << " - (Major: " + std::to_string(GLVersion.major) + ", Minor: " + std::to_string(GLVersion.minor) << ")" << std::endl;
            std::cout << "Driver: " + std::string((char*)glGetString(GL_VENDOR)) << std::endl;
            std::cout << "Total System Memory (RAM): " + SystemMemoryInfo() << std::endl;
            std::cout << "Processor: " + GetCpuInfo() << std::endl;
            std::cout << "Renderer: " + std::string((char*)glGetString(GL_RENDERER)) << std::endl;
            std::cout << "Display Memory (VRAM): " + DisplayMemory() << std::endl;
            std::cout << "Shading language version: " + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) << std::endl;
        }

        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);

        // set up render data 
        // ------------------
        cubeVAO = 0;
        quadVAO = 0;
        unsigned int cubemapTexture;
        engineResourceLoader(Shaderlist, ModelList, SoundList, texture, cubemapTexture, audio);

        // configure VAO 
        // -------------
        // plane VAO
        unsigned int planeVBO;
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);
        // skybox VAO
        unsigned int skyboxVAO, skyboxVBO;
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        // configure depth map FBO 
        // -----------------------
        unsigned int depthMapFBO;
        glGenFramebuffers(1, &depthMapFBO);
        // create depth texture
        unsigned int depthMaP;
        glGenTextures(1, &depthMaP);
        glBindTexture(GL_TEXTURE_2D, depthMaP);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, value.SHADOW_WIDTH, value.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMaP, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // shader configuration
        // --------------------
        Shaderlist[s_SHADOWMAP].use();
        Shaderlist[s_SHADOWMAP].setInt("shadowMap", 1);
        Shaderlist[s_SHADOWMAP].setInt("diffuseTexture", 0);

        Shaderlist[s_DEBUGQUAD].use();
        Shaderlist[s_DEBUGQUAD].setInt("depthMap", 0);

        Shaderlist[s_NORMALMAP].use();
        Shaderlist[s_NORMALMAP].setInt("diffuseMap", 0);
        Shaderlist[s_NORMALMAP].setInt("normalMap", 1);
        Shaderlist[s_NORMALMAP].setInt("shadowMap", 1);

        Shaderlist[s_SKYBOX].use();
        Shaderlist[s_SKYBOX].setInt("skybox", 0);

        // Play looping music to start scene
        audio.playSound(SoundList[0]);

        std::cout << "AMBATUKAM!!!!!!!!!!!!\n";

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window))
        {
            // per-frame time logic
            // --------------------
            GLfloat currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            GLfloat near_plane = 0.15f, far_plane = 30.5f;
            //*lightProjection = glm::perspective(glm::radians(90.0f), (GLfloat)value.SHADOW_WIDTH / (GLfloat)value.SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
            *lightProjection = glm::ortho(-(GLfloat)value.SHADOW_RANGE, (GLfloat)value.SHADOW_RANGE, -(GLfloat)value.SHADOW_RANGE, (GLfloat)value.SHADOW_RANGE, near_plane, far_plane);
            *lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
            *lightSpaceMatrix = *lightProjection * *lightView;

            *projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)ScreenValue.SCR_WIDTH / (GLfloat)ScreenValue.SCR_HEIGHT, 0.1f, 100.0f);
            *view = camera.GetViewMatrix();

            // input
            // -----
            processInput(window);
            // change light position over time
            if (DynamicPos == true)
            {
                lightPos.x = sin(glfwGetTime()) * 5.5f;
                lightPos.z = cos(glfwGetTime()) * 5.5f;
            }
            //lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f ;
            changeLightPos(window, lightPos);
            changeLightInfo(window, lightColor, ambientIntensity);

            // render
            // ------
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 1. render depth of scene to texture (from light's perspective)
            // --------------------------------------------------------------
            // render scene from light's point of view
            Shaderlist[s_DEPTHMAP].use();
            Shaderlist[s_DEPTHMAP].setMat4("lightSpaceMatrix", *lightSpaceMatrix);

            glViewport(0, 0, value.SHADOW_WIDTH, value.SHADOW_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            renderScene(Shaderlist[s_DEPTHMAP], texture, renderDepth = true);
            renderModel(Shaderlist[s_DEPTHMAP], ModelList, depthMaP, renderDepth = true);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // reset viewport
            glViewport(0, 0, ScreenValue.SCR_WIDTH, ScreenValue.SCR_HEIGHT);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 2. render model normal-mapped   
            // -----------------------------
            Shaderlist[s_NORMALMAP].use();
            Shaderlist[s_NORMALMAP].setMat4("projection", *projection);
            Shaderlist[s_NORMALMAP].setMat4("view", *view);
            Shaderlist[s_NORMALMAP].setFloat("ambientIntensity", ambientIntensity);
            Shaderlist[s_NORMALMAP].setVec3("viewPos", camera.Position);
            Shaderlist[s_NORMALMAP].setVec3("lightPos", lightPos);
            Shaderlist[s_NORMALMAP].setVec3("lightColor", lightColor);
            Shaderlist[s_NORMALMAP].setMat4("lightSpaceMatrix", *lightSpaceMatrix);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMaP);
            renderModel(Shaderlist[s_NORMALMAP], ModelList, depthMaP, renderDepth = false);

            // 3. render scene as normal using the generated depth/shadow map  
            // --------------------------------------------------------------
            Shaderlist[s_SHADOWMAP].use();
            Shaderlist[s_SHADOWMAP].setMat4("projection", *projection);
            Shaderlist[s_SHADOWMAP].setMat4("view", *view);
            Shaderlist[s_SHADOWMAP].setVec3("viewPos", camera.Position);
            Shaderlist[s_SHADOWMAP].setFloat("ambientIntensity", ambientIntensity);
            Shaderlist[s_SHADOWMAP].setVec3("lightPos", lightPos);
            Shaderlist[s_SHADOWMAP].setVec3("lightColor", lightColor);
            Shaderlist[s_SHADOWMAP].setMat4("lightSpaceMatrix", *lightSpaceMatrix);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMaP);
            renderScene(Shaderlist[s_SHADOWMAP], texture, renderDepth = false);

            // 4. render view model
            // --------------------
            //glDepthFunc(GL_ALWAYS);
            renderViewmodel(Shaderlist[s_MODELDRAW], ModelList, *projection, *view, weaponsNum);
            //glDepthFunc(GL_LESS);

            // 5. render skybox as last
            // ------------------------
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            Shaderlist[s_SKYBOX].use();
            glm:: mat4 model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(1.5f));
            model = glm::rotate(model, glm::radians((float)glfwGetTime() * 4.0f), glm::normalize(glm::vec3(0.25f, 0.0f, 0.5f)));
            *view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
            Shaderlist[s_SKYBOX].setMat4("view", *view);
            Shaderlist[s_SKYBOX].setMat4("projection", *projection);
            Shaderlist[s_SKYBOX].setMat4("model", model);
            // skybox cube
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default

            // 6. play sound effect (gunshot,footstep,...)
            // -------------------------------------------
            playSoundEffect();

            // render Depth map to quad for visual debugging
            // ---------------------------------------------
            //Shaderlist[s_DEBUGQUAD].use();
            //Shaderlist[s_DEBUGQUAD].setFloat("near_plane", near_plane);
            //Shaderlist[s_DEBUGQUAD].setFloat("far_plane", far_plane);
            //glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, depthMaP);
            //renderQuad();

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &planeVAO);
        glDeleteBuffers(1, &planeVBO);
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);

        delete lightProjection, lightSpaceMatrix, lightView, projection, view;

        glfwTerminate();
        for (int i = 0; i < SoundList.size(); i++)
            if(audio.soundIsPlaying(SoundList[i]))
               audio.stopSound(SoundList[i]);
        
        system("pause");
        return 0;
    }

private:
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[48] = 
    {
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
    float skyboxVertices[108] = 
    {
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

    // define shader numners
    // ---------------------
    enum
    {
        s_SHADOWMAP,
        s_DEPTHMAP,
        s_DEBUGQUAD,
        s_SKYBOX,
        s_NORMALMAP,
        s_MODELDRAW
    };

    // render info
    // -----------
    float ambientIntensity = 0.75f;
    bool renderDepth;
    glm::mat4* lightProjection = new glm::mat4;
    glm::mat4* lightView = new glm::mat4;
    glm::mat4* lightSpaceMatrix = new glm::mat4;
    glm::mat4* projection = new glm::mat4;
    glm::mat4* view = new glm::mat4;
    GLfloat countdown1 = 0, countdown2 = 0, moveCountDown = 0;

    // resource info
    // -------------
    std::vector <unsigned int> texture;
    std::vector <std::string> faces;
    std::vector <Shader> Shaderlist;
    std::vector <Model> ModelList;
    std::vector <SoundInfo> SoundList;

    // audio
    // -----
    AudioEngine audio;

    void playSoundEffect()
    {
        if (playerMovement == 1 && (moveCountDown == 0 || lastFrame - moveCountDown >= 0.3f))
        {
            audio.playSound(SoundList[5]);
            audio.playSound(SoundList[6]);
            audio.playSound(SoundList[7]);
            audio.playSound(SoundList[8]);
            moveCountDown = lastFrame;
        }
        else
        if (playerMovement == 2 && (moveCountDown == 0 || lastFrame - moveCountDown >= 0.2f))
        {
            audio.playSound(SoundList[5]);
            audio.playSound(SoundList[6]);
            audio.playSound(SoundList[7]);
            audio.playSound(SoundList[8]);
            moveCountDown = lastFrame;
        }

        if (weaponsSound == 0 && (countdown1 == 0 || lastFrame - countdown1 >= 0.1f))
        {
            audio.playSound(SoundList[2]);
            audio.playSound(SoundList[3]);
            countdown1 = lastFrame;
        }
        else
        if (weaponsSound == 1 && (countdown2 == 0 || lastFrame - countdown2 >= 2.0f))
        {
             audio.playSound(SoundList[4]);
             countdown2 = lastFrame;
        }
    }

    void readEnginConfig(const std::string& filename, EngineInfo& value) 
    {
        std::ifstream file(filename);

        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line)) 
            {
                size_t pos = line.find('=');
                if (pos != std::string::npos) 
                {
                    std::string key = line.substr(0, pos);
                    std::string getvalue = line.substr(pos + 1);

                    // Remove leading and trailing whitespace from key and value
                    key.erase(0, key.find_first_not_of(" \t"));
                    key.erase(key.find_last_not_of(" \t") + 1);
                    getvalue.erase(0, getvalue.find_first_not_of(" \t"));
                    getvalue.erase(getvalue.find_last_not_of(" \t") + 1);

                    if (key == "major_version") 
                        value.MAJOR_VERSION = std::stoi(getvalue);                
                    else 
                    if (key == "minor_version") 
                        value.MINOR_VERSION = std::stoi(getvalue);             
                    else 
                    if (key == "sample_level") 
                        value.SAMPLES_LEVEL = std::stoi(getvalue);
                    
                }
            }

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

    void readVideoConfig(const std::string& filename, ShadowInfo& value) 
    {
        std::ifstream file(filename);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line)) 
            {
                size_t pos = line.find('=');
                if (pos != std::string::npos) 
                {
                    std::string key = line.substr(0, pos);
                    std::string getvalue = line.substr(pos + 1);

                    // Remove leading and trailing whitespace from key and value
                    key.erase(0, key.find_first_not_of(" \t"));
                    key.erase(key.find_last_not_of(" \t") + 1);
                    getvalue.erase(0, getvalue.find_first_not_of(" \t"));
                    getvalue.erase(getvalue.find_last_not_of(" \t") + 1);

                    if (key == "shadow_range") 
                        value.SHADOW_RANGE = std::stoi(getvalue);                 
                    else 
                    if (key == "shadow_width") 
                        value.SHADOW_WIDTH = std::stoi(getvalue);     
                    else 
                    if (key == "shadow_height") 
                        value.SHADOW_HEIGHT = std::stoi(getvalue);
                    
                }
            }

            file.close();

            if (value.SHADOW_RANGE < 0) 
            {
                std::cout << "INVLID VALUE - RESTORE TO DEFAULT RANGE (10).";
                value.SHADOW_RANGE = 10;
            }
            if (value.SHADOW_WIDTH < 0 || value.SHADOW_HEIGHT < 0) 
            {
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

    void engineResourceLoader(std::vector <Shader>& Shaderlist, std::vector <Model>& ModelList, std::vector <SoundInfo>& SoundList, std::vector <unsigned int>& Texture, unsigned int & cubemapTexture, AudioEngine& audio)
    {
        // build and compile shaders
        // -------------------------
        std::cout << std::endl;
        Shaderlist.push_back(Shader("resources/shaders/shadow_mapping.vs", "resources/shaders/shadow_mapping.fs"));
        Shaderlist.push_back(Shader("resources/shaders/shadow_mapping_depth.vs", "resources/shaders/shadow_mapping_depth.fs"));
        Shaderlist.push_back(Shader("resources/shaders/debug_quad.vs", "resources/shaders/debug_quad_depth.fs"));
        Shaderlist.push_back(Shader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs"));
        Shaderlist.push_back(Shader("resources/shaders/normal_mapping.vs", "resources/shaders/normal_mapping.fs"));
        Shaderlist.push_back(Shader("resources/shaders/viewmodel.vs", "resources/shaders/viewmodel.fs"));

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
        ModelList.push_back(Model("resources/objects/viking_room/viking_room.obj"));
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
        std::vector <std::string> faces
        {
            "resources/textures/skybox2/right.jpg",
            "resources/textures/skybox2/left.jpg",
            "resources/textures/skybox2/top.jpg",
            "resources/textures/skybox2/bottom.jpg",
            "resources/textures/skybox2/front.jpg",
            "resources/textures/skybox2/back.jpg"
        };
        cubemapTexture = loadCubemap(faces);

        // Initialize Audio Engine and Load sounds
        // ---------------------------------------
        // load sound 
        SoundList.push_back(SoundInfo("resources/audio/bgm_track2_loop.mp3", 0.025f, 2.0f, SOUND_LOOP));
        SoundList.push_back(SoundInfo("resources/audio/bgm_track1_loop.mp3", 0.025f, 2.0f, SOUND_LOOP));
        SoundList.push_back(SoundInfo("resources/audio/AK47_Fire1.wav", 0.0075f, 2.0f, SOUND_ONE_SHOT));
        SoundList.push_back(SoundInfo("resources/audio/AK47_Fire2.wav", 0.075f, 2.0f, SOUND_ONE_SHOT));
        SoundList.push_back(SoundInfo("resources/audio/D.mp3", 0.175f, 9.0f, SOUND_ONE_SHOT));
        SoundList.push_back(SoundInfo("resources/audio/player_step_1.wav", 0.05f, 3.0f, SOUND_ONE_SHOT));
        SoundList.push_back(SoundInfo("resources/audio/player_step_2.wav", 0.05f, 3.0f, SOUND_ONE_SHOT));
        SoundList.push_back(SoundInfo("resources/audio/player_step_3.wav", 0.05f, 3.0f, SOUND_ONE_SHOT));
        SoundList.push_back(SoundInfo("resources/audio/player_step_4.wav", 0.05f, 3.0f, SOUND_ONE_SHOT));
        // Initialize audio engine
        std::cout << std::endl;
        audio.init();
        for(int i=0;i<SoundList.size();i++)
            audio.loadSound(SoundList[i]);
    }
};

#endif