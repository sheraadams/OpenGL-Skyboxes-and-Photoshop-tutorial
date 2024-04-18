#pragma once
#include <glad/glad.h>
#include<vector>
#include<string>
#include <iostream> 
#include <cstdlib>   

class Textures
{
public:
    Textures();
    bool createRepeatTexture(const char* filename, GLuint& textureId);
    unsigned int loadTexture(const char* path);
    void destroyTexture(GLuint textureId);
    unsigned int loadCubemap(std::vector<std::string> faces);
    void flipImageVertically(unsigned char* image, int width, int height, int channels);
    void flipImageHorizontally(unsigned char* image, int width, int height, int channels);
};