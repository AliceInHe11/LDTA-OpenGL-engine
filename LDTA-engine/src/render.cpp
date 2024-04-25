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

// renders the 3D scene
// --------------------
void renderScene(const Shader& shader, vector <unsigned int>& Texture,bool renderDepth)
{
    if (renderDepth == false) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture[0]);
    }

    // floor
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // cubes
    if (renderDepth == false)
        glBindTexture(GL_TEXTURE_2D, Texture[1]);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 2.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    if (renderDepth == false)
        glBindTexture(GL_TEXTURE_2D, Texture[2]);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 1.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 2.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 50.0f), glm::normalize(glm::vec3(0.5, 1.0, 0.5)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(-3.0f, 3.0f, 6.0));
    model = glm::translate(model, glm::vec3(-sin(glfwGetTime()) * 2.5f, 3.0f, cos(glfwGetTime()) * 2.5f));
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 100.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sin(glfwGetTime()) * 1.5f - 0.25f, sin(glfwGetTime() / 5.0f) + 2.5f, cos(glfwGetTime()) * 2.0f - 0.25f));
    model = glm::rotate(model, glm::radians((float)-glfwGetTime() * 300.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.25f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sin(glfwGetTime()) * 3.0f, cos(glfwGetTime()) * 2.0f, 5.0 + cos(glfwGetTime()) * 1.0f));
    model = glm::scale(model, glm::vec3(0.25f));
    shader.setMat4("model", model);
    renderCube();

    if (renderDepth == false)
        glBindTexture(GL_TEXTURE_2D, Texture[4]);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 3.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 2.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, 1.0f, -4.0));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    renderCube();
}

// renders the 3D model
// --------------------
void renderModel(Shader& shader, vector <Model>& ModelList, unsigned int& depthMaP, bool& renderDepth)
{
    // model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 1.0f, -3.0f));
    model = glm::rotate(model, glm::radians((float)-glfwGetTime() * 300.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.15f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[0].DrawShadowCast(shader, depthMaP);
    else
        ModelList[0].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[0].DrawShadowCast(shader, depthMaP);
    else
        ModelList[0].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-4.0f, -0.5f, 1.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    model = glm::scale(model, glm::vec3(0.15f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[1].DrawShadowCast(shader, depthMaP);
    else
        ModelList[1].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -0.5f, 1.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    model = glm::scale(model, glm::vec3(0.1f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[1].DrawShadowCast(shader, depthMaP);
    else
        ModelList[1].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, -0.5f, 5.0f));
    model = glm::rotate(model, glm::radians(0.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    model = glm::scale(model, glm::vec3(0.1f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[1].DrawShadowCast(shader, depthMaP);
    else
        ModelList[1].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -0.5f, -4.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[2].DrawShadowCast(shader, depthMaP);
    else
        ModelList[2].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, -0.5f, -1.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[2].DrawShadowCast(shader, depthMaP);
    else
        ModelList[2].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, -0.5f, 3.0f));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[2].DrawShadowCast(shader, depthMaP);
    else
        ModelList[2].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -0.5f, -1.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[3].DrawShadowCast(shader, depthMaP);
    else
        ModelList[3].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 3.0f));
    model = glm::rotate(model, glm::radians((float)-glfwGetTime() * 300.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    model = glm::scale(model, glm::vec3(0.25f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[3].DrawShadowCast(shader, depthMaP);
    else
        ModelList[3].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.5f, 0.2f, -1.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[4].DrawShadowCast(shader, depthMaP);
    else
        ModelList[4].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(5.5f, -0.2f, -2.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
    model = glm::scale(model, glm::vec3(0.25f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[4].DrawShadowCast(shader, depthMaP);
    else
        ModelList[4].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.5f, 0.2f, -2.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[4].DrawShadowCast(shader, depthMaP);
    else
        ModelList[4].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.75f, -0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[5].DrawShadowCast(shader, depthMaP);
    else
        ModelList[5].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.75f, -0.5f, 2.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    model = glm::scale(model, glm::vec3(0.25f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[5].DrawShadowCast(shader, depthMaP);
    else
        ModelList[5].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.75f, 0.5f, 1.0f));
    model = glm::rotate(model, glm::radians((float)glfwGetTime() * 50.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    model = glm::scale(model, glm::vec3(0.15f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[6].DrawShadowCast(shader, depthMaP);
    else
        ModelList[6].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.5f, -0.75f));
    model = glm::rotate(model, glm::radians(-(float)glfwGetTime() * 50.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
    model = glm::scale(model, glm::vec3(0.15f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[6].DrawShadowCast(shader, depthMaP);
    else
        ModelList[6].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, -0.5f, -2.0f));
    model = glm::scale(model, glm::vec3(0.075f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[7].DrawShadowCast(shader, depthMaP);
    else
        ModelList[7].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(180.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    model = glm::scale(model, glm::vec3(0.075f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[7].DrawShadowCast(shader, depthMaP);
    else
        ModelList[7].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -0.25f, -8.0f));
    model = glm::scale(model, glm::vec3(0.002f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[8].DrawShadowCast(shader, depthMaP);
    else
        ModelList[8].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -0.85f, 4.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[9].DrawShadowCast(shader, depthMaP);
    else
        ModelList[9].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-4.0f, -0.85f, 6.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[9].DrawShadowCast(shader, depthMaP);
    else
        ModelList[9].Draw(shader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, -0.5f, -6.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    model = glm::scale(model, glm::vec3(0.0075f));
    shader.setMat4("model", model);
    if (renderDepth == false)
        ModelList[10].DrawShadowCast(shader, depthMaP);
    else
        ModelList[10].Draw(shader);
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
void renderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
             // bottom face
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
              1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
              1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
             -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             // top face
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
              1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
              1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
              1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}