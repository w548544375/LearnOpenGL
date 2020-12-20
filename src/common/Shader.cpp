#include "Shader.h"

Shader::Shader(const char * vertexPath,const char * fragmentPath)
{
   std::string vertexString,fragmentString;
   try
   {
        readFile(vertexPath,&vertexString);
        readFile(fragmentPath,&fragmentString);
        const char * vertexCode = vertexString.c_str();
        const char * fragCode = fragmentString.c_str();
        buildProgram(vertexCode,fragCode);
   }
   catch(const std::exception& e)
   {
       std::cerr << e.what() << '\n';
   }
   

   
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::readFile(const char * path,std::string * outStr)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        outStr->append(shaderStream.str());
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(e.what());
    }
    
}

void Shader::buildProgram(const char * vertexCode,const char * fragCode)
{
    unsigned int vertexId,fragId;
    // vertex shader
    vertexId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexId,1,&vertexCode,NULL);
    glCompileShader(vertexId);
    char info[1024];
    memset(info,0,sizeof(info));
    if(!checkShader(vertexId,info))
    {
        std::string str;
        str.append("VERTEX::ERROR:");
        str.append(info);
        //std::cout << str.c_str() << std::endl;
        throw std::runtime_error(str.c_str());
    }
    // fragment shader
    fragId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragId,1,&fragCode,NULL);
    glCompileShader(fragId);
    char fragInfo[1024];
    memset(fragInfo,0,sizeof(fragInfo));
    if(!checkShader(fragId,fragInfo))
    {
        std::string str;
        str.append("FRAGMENT::ERROR:");
        str.append(fragInfo);
        //std::cout << str.c_str() << std::endl;
        throw std::runtime_error(str.c_str());
    }
    // shader prorgam
    ID = glCreateProgram();
    glAttachShader(ID,vertexId);
    glAttachShader(ID,fragId);
    glLinkProgram(ID);
    glDeleteShader(vertexId);
    glDeleteShader(fragId);
    char programInfo[1024];
    memset(programInfo,0,sizeof(programInfo));
    if(!checkProgram(programInfo))
    {
        std::string str;
        str.append(programInfo);
        throw std::runtime_error(str.c_str());
    }

}

bool Shader::checkShader(unsigned int shader_id, char * error)
{
    char info[512];
    int success;
    glGetShaderiv(shader_id,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        glGetShaderInfoLog(shader_id,sizeof(info),NULL,info);
        std::stringstream strStream;
        strStream << info;
        //std::cout << strStream.str() << std::endl;
        std::string errStr = strStream.str();
        const char * err = errStr.c_str();
        memcpy(error,err,strlen(err));
        return false;
    }
    return true;
}

bool Shader::checkProgram(char * error)
{
    char info[512];
    int success;
    glGetProgramiv(ID,GL_LINK_STATUS,&success);
    if(!success)
    {
        glGetProgramInfoLog(ID,sizeof(info),NULL,info);
        std::stringstream strStream;
        strStream << "PROGRAM::ERROR:" << info;
      //  std::cout << strStream.str() << std::endl;
        std::string str = strStream.str();
        const char * err = str.c_str();
        memcpy(error,err,strlen(err));
        return false;
    }
    return true;
}
