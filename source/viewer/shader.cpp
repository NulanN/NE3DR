#include "shader.hpp"

shader::shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream vFragmentFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vFragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        vFragmentFile.open(fragmentPath);
        std::stringstream vShaderStream, vFragmentStream;
        vShaderStream << vShaderFile.rdbuf();
        vFragmentStream << vFragmentFile.rdbuf();

        vShaderFile.close();
        vFragmentFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = vFragmentStream.str();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* vFragmentCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    //vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode,NULL);
    glCompileShader(vertex);
    //print error if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER:VERTEX:COMPILATION ERROR\n" << infoLog << "\n" << std::endl;
    }
    
    //fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1 , &vFragmentCode, NULL);
    glCompileShader(fragment);
    //print error if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER:FRAGMENT:COMPILATION ERROR\n" << infoLog << "\n" << std::endl;
    }

    //shader Program
    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);
    //print error if any
    glGetProgramiv(_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(_id, 512, NULL, infoLog);
        std::cout << "ERROR:PROGRAM:LINK:ERROR\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void shader::use()
{
    glUseProgram(_id);
}

void shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value); 
}
void shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value); 
}
void shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value); 
}
void shader::getFloat(const std::string &name, float &value)
{
    glGetUniformfv(_id, glGetUniformLocation(_id, name.c_str()), &value);
}

void shader::getMatrix4(const std::string &name, glm::mat4 &value)
{
    glUniformMatrix4fv(_id, glGetUniformLocation(_id, name.c_str()), false ,&value[0][0]);
}