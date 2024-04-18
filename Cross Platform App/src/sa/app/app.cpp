// library includes
#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
// learnopengl resources includes
#include "shader.h"
#include "camera.h"
#include "model.h"
// shapes
#include "objects.h"
#include "texture.h"
#include "pen_accent.h"
#include "pen_body.h"
#include "pen_clip.h"
#include "pen_point.h"
#include <ft2build.h>
#include FT_FREETYPE_H

/* FUNCTIONS */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void RenderText(Shader& s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
void InitText();
/* FUNCTIONS */

#ifndef __APPLE__
void GetDesktopResolution(float& horizontal, float& vertical);
#endif

//variables to move objects for development
int t = 0;
int r = 0;
std::vector<unsigned int> textures;
float x = -.0f;
float y = -1.3f; // we are hiding the box in the desk for now
float z = -3.2f; // we do this setting y-position to -1.3 and the z position to -3.2
float xx = 0.0f;
float yy = 0.0f;
float zz = 0.0f;

struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
GLuint textVAO, textVBO;
bool renderText = true;

/* VARIABLES */
float speed = 45.0f; // light orbiting speed
float direction = -1.0; // used for light orbiting direction
bool lampIsOrbiting = true;
bool Keys[1024], firstMouse = true, onPerspective = true;
float SCR_WIDTH = 1000, SCR_HEIGHT = 900;
float lastX = (float)SCR_WIDTH / 2.0, lastY = (float)SCR_HEIGHT / 2.0;
float lastFrame = 0.0f, deltaTime = 0.0f;
GLfloat xoffset = 0.0f, yoffset = 0.0f;

/* VARIABLES */

// initialize the camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

#ifndef __APPLE__
/* FUNCTION TO MAKE THE PROGRAM FULLSCREEN */
void GetDesktopResolution(float& horizontal, float& vertical)
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    horizontal = desktop.right;
    vertical = desktop.bottom;
}
#endif
int main()
{
    // make the program fullscreen
#ifndef __APPLE__
    GetDesktopResolution(SCR_WIDTH, SCR_HEIGHT);
#endif
    /* GLFW INITIALIZE */
    glfwInit();

    // set the version to be used, I am using version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create the glfw window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Shera Adams: Final Project", NULL, NULL);
    // output error message if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // make the window the current context for the openGL application
    glfwMakeContextCurrent(window);
    // tell glfw to capture the mouse movement within the window and scroll movement within the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load openGL pointers and glad pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    InitText();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* LOAD SHADERS FROM FILES */
    Shader lightingShader("specular.vs", "specular.fs");
    Shader skyboxShader("skybox.vs", "skybox.fs");
    Shader pinkShader("glsl.vs", "light_pink.fs");
    Shader purpleShader("glsl.vs", "light_purple.fs");
    Shader textShader("TextShader.vs", "TextShader.fs");

    // PROJECTION FOR TEXT RENDERING
    glm::mat4 Text_projection = glm::ortho(0.0f, SCR_WIDTH, 0.0f, SCR_HEIGHT);
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(Text_projection));

    /* VERTICES */
    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,   // this is a modified cube z axis is my x axis and it is modified to be less tall
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  // left
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // right
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // back
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  // right
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // top
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  //bottom
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float compassVertices[] = {
            0.49f, -0.5f, .5f,     0.0f,  0.0f, -1.0f,     0.0f, 0.0f, // back rectangle
            0.5f, -0.5f, .5f,      0.0f, -1.0f,  0.0f,     0.0f, 1.0f,
            0.49f, 0.5f, .5f,      0.0f,  0.0f, -1.0f,     1.0f, 0.0f,
            0.5f, 0.5f, .5f,       0.0f,  0.0f, -1.0f,     1.0f, 1.0f,
            0.5f, -0.5f, .5f,      0.0f,  0.0f, -1.0f,     0.0f, 1.0f,

            0.49f, 0.5f, .5f,      0.0f,  0.0f,  1.0f,     1.0f, 0.0f,  // bottom
            0.0f, 0.0f, 1.0f,      0.0f,  0.0f,  1.0f,     0.5f, 0.5f,
            0.49f, -0.5f, .5f,     0.0f,  0.0f,  1.0f,     0.0f, 0.0f,

            0.49f, 0.5f, 0.5f,     0.0f,  0.0f,  1.0f,     1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,      0.0f,  0.0f,  1.0f,     0.5f, 0.5f,
            0.49f, 0.5f, .5f,      -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,

            0.5f, 0.5f, .5f,       -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,     // modified pyramid to shift .1 down (y axis modified after camera transformations)
            0.0f, 0.0f, 1.0f,      -1.0f,  0.0f,  0.0f,    0.5f, 0.5f,
            0.5f, 0.5f, .5f,       -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
            0.5f, -0.5f, .5f,      -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,      1.0f,  0.0f,  0.0f,     0.5f, 0.5f,
            0.5f, -0.5f, .5f,      1.0f,  0.0f,  0.0f,     0.0f, 0.0f,
            0.49f, -0.5f, .5f,     1.0f,  0.0f,  0.0f,     1.0f, 0.0f,
    };


    float boxVertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,  // left
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // back
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  // front
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // top
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,  //bottom
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    // light box positions
    glm::vec3 lightPositions[] = {
            glm::vec3(-3.0f, 0.0f, 3.0f),
            glm::vec3(-3.0f, 0.0f, -3.0f),
    };
    // lighting positions for our scene
    glm::vec3 pointLightPositions[] = {
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(1.0f, 2.0f, -3.0f),
            glm::vec3(-3.0f, 1.0f, -2.0f),
            glm::vec3(1.0f, 1.0f, 2.0f),
            glm::vec3(-0.0f, 2.0f, -2.0f),
            glm::vec3(2.0f, 2.0f, 3.0f),
            glm::vec3(0.0f, 3.0f, -2.0f),
            glm::vec3(-2.0f, 1.0f, -2.0f),
            glm::vec3(1.0f, 2.0f, -3.0f),
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(3.0f, 1.0f, 0.0f),
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(2.0f, 1.0f, -2.0f),
            glm::vec3(-1.0f, 2.0f, 2.0f),
            glm::vec3(2.0f, 1.0f, 2.0f),
            glm::vec3(3.0f, 3.0f, -5.0f),
            glm::vec3(0.0f, 2.0f, 2.0f),
            glm::vec3(2.0f, 2.0f, 3.0f),
    };
    /* TEXT RENDERING VAO-VBO */
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    /* TEXT RENDERING VAO-VBO*/

    /* LIGHT CUBE UNIFORM MATRICES */
    unsigned int lightCubeVAO, lightCubeVBO;
    // generate vao
    glGenVertexArrays(1, &lightCubeVAO);
    // generate vbo for the light cubes
    glGenBuffers(1, &lightCubeVBO);
    // bind buffer to the vertices and create a buffer the size of the vertices
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    // bind vbo attribute pointers to the vao
    // our vertices have 3 floats per vertex = 3 floats
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // create a uniform buffer object (ubo)
    unsigned int uniformBlockIndexPink = glGetUniformBlockIndex(pinkShader.ID, "Matrices");
    unsigned int uniformBlockIndexPurple = glGetUniformBlockIndex(purpleShader.ID, "Matrices");
    // link uniform block to shader
    glUniformBlockBinding(pinkShader.ID, uniformBlockIndexPink, 0);
    glUniformBlockBinding(purpleShader.ID, uniformBlockIndexPurple, 0);
    // create buffer
    unsigned int uboMatrices;
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // define the buffer range
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
    // set the projection matrix
    glm::mat4 projection = glm::perspective(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    /* LOAD TEXTURES */
    Textures texture;
    unsigned int redTexture = texture.loadTexture("resources/textures/class/red.jpg");
    unsigned int blueTexture = texture.loadTexture("resources/textures/class/blue.jpg");
    unsigned int lightgreyTexture = texture.loadTexture("resources/textures/class/lightgrey.jpg");
    unsigned int compassTexture = texture.loadTexture("resources/textures/class/protractor2.jpg");
    unsigned int skyboxTexture = texture.loadTexture("resources/textures/class/wall.png");
    unsigned int blackboardTexture = texture.loadTexture("resources/textures/class/blackboard.jpg");
    unsigned int groundTexture = texture.loadTexture("resources/textures/class/AdobeStock_321846439.jpg");
    unsigned int metalTexture = texture.loadTexture("resources/textures/class/metal.jpg");
    unsigned int deskTexture = texture.loadTexture("resources/textures/class/AdobeStock_372442505.jpg");
    unsigned int ballpointTexture = texture.loadTexture("resources/textures/class/1.jpg");
    unsigned int certificateTexture = texture.loadTexture("resources/textures/class/cert.jpg");
    unsigned int posterTexture = texture.loadTexture("resources/textures/class/poster.jpg");

    textures.push_back(blueTexture);
    textures.push_back(redTexture);
    textures.push_back(lightgreyTexture);
    textures.push_back(groundTexture);
    textures.push_back(metalTexture);
    textures.push_back(ballpointTexture);
    textures.push_back(compassTexture);

    /* CREATE A CUBEMAP SKYBOX */
    vector<std::string> faces
            {
                    "resources/textures/class/right.jpg", // right
                    "resources/textures/class/left.jpg", // left
                    "resources/textures/class/top.jpg", // top
                    "resources/textures/class/bottom.jpg", // bottom
                    "resources/textures/class/front.jpg", // front
                    "resources/textures/class/back.jpg", // back
            };
    unsigned int cubemapTexture = texture.loadCubemap(faces);

    // set the shader ints
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);

    /* SET THE PROJECTION AS PERSPECTIVE BY DEFAULT*/
    onPerspective = true;

    /* RENDER LOOP */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input throughout the loop
        processInput(window);
        int major = GLFW_VERSION_MAJOR;
        int minor = GLFW_VERSION_MINOR;
        int revision = GLFW_VERSION_REVISION;

        //debug
        //printf("GLFW version: %d.%d.%d\n", major, minor, revision);

        // set the background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // update the light position coordinates
        /****************************************************************/
        const float angularVelocity = glm::radians(45.0f);
        if (lampIsOrbiting)
        {
            glm::vec4 newPosition = glm::rotate(angularVelocity * deltaTime,
                                                glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(pointLightPositions[0], 1.0f);
            pointLightPositions[0].x = newPosition.x;
            pointLightPositions[0].y = newPosition.y;
            pointLightPositions[0].z = newPosition.z;

            glm::vec4 newPosition2 = glm::rotate(angularVelocity * deltaTime,
                                                 glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(pointLightPositions[1], 1.0f);
            pointLightPositions[1].x = newPosition.x;
            pointLightPositions[1].y = newPosition.y;
            pointLightPositions[1].z = newPosition.z;
        }

        /* LIGHTING SETTINGS FOR THE SCENE */
        /****************************************************************/
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);
        // directional light
        lightingShader.setVec3("dirLight.direction", 0.2f, 0.0f, 0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z );
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09f);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09f);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09f);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09f);
        lightingShader.setFloat("spotLight.quadratic", 0.032f);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        float ambient[] = { 0.5f, 0.5f, 0.5f, 1 };
        float diffuse[] = { 0.8f, 0.8f, 0.8f, 1 };
        float specular[] = { 1.0f, 1.0f, 1.0f, 1 };
        float shininess = 128;
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);

        /* INITIALIZE VARAIBLES */
        glm::mat4 projection, view, model;

        /* SET PROJECTION
        /****************************************************************/
        if (onPerspective)
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        if (!onPerspective)
            projection = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, 1.0f, 100.0f);

        /* SET SHADER */
        view = camera.GetViewMatrix();
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("model", model);

        /* RENDER SCENE */
        /* COMPASS */
        float angle = 0.0;
        Objects pyramid;
        pyramid.link(sizeof(compassVertices), compassVertices);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(240.0f), glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::translate(model, glm::vec3(-1.85f, -0.2f, -.536f)); //(forward .14, left .2, up..467) AFTER ROTATION
        model = glm::scale(model, glm::vec3(1.1f));
        model = glm::scale(model, glm::vec3(.75f, .75f, .25f));
        lightingShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, compassTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        /* CUTOUT BOX */
        glm::vec4 color1 = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spriteColor", color1);
        Objects box;
        box.link(sizeof(boxVertices), boxVertices);
        model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
        model = glm::translate(model, glm::vec3(0.0f, 1.65f, -3.240f));
        model = glm::scale(model, glm::vec3(9.0f, 6.5f, 9.0f));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, skyboxTexture);
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* all cube-like shapes are bound to one vao and are instanced to make the desk,
        books, ground, and blackboard */
        Objects book;
        book.link(sizeof(vertices), vertices);
        for (unsigned int i = 0; i <= 12; i++)
        {   /* translate everything down 1 */
            model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.00f));
            glm::mat4 model = glm::mat4(1.0f);
            if (i == 0)
            {   /* DESK */
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::scale(model, glm::vec3(1.8f, 1.2f, 1.5f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, deskTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 1)
            {   /* RIGHT STACK RED TOP BOOK */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(0.0f, .62f, 0.05f));
                model = glm::scale(model, glm::vec3(.4f, .035f, .6f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, redTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 2)
            {/* RIGHT STACK BLUE BOOK */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, -1.1f, 0.01f));
                model = glm::translate(model, glm::vec3(0.0f, .695f, 0.03f));
                model = glm::scale(model, glm::vec3(.4f, .035f, .4f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 3)
            {/* LEFT STACK BIG BOOK */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.5f, .64f, 0.0285f));
                model = glm::scale(model, glm::vec3(.4f, .07f, .5f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, lightgreyTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 4)
            { /* BIG BOOK SIDE BINDING */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.5f, .605f, 0.03f));
                model = glm::scale(model, glm::vec3(.41f, .01f, .51f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 5)
            {  /* BIG BOOK TOP BINDING */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.50f, .678f, 0.03f));
                model = glm::scale(model, glm::vec3(.41f, .01f, .51f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 6)
            {  /* BIG BOOK BOTTOM BINDING */
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.01f, 0.01f, 0.01f));
                model = glm::translate(model, glm::vec3(-.3f, .639f, 0.03f));
                model = glm::scale(model, glm::vec3(.01f, .07f, .51f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, blueTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 7)
            {  /* TOP RED LEFT STACKED BOOK */
                angle = 7.0f;
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.00f, -.08f, 0.00f));
                model = glm::translate(model, glm::vec3(-.50f, .7f, 0.05));
                model = glm::scale(model, glm::vec3(.31f, .03f, .41f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, redTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 8)
            {  /* RIGHT STACK MIDDLE BOOK GREY */
                angle = -9;
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(0.0f, -1.0f, -2.00f));
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.00f, 1.00f, 0.0f));
                model = glm::translate(model, glm::vec3(0.0f, .66f, 0.03f));
                model = glm::scale(model, glm::vec3(.4f, .035f, .6f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, lightgreyTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 9)
            {  /* GROUND */
                angle = 0.0f;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(0.001f, 0.001f, 0.0001f));
                model = glm::translate(model, glm::vec3(0.0f, -1.6f, -3.25f));
                model = glm::scale(model, glm::vec3(9.0f, 0.03f, 9.0f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, groundTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 10)
            {  /* BLACKBOARD */
                angle = 0.0f;
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(3.01f, 0.01f, -12.01f));
                model = glm::translate(model, glm::vec3(0.0f, 1.5f, -7.75f));
                model = glm::scale(model, glm::vec3(5.5f, 4.0f, .03f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textures[t]);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else if (i == 11)
            {  /* CERTIFICATE */
                angle = 0.0f;
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(3.01f, 0.01f, -12.01f));
                model = glm::translate(model, glm::vec3(4.468f, 1.5f, -4.0f));
                model = glm::scale(model, glm::vec3(.02f, .9f, .9f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, certificateTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else
            {  /* POSTER */
                angle = 0.0f;
                model = glm::mat4(1.0f);
                model = glm::rotate(model, glm::radians(angle), glm::vec3(3.01f, 0.01f, -12.01f));
                model = glm::translate(model, glm::vec3(-4.468f, 1.5f, -4.0f));
                model = glm::scale(model, glm::vec3(.02f, 1.7f, 1.7f));
                lightingShader.setMat4("model", model);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, posterTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        // unbind the vao after drawing all objects
        glBindVertexArray(0);

        // bind metal texture and use it on the next three objects
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, metalTexture);

        // set model and translate the next three objects down 2 and back 3
        model = glm::mat4(0.5f);
        model = glm::translate(model, glm::vec3(0.0f, -2.005f, -3.80f));

        // draw the penbody shape
        lightingShader.setMat4("model", model);
        PenBody penBody;
        penBody.Draw();

        // draw the penclip shape
        lightingShader.setMat4("model", model);
        PenClip penClip;
        penClip.Draw();

        // draw the sphere accent shape
        lightingShader.setMat4("model", model);
        PenAccent sphereAccent;
        sphereAccent.Draw();

        // bind the ballpoint texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ballpointTexture);

        // set the model
        lightingShader.setMat4("model", model);
        model = glm::mat4(0.5f);
        // draw the pen point
        PenPoint penPoint;
        penPoint.Draw();


        /* ADDING AND MOVING NEW OBJECTS */
        Objects book1;
        book1.link(sizeof(vertices), vertices);
        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::scale(model, glm::vec3(xx + 1, yy + 1, zz + 1));
        lightingShader.setMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[r]);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* RENDER LIGHTS */
        // bind the light uniform buffer object and the lightcube vao
        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glBindVertexArray(lightCubeVAO);
        // set the speed of the light movement
        // iterate through two draw loops binding the light cube vao twice in two different positions
        for (unsigned int i = 0; i < 2; i++)
        {
            if (i == 0)
            { /* PINK LIGHT */
                // initialize the shader and use it to draw pink lights
                pinkShader.use();
                pinkShader.setMat4("projection", projection);
                pinkShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                /* ORBIT THE LIGHTS */
                model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * direction * 2.0f, glm::vec3(0.0f, 1.0f, 0.f));
                // place the lights into the lightpoistions vector positions
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(.25f));
                pinkShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else
            { /* PURPLE LIGHT */
                // initialize the shader and use it to draw purple lights
                purpleShader.use();
                purpleShader.setMat4("projection", projection);
                purpleShader.setMat4("view", view);
                model = glm::mat4(1.0f);
                model = glm::rotate(model, (GLfloat)glfwGetTime() * glm::radians(speed) * 2.0f, glm::vec3(0.0f, 1.0f, 0.f));
                // place the lights into the lightpoistions vector positions
                model = glm::translate(model, lightPositions[i]);
                model = glm::scale(model, glm::vec3(.25f));
                purpleShader.setMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        /* RENDER SKYBOX */
        glDepthFunc(GL_LEQUAL);
        Objects skybox;
        skybox.skybox(sizeof(skyboxVertices), skyboxVertices);
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        skybox.bindSkybox();
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Delete VAO and VBO
    glDeleteVertexArrays(1, &textVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &textVBO);
    glDeleteBuffers(1, &lightCubeVBO);
    glDeleteBuffers(1, &uboMatrices);

    /* CLEAR MEMORY OF OBJECTS */
    Objects skybox;
    skybox.clear();

    // these classes have deconstructors that will be called when they go out of scope
    PenPoint penPoint;
    PenAccent sphereAccent;
    PenClip penClip;
    PenBody penBody;

    Objects cube;
    cube.clear();
    Objects box;
    box.clear();
    Objects pyramid;
    pyramid.clear();

    // clear textures
    texture.destroyTexture(redTexture);
    texture.destroyTexture(blueTexture);
    texture.destroyTexture(skyboxTexture);
    texture.destroyTexture(lightgreyTexture);
    texture.destroyTexture(blackboardTexture);
    texture.destroyTexture(cubemapTexture);
    texture.destroyTexture(compassTexture);
    texture.destroyTexture(groundTexture);
    texture.destroyTexture(metalTexture);
    texture.destroyTexture(ballpointTexture);
    texture.destroyTexture(posterTexture);
    texture.destroyTexture(certificateTexture);

    glDeleteProgram(lightingShader.ID);
    glDeleteProgram(skyboxShader.ID);
    glDeleteProgram(purpleShader.ID);
    glDeleteProgram(pinkShader.ID);
    glDeleteProgram(textShader.ID);

    glfwTerminate();
    return 0;
}

/* PROCESS INPUT */
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if ((glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS))
        onPerspective = false;
    if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS))
        onPerspective = true;
    if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
        x -= .005;
    if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
        x += .005;
    if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
        y += .005;
    if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
        y -= .005;
    if ((glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS))
        z -= .005;
    if ((glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS))
        z += .005;
    if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS))
        xx -= .0005;
    if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS))
        xx += .0005;
    if ((glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS))
        yy += .0005;
    if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS))
        yy -= .0005;
    if ((glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS))
    {
        std::cout << "x value: " << x << ", y Value: " << y << " z Value: " << z << std::endl;
        std::cout << "xx value: " << xx << ", yy Value: " << yy << " zz Value: " << zz << std::endl;
        std::cout << endl;

    }
    if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS))
    {
        if (t < textures.size() - 1)
            t += 1;
        else
            t = 0;
    }
    if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS))
    {
        if (r < textures.size() - 1)
            r += 1;
        else
            r = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        renderText = !renderText; // Toggle the rendering of text
    }
}
// mouse button handling
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            if (action == GLFW_PRESS)
                cout << "Left mouse button pressed" << endl;
            else
                cout << "Left mouse button released" << endl;
        }
            break;

        case GLFW_MOUSE_BUTTON_MIDDLE:
        {
            if (action == GLFW_PRESS)
                cout << "Middle mouse button pressed" << endl;
            else
                cout << "Middle mouse button released" << endl;
        }
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
        {
            if (action == GLFW_PRESS)
                cout << "Right mouse button pressed" << endl;
            else
                cout << "Right mouse button released" << endl;
        }
            break;

        default:
            cout << "Unhandled mouse button event" << endl;
            break;
    }
}
/* CALLBACKS */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// function to increase or decrease camera speed depending on the scroll direction
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
    // increase camera speed
    if (yoffset > 0)
        camera.MovementSpeed += 1.0f;
        // decrease camera speed
    else
        camera.MovementSpeed -= 1.0f;
}
void RenderText(Shader& s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state
    s.use();
    glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



void InitText()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "OCRAEXT.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}
