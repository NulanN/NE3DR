#include <stdlib.h>
#include <iostream>
#include <vector>
#include "../../dependancies/glad/glad.h"
#include <GLFW/glfw3.h>

class Viewer
{
private:
    Viewer(GLFWwindow * window);
    ~Viewer();

    std::vector<unsigned int>_VAOs;
    std::vector<unsigned int> _VBOs;
public:
    void render();
};