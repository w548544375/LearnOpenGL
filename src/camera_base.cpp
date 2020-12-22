#include "common/Shader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "common/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int createTexture(const char * path,bool flip);

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
    window = glfwCreateWindow(800,600, "3D展示", NULL, NULL);
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
    float points[] ={      // 坐标
      -0.5f, 0.5f, 0.5f, 0.0f,1.0f,
       0.5f, 0.5f, 0.5f, 1.0f,1.0f,
       0.5f,-0.5f, 0.5f, 1.0f,0.0f,
      -0.5f,-0.5f, 0.5f, 0.0f,0.0f,
      -0.5f, 0.5f,-0.5f, 0.0f,1.0f,
       0.5f, 0.5f,-0.5f, 1.0f,1.0f,
       0.5f,-0.5f,-0.5f, 1.0f,0.0f,
      -0.5f,-0.5f,-0.5f, 0.0f,0.0f
    };

    unsigned int indices[] ={
        0,1,2,
        0,2,3,
        1,5,6,
        1,6,2,
        5,4,7,
        5,7,6,
        0,4,5,
        0,5,1,
        0,4,7,
        0,7,3,
        3,7,6,
        3,6,2
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
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

    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5 * sizeof(float),(void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    #ifdef _DEBUG_
      Shader shader("shaders/3d.vert","shaders/3d.frag");
    #else
      Shader shader("../shaders/3d.vert","../shaders/3d.frag");
    #endif
    shader.use();
    #ifdef _DEBUG_
      int texture = createTexture("src/Flames.jpg",false);
    #else
      int texture = createTexture("../src/timg.jpg",false);
    #endif
    glEnable(GL_DEPTH_TEST);
    glUniform1i(glGetUniformLocation(shader.ID,"tex"),0);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);

        shader.use();

        float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        glm::mat4 model =      glm::mat4(1.0f);
        glm::mat4 view  =      glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
       
        view       = glm::lookAt(glm::vec3(camX,0.0f,camZ),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
        projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);

        unsigned int modelLoc = glGetUniformLocation(shader.ID,"model");
        unsigned int viewLoc  = glGetUniformLocation(shader.ID,"view");
        unsigned int projLoc  = glGetUniformLocation(shader.ID,"projection");

        
        glUniformMatrix4fv(viewLoc,1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(projLoc,1,GL_FALSE,glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        for(unsigned int i = 0;i < 10;i++)
        {
          model = glm::translate(model,cubePositions[i]);
          float angle = 20.0f * i;
          
          model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          glUniformMatrix4fv(modelLoc,1,GL_FALSE,glm::value_ptr(model));
          glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
        }
        
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


int createTexture(const char * path,bool flip)
{
    unsigned int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width,height,nrChannels;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char * data = stbi_load(path,&width,&height,&nrChannels,0);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}