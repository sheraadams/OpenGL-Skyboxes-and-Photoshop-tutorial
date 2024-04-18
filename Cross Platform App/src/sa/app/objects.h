#pragma once
#include <glad/glad.h>
#include "camera.h"
class Objects
{
public:

    GLuint VAO, VBO;

    Objects();
    void link(GLsizeiptr size, GLfloat* vertices);
    void link5(GLsizeiptr size, GLfloat* vertices);
    void skybox(GLsizeiptr size, GLfloat* vertices);
    void bind();
    void bindSkybox();
    void clear();
};