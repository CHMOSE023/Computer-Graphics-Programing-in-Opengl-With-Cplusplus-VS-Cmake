#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utils/Utils.hpp"
#include "../utils/Sphere.hpp"

using namespace std;

#define numVAOs 1
#define numVBOs 1

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint projLoc, mvLoc;

int width, height;
float aspect;


glm::mat4 pMat, vMat, mMat, mvMat;

Sphere mySphere(48);  // Inicialización de la esfera

void setupVertices() {
    std::vector<unsigned int> ind = mySphere.getIndices();
    std::vector<glm::vec3> vert = mySphere.getVertices();

    std::vector<float> pvalues;

    int numIndices = mySphere.getNumIndices();
    int numVertices = mySphere.getNumVertices();
                        // 
    for (int i = 0; i < numIndices; i++) {
        pvalues.push_back(vert[i].x);
        pvalues.push_back(vert[i].y);
        pvalues.push_back(vert[i].z);
    }

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(1, vbo);

    // Vértices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), pvalues.data(), GL_STATIC_DRAW);

//  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
 
}

void init(GLFWwindow *window) {
    renderingProgram = Utils::createShaderProgram("vertex_shader61.glsl", "fragment_shader61.glsl");

    if (Utils::checkOpenGLError()) {
        std::cerr << "ERROR: Could not create the shader program" << std::endl;
    }

    glfwGetFramebufferSize(window, &width, &height);
    aspect = static_cast<float>(width) / static_cast<float>(height);
    pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 6.0f;
    pyrLocX = 0.0f; pyrLocY = 0.0f; pyrLocZ = 2.0f;

    //brickTexture = Utils::loadTexture("foto.png");

    setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));

    mvMat = vMat * mMat;

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    
    glBindVertexArray(vao[0]);
  
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    glPointSize(5.0);
    glDrawArrays(GL_POINT, 0, mySphere.getNumVertices());
    
    glBindVertexArray(0);
}


int main(void) {
    if (!glfwInit()) {exit(EXIT_FAILURE);}
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            

    GLFWwindow* window = glfwCreateWindow(1080, 720, "exercise", nullptr, nullptr);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glfwSwapInterval(1);
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

