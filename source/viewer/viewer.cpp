#include "viewer.hpp"

void Viewer::render()
{
    while(!glfwWindowShouldClose(window))
    {
        process_input(window);

        glClearColor(red,green,blue,alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProg);

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0,3);

        glUseProgram(shaderProg2);

        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0,3);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
}