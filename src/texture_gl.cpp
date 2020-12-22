#include "common/Shader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "common/stb_image.h"

int createTexture(const char * path);


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
    float points[] ={     //tex coor 
        -0.5f,-0.5f,0.0f, 0.0f,1.0f,
         0.5f,-0.5f,0.0f, 1.0f,1.0f,
         0.5f, 0.5f,0.0f, 1.0f,0.0f,
        -0.5f, 0.5f,0.0f, 0.0f,0.0f 
    };

    unsigned int indices[] ={
        0,1,2,
        0,2,3
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
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);




    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    Shader shader("../src/base.vert","../src/base.frag");
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    // loading image
    int texture = createTexture("../src/aerial_grass_rock_diff_1k.jpg");
    int texture2 = createTexture("../src/Flames.jpg");
    shader.use();
    glUniform1i(glGetUniformLocation(shader.ID,"tex"),0);
    glUniform1i(glGetUniformLocation(shader.ID,"tex2"),1);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f,0.2f,0.2f,1.0f);

        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);

        shader.use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glDrawElements(GL_TRIANGLES,sizeof(indices) / sizeof(float),GL_UNSIGNED_INT,0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


int createTexture(const char * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}