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
