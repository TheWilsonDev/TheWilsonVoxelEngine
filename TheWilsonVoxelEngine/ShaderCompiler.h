#pragma once
#include <GL/glew.h>
#include <glm.hpp>
#include <string>

class ShaderCompiler {
public:
    ShaderCompiler(const char* vertexSource, const char* fragmentSource);
    ~ShaderCompiler();

    void use();
    unsigned int getProgramID() const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
    unsigned int programID;

    unsigned int compileShader(GLenum type, const std::string& source);
    void linkProgram(unsigned int vertexShader, unsigned int fragmentShader);
    void checkCompileErrors(unsigned int shader, std::string type);
    std::string readFile(const std::string& filePath);
};
