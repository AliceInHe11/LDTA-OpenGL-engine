#include "engine.h"
#include "render.cpp"
#include "input.cpp"
#include "texture.cpp"

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    readWindowConfig("config/windowconfig.txt", FULL_SCREEN, SCR_WIDTH, SCR_HEIGHT);

    GLFWwindow* window = NULL;
    if (FULL_SCREEN == 1)
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LDTA OPENGL-ENGINE", glfwGetPrimaryMonitor(), NULL); // full screen?
    else
        if (FULL_SCREEN == 0)
            window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LDTA OPENGL-ENGINE", NULL, NULL); // window screen
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
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
        return -1;
    }

    unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096, SHADOW_RANGE = 20;
    readVideoConfig("config/videoconfig.txt", SHADOW_RANGE, SHADOW_WIDTH, SHADOW_HEIGHT);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ShadowMapshader("resources/shaders/shadow_mapping.vs", "resources/shaders/shadow_mapping.fs");
    Shader simpleDepthShader("resources/shaders/shadow_mapping_depth.vs", "resources/shaders/shadow_mapping_depth.fs");
    Shader debugDepthQuad("resources/shaders/debug_quad.vs", "resources/shaders/debug_quad_depth.fs");
    Shader skyboxShader("resources/shaders/skybox.vs", "resources/shaders/skybox.fs");
    Shader normalmapShader("resources/shaders/normal_mapping.vs", "resources/shaders/normal_mapping.fs");

    // load models
   // -----------
    vector <Model> ModelList;
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

    // load textures
    // -------------
    vector <unsigned int> Texture;
    Texture.push_back(loadTexture("resources/textures/dirt.bmp"));
    Texture.push_back(loadTexture("resources/textures/dirt3.bmp"));
    Texture.push_back(loadTexture("resources/textures/ambatukam.bmp"));

    // load sky box textures
    // ---------------------
    vector<string> faces
    {
        "resources/textures/skybox/right.jpg",
        "resources/textures/skybox/left.jpg",
        "resources/textures/skybox/top.jpg",
        "resources/textures/skybox/bottom.jpg",
        "resources/textures/skybox/front.jpg",
        "resources/textures/skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

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
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    ShadowMapshader.use();
    ShadowMapshader.setInt("diffuseTexture", 0);
    ShadowMapshader.setInt("shadowMap", 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);
    normalmapShader.use();
    normalmapShader.setInt("diffuseMap", 0);
    normalmapShader.setInt("normalMap", 1);

    // lighting info
    // -------------
    glm::vec3 lightPos(3.0f, 10.0f, 3.0f);

    cout << "Em Oi Lau Dai Tinh Ai Doooooooo.........!!!!!!!!!!";

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

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 0.15f, far_plane = 30.5f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-(float)SHADOW_RANGE, (float)SHADOW_RANGE, -(float)SHADOW_RANGE, (float)SHADOW_RANGE, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, dirtTexture);
        renderScene(simpleDepthShader, Texture);
        renderModel(simpleDepthShader, ModelList);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // 2. render model normal-mapped   
        // -----------------------------
        normalmapShader.use();
        normalmapShader.setMat4("projection", projection);
        normalmapShader.setMat4("view", view);
        normalmapShader.setVec3("viewPos", camera.Position);
        normalmapShader.setVec3("lightPos", lightPos);
        renderModel(normalmapShader, ModelList);
        //renderScene(ShadowMapshader, Texture);
        // render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
        /*glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        normalmapShader.setMat4("model", model);*/

        // 3. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        ShadowMapshader.use();
        ShadowMapshader.setMat4("projection", projection);
        ShadowMapshader.setMat4("view", view);
        // set light uniforms
        ShadowMapshader.setVec3("viewPos", camera.Position);
        ShadowMapshader.setVec3("lightPos", lightPos);
        ShadowMapshader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture[0]);*/
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        renderScene(ShadowMapshader, Texture);
        //renderModel(ShadowMapshader, ModelList);

        // 4. render skybox as last
        // ----------------------
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        /*debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap)*/;
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

    glfwTerminate();
    return 0;
}

