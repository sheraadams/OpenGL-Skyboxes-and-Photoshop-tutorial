#ifndef PEN_CLIP_H
#define PEN_CLIP_H
#include <glad/glad.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PenClip
{
private:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> uv;
    std::vector<glm::vec3> normals;
    const unsigned int X_SEGMENTS = 64 / 5; // dividing the counts by 5 to reduce the polygon count of my pen
    const unsigned int Y_SEGMENTS = 64 / 5;
    const float PI = 3.14159265359f;
    std::vector<unsigned int> indices;
    GLuint VBO, EBO;
    float radius = 1.0f;
    int sectorCount = 36 / 5;
    int stackCount = 18 / 5;
    unsigned int VAO = 0;
    unsigned int indexCount;

public:
    // function to clear the VAO, EBO, VBO
    ~PenClip()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    // function to create VAO, VBO, EBO
    PenClip()
    {
        if (VAO == 0)
        {
            glGenVertexArrays(1, &VAO);

            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
                {
                    float xSegment = (float)x / (float)X_SEGMENTS;
                    float ySegment = (float)y / (float)Y_SEGMENTS;

                    // this is where I have modified a sphere to create elongated spheres that give the appearance of a pen shape

                    // the multiplication elongates or truncates the shape.
                    // the addition or subtraction shifts the shape up and down like a translation
                    // the numbers here are used to create a very small pen clip object
                    float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * .125 + .25;
                    float yPos = std::cos(ySegment * PI) * .01 + 1.23;
                    float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI) * .0083 + 1.17;

                    positions.push_back(glm::vec3(xPos, yPos, zPos));
                    uv.push_back(glm::vec2(xSegment, ySegment));
                    normals.push_back(glm::vec3(xPos, yPos, zPos));
                }
            }
            bool oddRow = false;
            for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
            {
                if (!oddRow)
                {
                    for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
                    {
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                }
                else
                {
                    for (int x = X_SEGMENTS; x >= 0; --x)
                    {
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                    }
                }
                oddRow = !oddRow;
            }
            indexCount = static_cast<unsigned int>(indices.size());
            std::vector<float> data;
            for (unsigned int i = 0; i < positions.size(); ++i)
            {

                data.push_back(positions[i].x);
                data.push_back(positions[i].y);
                data.push_back(positions[i].z);
                if (normals.size() > 0)
                {
                    data.push_back(normals[i].x);
                    data.push_back(normals[i].y);
                    data.push_back(normals[i].z);
                }
                if (uv.size() > 0)
                {
                    data.push_back(uv[i].x);
                    data.push_back(uv[i].y);
                }
            }
            // bind the vao, vbo, ebo using the vectors that were just created
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
            // there are 3 positions, 2 uv coords, 3 normals and this will be the stride
            unsigned int stride = (3 + 2 + 3) * sizeof(float);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        }
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
    }
    // function to draw the shape using GL_TRIANGLES
    void Draw()
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

};


#endif
