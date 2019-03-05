// Shader.h
// Author: Bairuo

#ifndef SHADER_H_
#define SHADER_H_

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;

    Shader();

    Shader(const GLchar *vertexPath, const GLchar *fragmentPath);
    void Use();
    void Stop();
    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void Set2f(const std::string &name, float v0, float v1) const;
    void Set3f(const std::string &name, float v0, float v1, float v2) const;
    void Set3f(const std::string &name, const glm::vec3 &value) const;
    void Set4f(const std::string &name, float v0, float v1, float v2, float v3) const;
    void SetMat4(const std::string &name, glm::mat4 matrix) const;
};



#endif // SHADER_H
