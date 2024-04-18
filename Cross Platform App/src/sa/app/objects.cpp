#include "objects.h"

Objects::Objects()
{

}
// function to link for objects with 3 normal,  2 tex coords
void Objects::link5(GLsizeiptr size, GLfloat* vertices)
{
    // generate vao
    glGenVertexArrays(1, &VAO);
    // generate vao
    glGenBuffers(1, &VBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per position,  and 2 per tex coord = 5 floats
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(VAO);
}
// function to link for objects with 3 normal, 3 position, 2 tex coords
void Objects::link(GLsizeiptr size, GLfloat* vertices)
{
    // generate vao
    glGenVertexArrays(1, &VAO);
    // generate vao
    glGenBuffers(1, &VBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per position, 3 floats per normal, and 2 per tex coord = 8 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(VAO);
}
// function to link for skybox or objects that use only 3 positions
void Objects::skybox(GLsizeiptr size, GLfloat* vertices)
{
    // generate vao
    glGenVertexArrays(1, &VAO);
    // generate vao
    glGenBuffers(1, &VBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per position, 3 floats per normal, and 2 per tex coord = 8 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(VAO);
}
// function to draw
void Objects::bind()
{
    // draw the object with gl triangles
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // unbind the vao
    glBindVertexArray(0);
}
// function to draw skybox
void Objects::bindSkybox()
{
    // draw the object with gl triangles
    glDrawArrays(GL_TRIANGLES, 0, 72);
    // unbind the vao
    glBindVertexArray(0);
}
// function to clear vao and vbos
void Objects::clear()
{
    // delete the vao and vbo
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}