#include "ShaderCompiler.h"
#include <iostream>
#include <fstream>
#include <sstream>

ShaderCompiler::ShaderCompiler(const char* vertexSource, const char* fragmentSource) {
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    programID = glCreateProgram();
    linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderCompiler::~ShaderCompiler() {
    glDeleteProgram(programID);
}

void ShaderCompiler::use() {
    glUseProgram(programID);
}

unsigned int ShaderCompiler::getProgramID() const {
    return programID;
}

std::string ShaderCompiler::readFile(const std::string& filePath) {
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::stringstream sstr;
    sstr << fileStream.rdbuf();
    fileStream.close();

    return sstr.str();
}

unsigned int ShaderCompiler::compileShader(GLenum type, const std::string& filePath) {
    unsigned int shader = glCreateShader(type);
    std::string sourceCode = readFile(filePath);
    std::cout << sourceCode << std::endl;
    const char* sourceCStr = sourceCode.c_str();
    glShaderSource(shader, 1, &sourceCStr, NULL);
    glCompileShader(shader);
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void ShaderCompiler::setVec3(const std::string& name, const glm::vec3& value) const {
    int uniformLoc = glGetUniformLocation(programID, name.c_str());
    glUniform3f(uniformLoc, value.x, value.y, value.z);
}

void ShaderCompiler::setMat4(const std::string& name, const glm::mat4& mat) const {
    int uniformLoc = glGetUniformLocation(programID, name.c_str());
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, &mat[0][0]);
}

void ShaderCompiler::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");
}

void ShaderCompiler::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
