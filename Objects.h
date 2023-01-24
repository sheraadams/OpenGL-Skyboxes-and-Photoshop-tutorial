#pragma once
#include <glad/glad.h>

class Objects
{
public:
    GLuint VAO;
    GLuint VBO;
    Objects();
    void link(GLsizeiptr size, GLfloat* vertices);
    void bind();
    void Delete();
};
