#include "gl_tools.h"
#include <iostream>
#include <fstream>
#include <sstream>

GLuint gl_tools::compile_shaders(const std::string& vertexPath, const std::string& fragmentPath, const std::string& sdf_code) {

    // VERTEX
    std::string vertexSource;
    std::ifstream vertexIn;
    vertexIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexIn.open(vertexPath);
        std::stringstream vertexStream;
        vertexStream << vertexIn.rdbuf();
        vertexIn.close();
        vertexSource = vertexStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "ERROR::VERTEX_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vertexShaderSource = vertexSource.c_str();
    

    unsigned int vertexShader;
    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::VERTEX_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else {
        std::cout << "Compiled : " << vertexPath << std::endl;
    }

    // FRAGMENT
    std::string fragmentSource;
    std::ifstream fragmentIn;
    fragmentIn.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        fragmentIn.open(fragmentPath);
        std::stringstream fragmentStream;
        fragmentStream << fragmentIn.rdbuf();
        fragmentIn.close();
        fragmentSource = fragmentStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "ERROR::FRAGMENT_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    // place sdf glsl code inside shader
    fragmentSource.replace(
        fragmentSource.find("#place_sdf_code_here"),
        sizeof("#place_sdf_code_here") - 1,
        sdf_code
    );

    const char* fragmentShaderSource = fragmentSource.c_str();

    unsigned int fragmentShader;

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::FRAGMENT_SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    else {
        std::cout << "Compiled : " << fragmentPath << std::endl;
    }


    // SHADER PROGRAM
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return ID;
}
