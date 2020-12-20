#ifndef _SHADER_H
#define _SHADER_H
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
class Shader
{
    public:
        unsigned ID;
        void use();
        Shader(const char * vertexPath,const char * fragmentPath);
    private:
        void readFile(const char * path,std::string * str);
        void buildProgram(const char * vertexCode,const char * fragCode);
        bool checkShader(unsigned int shader_id,char * error);
        bool checkProgram(char * error);
};

#endif