#include "common/Shader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800,600, "OpengGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
    {
        std::cout << "init glew failed" << std::endl;
    }
    glViewport(0,0,800,600);
    std::cout << glGetString(GL_VERSION) << std::endl;
    // init view
    float points[] ={      // yanse
        -1.0f,-0.5f,0.0f,  1.0f,0.0f,0.0f,
        -0.5f, 0.5f,0.0f,  0.0f,1.0f,0.0f,
         0.0f,-0.5f,0.0f,  0.0f,0.0f,1.0f,
         1.0f,-0.5f,0.0f,  0.0f,1.0f,0.0f,
         0.5f, 0.5f,0.0f,  1.0f,0.0f,0.0f
    };

    unsigned int indices[] ={
        0,1,2,
        2,3,4
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO; // huancun duxiang 

    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);

    glGenBuffers(1,&EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    glBufferData(GL_ARRAY_BUFFER,sizeof(points),points,GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    //
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),(void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    Shader shader("../src/base.vert","../src/base.frag");
    Shader shader1("../src/base.vert","../src/yellow.frag");
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        shader1.use();
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
        
        shader.use();
        // float timeValue = glfwGetTime();
        // float greenColor = (sin(timeValue)/2.0f) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shader.ID,"vertexColor");
        // glUniform4f(vertexColorLocation,0.0f,greenColor,0.0f,1.0f);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void *)(sizeof(unsigned int) * 3));
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}