#include "common/Shader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "common/stb_image.h"
// 定义正方体的顶点数据
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
unsigned int provideVBO();
unsigned int provideVAO(unsigned int vbo);
void placeCube(Shader & shader);
void placeLight(Shader & shader);
int createTexture(const char * path,bool flip);
glm::vec3 lightPos(5.0f, 2.0f,3.0f); 
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800,600, "基础光照", NULL, NULL);
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
    unsigned int cubeVBO = provideVBO();
    unsigned int cubeVAO = provideVAO(cubeVBO);
    unsigned int lightVAO = provideVAO(cubeVBO);
    
    Shader lightShader("../shaders/white_light.vert","../shaders/white_light.frag");
    Shader cubeShader("../shaders/light_simple.vert","../shaders/phong_material.frag");

    int texture = createTexture("../src/container2.png",false);
    int specTexture = createTexture("../src/lighting_maps_specular_color.png",false);
    int emissionTexture = createTexture("../src/matrix.jpg",false);
    cubeShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,specTexture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D,emissionTexture);

    glEnable(GL_DEPTH_TEST);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(lightVAO);
        lightShader.use();
        placeLight(lightShader);
        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);


        glBindVertexArray(cubeVAO);
        cubeShader.use();
        placeCube(cubeShader);

        glDrawArrays(GL_TRIANGLES,0,36);
        glBindVertexArray(0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &cubeVBO);

    glfwTerminate();
    return 0;
}

unsigned int provideVBO()
{
    unsigned int cubeVBO;
    glGenBuffers(1,&cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER,cubeVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    return cubeVBO;
}


unsigned int provideVAO(unsigned int vbo)
{
    unsigned int vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(float) * 8,0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(float) * 8,(void *)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(float) * 8,(void *)(sizeof(float) * 6));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    return vao;
}


void placeLight(Shader & shader)
{
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //model      = glm::rotate(model,glm::radians(-30.0f),glm::vec3(1.0f,0.0f,0.0f));
        model      = glm::scale(model,glm::vec3(0.2f,0.2f,0.2f));
        model      = glm::translate(model,lightPos);
        view       = glm::translate(view,glm::vec3(0.0f,0.0f,-4.0f));
        projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID,"model"),1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID,"view"),1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID,"projection"),1,GL_FALSE,glm::value_ptr(projection));
}


void placeCube(Shader & shader)
{
        glm::mat4 model      = glm::mat4(1.0f);
        glm::mat4 view       = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        float time = glfwGetTime();
        model      = glm::rotate(model,time,glm::vec3(0.8f,0.7f,0.6f)); 
        glm::mat3 normVec = glm::mat3(glm::transpose(glm::inverse(model)));
        view       = glm::translate(view,glm::vec3(0.0f,0.0f,-4.0f));
        projection = glm::perspective(glm::radians(45.0f),800.0f/600.0f,0.1f,100.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.ID,"model"),1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID,"view"),1,GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID,"projection"),1,GL_FALSE,glm::value_ptr(projection));
        glUniformMatrix3fv(glGetUniformLocation(shader.ID,"normalDirection"),1,GL_FALSE,glm::value_ptr(normVec));
        glUniform1i(glGetUniformLocation(shader.ID,"material.diffuseTexture"),0);
        glUniform1i(glGetUniformLocation(shader.ID,"material.specularTexture"),1);
        glUniform1i(glGetUniformLocation(shader.ID,"material.emissionTexture"),2);
        //glUniform3f(glGetUniformLocation(shader.ID,"material.diffuse"),1.0f, 0.5f, 0.31f);
       // glUniform3f(glGetUniformLocation(shader.ID,"material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(shader.ID,"material.shininess"), 16.0f);
        glUniform3fv(glGetUniformLocation(shader.ID,"viewPos"),1,glm::value_ptr(model));

        glm::vec3 lightColor;
        lightColor.x = sin(glfwGetTime() * 2.0f);
        lightColor.y = sin(glfwGetTime() * 0.7f);
        lightColor.z = sin(glfwGetTime() * 1.3f);

        glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响
        glm::vec3 whiteLight = glm::vec3(1.0f);
        glUniform3fv(glGetUniformLocation(shader.ID,"light.position"),1,glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(shader.ID,"light.diffuseTexture"),1,glm::value_ptr(ambientColor));
        glUniform3fv(glGetUniformLocation(shader.ID,"light.diffuse"),1,glm::value_ptr(whiteLight)); // 将光照调暗了一些以搭配场景
        glUniform3f(glGetUniformLocation(shader.ID,"light.specular"), 1.0f, 1.0f, 1.0f); 
}


int createTexture(const char * path,bool flip)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(flip);
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