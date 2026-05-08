#include "../../dependancies/glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader
{
private:
    unsigned int _id;

public:
    shader(const char* vertexPath, const char* fragmentPath);

    void use();

    unsigned int getID() {return _id;}

    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void getFloat(const std::string &name, float &value);
};