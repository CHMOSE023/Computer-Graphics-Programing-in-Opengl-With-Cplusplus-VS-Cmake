#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include "../utils/Utils.hpp"
using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint projLoc, mvLock,tfLoc;
GLuint brickTexture;

int width, height;
float aspect,timeFactor;

glm::mat4 pMat, vMat, tMat, rMat, mMat, mvMat;

void setupVertices(void) {
  

   // 6 triangles * 3 vertices * 3 values
    float pyramidPositions[54] = {
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f
    };
    
    float pyTexCoords[36]{0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
                          1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
                          1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
                          1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
                          1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
                          1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f};

    glGenVertexArrays(numVAOs, vao);  // creates VAO and returns the integer ID
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);  // creates VBO and returns the integer ID
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyTexCoords), pyTexCoords, GL_STATIC_DRAW);
}

void init (GLFWwindow *window){    
    renderingProgram = Utils::createShaderProgram("vertex_shader51.glsl", "fragment_shader51.glsl");
    
    if (Utils::checkOpenGLError()){
        std::cout << "ERROR: Could not create the shader program" << std::endl;
    }

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective<float>(glm::radians(60.0f), aspect, 0.1f, 24.0f);
    
    // position the camera further down the positive Z axis (to see all of the cubes)
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 5.0f;
    pyrLocX = 0.0f; pyrLocY = 0.0f; pyrLocZ = 2.0f;
    
    brickTexture = Utils::loadTexture("sand.png");

    setupVertices();
}

void display (GLFWwindow *window, double currentTime){
     
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);   
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mvLock = glGetUniformLocation(renderingProgram, "mv_matrix");
    
    // the view matrix is computed once and used for both objects
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ)) ;
    mvMat = vMat * mMat;
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(mvLock, 1, GL_FALSE, glm::value_ptr(mvMat));
    
 
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTexture);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, 18);

}

int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            

    GLFWwindow* window = glfwCreateWindow(1080, 720, "opengl_program_5_1", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
   
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}