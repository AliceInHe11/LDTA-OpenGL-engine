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

// camera
Camera camera(glm::vec3(-3.0f, 3.0f, 3.0f));
float lastX = (float)ScreenValue.SCR_WIDTH / 2.0;
float lastY = (float)ScreenValue.SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void setWindowPosition(ScreenInfo ScreenValue, WindowsPosition &WindowsPos) {
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    if (mode->width >= ScreenValue.SCR_WIDTH && mode->height >= ScreenValue.SCR_HEIGHT) {
        WindowsPos.WINDOWS_POS_X = mode->width / 2 - ScreenValue.SCR_WIDTH / 2;
        WindowsPos.WINDOWS_POS_Y = mode->height / 2 - ScreenValue.SCR_HEIGHT / 2;
    }
    else
        return;
}

// config a window mode and resolution
// -----------------------------------
void readWindowConfig(const std::string& filename, ScreenInfo &ScreenValue) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line))
            ScreenValue.SCREEN_MODE = std::stoi(line);
        if (std::getline(file, line))
            ScreenValue.SCR_WIDTH = std::stoi(line);
        if (std::getline(file, line))
            ScreenValue.SCR_HEIGHT = std::stoi(line);
        file.close();

        if (ScreenValue.SCR_WIDTH <= 0 || ScreenValue.SCR_HEIGHT <= 0) {
            cout << "INVLID RESOLUTION VALUE - RESTORE TO DEFAULT RESOLUTION (800x600).";
            ScreenValue.SCR_WIDTH = 800;
            ScreenValue.SCR_HEIGHT = 600;
        }
        if (ScreenValue.SCREEN_MODE < 0 || ScreenValue.SCREEN_MODE > 1) {
            cout << endl << "INVLID WINDOW MODE VALUE - RESTORE TO DEFAULT WINDOW MODE (WINDOWSCREEN).";
            ScreenValue.SCREEN_MODE = 0;
        }

        cout << "Window Mode: ";
        if (ScreenValue.SCREEN_MODE == 1)
            cout << "FULLSCREEN";
        else
            cout << "WINDOWSCREEN";
        cout << endl << "Resolution: " << ScreenValue.SCR_WIDTH << "x" << ScreenValue.SCR_HEIGHT << endl;

    }
    else {
        cout << "CAN NOT OPEN CONFIG FILE - USING DEFAULT CONFIG.";
        cout << endl << "WINDOW MODE: WINDOWSCREEN";
        cout << endl << "RESOLUTION: 800x600" << endl;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera.ProcessKeyboard(FORWARD, deltaTime * 5);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera.ProcessKeyboard(BACKWARD, deltaTime * 5);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera.ProcessKeyboard(LEFT, deltaTime * 5);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera.ProcessKeyboard(RIGHT, deltaTime * 5);
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                camera.ProcessKeyboard(UP, deltaTime * 5);
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                camera.ProcessKeyboard(DOWN, deltaTime * 5);
        }else
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera.ProcessKeyboard(FORWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera.ProcessKeyboard(BACKWARD, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera.ProcessKeyboard(LEFT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera.ProcessKeyboard(RIGHT, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                camera.ProcessKeyboard(UP, deltaTime);
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                camera.ProcessKeyboard(DOWN, deltaTime);
        }
        else {
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                camera.ProcessKeyboard(FORWARD, deltaTime * 2.5);
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                camera.ProcessKeyboard(BACKWARD, deltaTime * 2.5);
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                camera.ProcessKeyboard(LEFT, deltaTime * 2.5);
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                camera.ProcessKeyboard(RIGHT, deltaTime * 2.5);
            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
                camera.ProcessKeyboard(UP, deltaTime * 2.5);
            if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                camera.ProcessKeyboard(DOWN, deltaTime * 2.5);
        }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void changeLightPos(GLFWwindow* window, glm::vec3& lightPos) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        if (lightPos.y >= 20.0f)
            lightPos.y = 20.0f;
        else
            lightPos.y += 0.25f;

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        if (lightPos.y <= -0.5f)
            lightPos.y = -0.5f;
        else
            lightPos.y -= 0.25f;

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        if (lightPos.x >= 20.0f)
            lightPos.x = 20.0f;
        else
            lightPos.x += 0.25f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        if (lightPos.x <= -20.0f)
            lightPos.x = -20.0f;
        else
            lightPos.x -= 0.25f;
}

void changeLightInfo(GLFWwindow* window, glm::vec3& lightColor, float& ambientIntensity) {
    if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            if (lightColor.x <= 0.0f)
                lightColor.x = 0.0f;
            else
                lightColor.x -= 0.001f;

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            if (lightColor.y <= 0.0f)
                lightColor.y = 0.0f;
            else
                lightColor.y -= 0.001f;

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            if (lightColor.z <= 0.0f)
                lightColor.z = 0.0f;
            else
                lightColor.z -= 0.001f;

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            if (ambientIntensity <= 0.0f)
                ambientIntensity = 0.0f;
            else
                ambientIntensity -= 0.025f;
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
            if (lightColor.x >= 1.0f)
                lightColor.x = 1.0f;
            else
                lightColor.x += 0.001f;

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
            if (lightColor.y >= 1.0f)
                lightColor.y = 1.0f;
            else
                lightColor.y += 0.001f;

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
            if (lightColor.z >= 1.0f)
                lightColor.z = 1.0f;
            else
                lightColor.z += 0.001f;

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            if (ambientIntensity >= 10.0f)
                ambientIntensity = 10.0f;
            else
                ambientIntensity += 0.025f;
    }
}