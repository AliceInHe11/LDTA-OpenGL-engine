// camera
Camera camera(glm::vec3(-3.0f, 3.0f, 3.0f));
GLfloat lastX = (GLfloat)ScreenValue.SCR_WIDTH / 2.0;
GLfloat lastY = (GLfloat)ScreenValue.SCR_HEIGHT / 2.0;
bool firstMouse = true;

static void setWindowPosition(ScreenInfo ScreenValue, WindowsPosition &WindowsPos)
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    if (mode->width >= ScreenValue.SCR_WIDTH && mode->height >= ScreenValue.SCR_HEIGHT) 
    {
        WindowsPos.WINDOWS_POS_X = mode->width / 2 - ScreenValue.SCR_WIDTH / 2;
        WindowsPos.WINDOWS_POS_Y = mode->height / 2 - ScreenValue.SCR_HEIGHT / 2;
    }
    else
        return;
}

// config a window mode and resolution
// -----------------------------------
static void readWindowConfig(const std::string& filename, ScreenInfo &ScreenValue)
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

                if (key == "window_mode")
                    ScreenValue.SCREEN_MODE = std::stoi(getvalue);
                else
                if (key == "width_resolution")
                    ScreenValue.SCR_WIDTH = std::stoi(getvalue);
                else
                if (key == "height_resolution")
                    ScreenValue.SCR_HEIGHT = std::stoi(getvalue);

            }
        }

        file.close();

        if (ScreenValue.SCR_WIDTH <= 0 || ScreenValue.SCR_HEIGHT <= 0) 
        {
            std::cout << "INVLID RESOLUTION VALUE - RESTORE TO DEFAULT RESOLUTION (800x600).";
            ScreenValue.SCR_WIDTH = 800;
            ScreenValue.SCR_HEIGHT = 600;
        }
        if (ScreenValue.SCREEN_MODE < 0 || ScreenValue.SCREEN_MODE > 1) 
        {
            std::cout << std::endl << "INVLID WINDOW MODE VALUE - RESTORE TO DEFAULT WINDOW MODE (WINDOWSCREEN).";
            ScreenValue.SCREEN_MODE = 0;
        }

        std::cout << "Window Mode: ";
        if (ScreenValue.SCREEN_MODE == 1)
            std::cout << "FULLSCREEN";
        else
            std::cout << "WINDOWSCREEN";
        std::cout << std::endl << "Resolution: " << ScreenValue.SCR_WIDTH << "x" << ScreenValue.SCR_HEIGHT << std::endl;

    }
    else 
    {
        std::cout << "CAN NOT OPEN CONFIG FILE - USING DEFAULT CONFIG.";
        std::cout << std::endl << "WINDOW MODE: WINDOWSCREEN";
        std::cout << std::endl << "RESOLUTION: 800x600" << std::endl;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static void processInput(GLFWwindow* window)
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
        }
        else
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
        else 
        {
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
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
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
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever the key are press, this callback is called
// ----------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
        if(DynamicPos == true)     
            DynamicPos = false;   
        else
            DynamicPos = true;

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        camera.MouseSensitivity += 0.001f;
    else
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        if(camera.MouseSensitivity <= 0)
            camera.MouseSensitivity = 0.0f;
        else
            camera.MouseSensitivity -= 0.001f;

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        weaponsNum = 0;
        weaponsSelect = true;
    }
    else
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        weaponsNum = 1;
        weaponsSelect = true;
    }
    else
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        weaponsNum = 2;
        weaponsSelect = true;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        playerMovement = -1;
    else
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)

            playerMovement = 2;
        else
            playerMovement = -1;
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
            glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)

            playerMovement = 1;
        else
            playerMovement = -1;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (weaponsNum == 0) 
            weaponsSound = 0;
        else
        if (weaponsNum == 1)
            weaponsSound = 1;
        else
        if (weaponsNum == 2)
            weaponsSound = 2;
    }
    else
        weaponsSound = -1;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        weaponsTap = true;
}

void changeLightPos(GLFWwindow* window, glm::vec3& lightPos) 
{
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

void changeLightInfo(GLFWwindow* window, glm::vec3& lightColor, GLfloat& ambientIntensity)
{
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