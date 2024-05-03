// Prevent accidentally selecting integrated GPU
// ---------------------------------------------
extern "C" {
    __declspec(dllexport) unsigned __int32 AmdPowerXpressRequestHighPerformance = 0x1;
    __declspec(dllexport) unsigned __int32 NvOptimusEnablement = 0x1;
}

#include "engine.h"

int main()
{
    EngineInfo engine;
    readEnginConfig("config/engineconfig.txt", engine);

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, engine.MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, engine.MINOR_VERSION);
    glfwWindowHint(GLFW_SAMPLES, engine.SAMPLES_VALUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
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

    // tell GLFW to capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        system("pause");
        return -1;
    }
    else
    {
        std::cout << "OpenGL functions succesfully loaded." << std::endl;
        std::cout << "Version: " + std::string((char*)glGetString(GL_VERSION)) << " - (Major: " + std::to_string(GLVersion.major) + ", Minor: " + std::to_string(GLVersion.minor) << ")" << std::endl;
        std::cout << "Driver: " + std::string((char*)glGetString(GL_VENDOR)) << std::endl;
        std::cout << "Total System Memory (RAM): " + SystemMemoryInfo() << endl;
        std::cout << "Processor: " + GetCpuInfo() << std::endl;
        std::cout << "Renderer: " + std::string((char*)glGetString(GL_RENDERER)) << std::endl;
        std::cout << "Shading language version: " + std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION)) << std::endl;   
    }

    // configure video 
    // ---------------
    ShadowInfo value;
    readVideoConfig("config/videoconfig.txt", value);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // set up render data 
    // ------------------
    engineResource(Shaderlist, ModelList, Texture, faces);
    unsigned int cubemapTexture = loadCubemap(faces);

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
    Shaderlist[0].use();
    Shaderlist[0].setInt("shadowMap", 1);
    Shaderlist[0].setInt("diffuseTexture", 0);

    Shaderlist[2].use();
    Shaderlist[2].setInt("depthMap", 0);

    Shaderlist[4].use();
    Shaderlist[4].setInt("diffuseMap", 0);
    Shaderlist[4].setInt("normalMap", 1);
    Shaderlist[4].setInt("shadowMap", 1);

    Shaderlist[3].use();
    Shaderlist[3].setInt("skybox", 0);

    cout << "AMBATUKAM!!!!!!!!!!!!";

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // change light position over time
        lightPos.x = sin(glfwGetTime()) * 5.5f;
        lightPos.z = cos(glfwGetTime()) * 5.5f;
        //lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f ;
        changeLightPos(window, lightPos);
        changeLightInfo(window, lightColor, ambientIntensity);
  
        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        float near_plane = 0.15f, far_plane = 30.5f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        *lightProjection = glm::ortho(-(float)value.SHADOW_RANGE, (float)value.SHADOW_RANGE, -(float)value.SHADOW_RANGE, (float)value.SHADOW_RANGE, near_plane, far_plane);
        *lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        *lightSpaceMatrix = *lightProjection * *lightView;
        // render scene from light's point of view
        Shaderlist[1].use();
        Shaderlist[1].setMat4("lightSpaceMatrix", *lightSpaceMatrix);

        glViewport(0, 0, value.SHADOW_WIDTH, value.SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderScene(Shaderlist[1], Texture, renderDepth = true);
        renderModel(Shaderlist[1], ModelList, depthMaP, renderDepth = true);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, ScreenValue.SCR_WIDTH, ScreenValue.SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        *projection = glm::perspective(glm::radians(camera.Zoom), (float)ScreenValue.SCR_WIDTH / (float)ScreenValue.SCR_HEIGHT, 0.1f, 100.0f);
        *view = camera.GetViewMatrix();

        // 2. render model normal-mapped   
        // -----------------------------
        Shaderlist[4].use();
        Shaderlist[4].setMat4("projection", *projection);
        Shaderlist[4].setMat4("view", *view);
        Shaderlist[4].setFloat("ambientIntensity", ambientIntensity);
        Shaderlist[4].setVec3("viewPos", camera.Position);
        Shaderlist[4].setVec3("lightPos", lightPos);
        Shaderlist[4].setVec3("lightColor", lightColor);
        Shaderlist[4].setMat4("lightSpaceMatrix", *lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMaP);
        renderModel(Shaderlist[4], ModelList,  depthMaP, renderDepth = false);

        // 3. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        Shaderlist[0].use();
        Shaderlist[0].setMat4("projection", *projection);
        Shaderlist[0].setMat4("view", *view);
        Shaderlist[0].setVec3("viewPos", camera.Position);
        Shaderlist[0].setFloat("ambientIntensity", ambientIntensity);
        Shaderlist[0].setVec3("lightPos", lightPos);
        Shaderlist[0].setVec3("lightColor", lightColor);
        Shaderlist[0].setMat4("lightSpaceMatrix", *lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMaP);
        renderScene(Shaderlist[0], Texture, renderDepth = false);

        // 4. render skybox as last
        // ----------------------
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        Shaderlist[3].use();
        *view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        Shaderlist[3].setMat4("view", *view);
        Shaderlist[3].setMat4("projection", *projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        //Shaderlist[2].use();
        //Shaderlist[2].setFloat("near_plane", near_plane);
        //Shaderlist[2].setFloat("far_plane", far_plane);
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

    system("pause");
    return 0;
}

