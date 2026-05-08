#include <stdlib.h>
#include <iostream>
#include <vector>
#include "../dependancies/glad/glad.h"
#include <GLFW/glfw3.h>
#include "viewer/viewer.hpp"
#include "viewer/shader.hpp"
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "texture/stb_image.h"
//#include <GL/gl.h>
//#include <vulkan/vulkan.h>

#define DEBUG_MODE

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};


unsigned int indices[] =
{
    0,1,2,
    0,2,3
};

void testFunction()
{
    std::cout << std::filesystem::current_path() << std::endl;
    glm::vec4 vec(1.0, 0.0, 0.0, 1.0);
    glm::mat4 trans(1.0);
    trans =  glm::translate(trans, glm::vec3(1.0,1.0,0.0));
    vec = trans * vec;
    std::cout << "La translation de 1.0 0.0 0.0 1.0 par le vecteur 1.0 1.0 0.0 1.0 via un matrice de transfomartion donne : " << vec.x << vec.y << vec.z << std::endl;
}

void frameBuffer_resize(GLFWwindow * window, int width, int height)
{
    glViewport(0,0,width,height);
    #ifdef DEBUG_MODE
    std::cout << "Change windows value for width : " << std::to_string(width) << " & height : " << std::to_string(height) << std::endl; 
    #endif
}

void process_input(GLFWwindow * window, shader Shader)
{
    bool isSpacePressed = false;
    if(glfwGetKey(window , GLFW_KEY_SPACE) == GLFW_PRESS && !isSpacePressed)
    {
        isSpacePressed = true;
        int data[2];
        glGetIntegerv(GL_POLYGON_MODE, data);
        if(data[0] == GL_LINE)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if(glfwGetKey(window , GLFW_KEY_SPACE) == GLFW_RELEASE && isSpacePressed)
        isSpacePressed = false;

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window , GLFW_KEY_UP) == GLFW_PRESS)
    {
        float variation;
        Shader.getFloat("variation", variation);
        std::cout << "Variation par défaut est à : " << variation << std::endl;
        if(variation < 1.0f)
        {
            variation += 0.005f;
            Shader.setFloat("variation", variation);
        }
    }
        if(glfwGetKey(window , GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        float variation;
        Shader.getFloat("variation", variation);
        std::cout << "Variation par défaut est à : " << variation << std::endl;
        if(variation > 0.0f)
        {
            variation -= 0.005f;
            Shader.setFloat("variation", variation);
        }
    }
}

unsigned int generateTexture(std::string imagePath, bool flipImage = false)
{    
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nbChannels;

    if(flipImage)
        stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(imagePath.c_str(),  &width, &height, &nbChannels, 0);
    if(!data)
    {
        std::cout << "ERROR:IMAGE:PARSE:FAILED TO READ TEXTURE IMAGE" << std::endl;
    }

    if(std::string::npos != imagePath.find(".png"))
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

int initGLFW(GLFWwindow * window)
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    //glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
    window = glfwCreateWindow(1280, 720, "NE3DR", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, frameBuffer_resize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //Initialize Vertex Array Object and bind it
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    //Initialize Vertex Buffer Object with the vertices variable
    unsigned int VBO;
    glGenBuffers(1,&VBO);

    unsigned int EBO;
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Postion Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);  
    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture Coordinate Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture = generateTexture("../source/texture/images/wall.jpg");
    unsigned int texture2 = generateTexture("../source/texture/images/awesomeface.png", true);

    shader Shader("../source/viewer/vertex_shader.glsl", "../source/viewer/fragment_shader.glsl");

    Shader.use();
    glUniform1i(glGetUniformLocation(Shader.getID(), "ourTexture"), 0);
    Shader.setInt("ourTexture2", 1);

    float red = 0.2,green = 0.4,blue = 0.3,alpha = 1.;
    while(!glfwWindowShouldClose(window))
    {
        process_input(window, Shader);

        glClearColor(red,green,blue,alpha);
        glClear(GL_COLOR_BUFFER_BIT);


        //Shader.setFloat("h_offset", 0.0f);

        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0,3);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

    glfwTerminate();
    return 0;
}

int main(int argc, char ** argv)
{
    #ifdef DEBUG_MODE
    testFunction();
    #endif
    GLFWwindow * window = nullptr;
    if(initGLFW(window))
    {
        std::cout << "Program will close as an error happened during window creation" << std::endl;
    }
    std::cout << "Program closing properly with no issues" << std::endl;
    return 0;
}