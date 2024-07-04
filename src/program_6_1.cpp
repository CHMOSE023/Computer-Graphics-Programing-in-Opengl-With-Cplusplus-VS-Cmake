#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../utils/Utils.hpp"
#include "../utils/Sphere.hpp"

using namespace std;

#define numVAOs 1
#define numVBOs 2  //changed from 2 to 3

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint projLoc, mvLoc;
int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat;
Sphere mySphere(1.0f, 72, 36);  // Initialize a sphere with radius 1.0, 72 sectors, and 36 stacks

void setupVertices() {
    // get the vertices and indices from sphere object
    std::vector<unsigned int> ind = mySphere.getIndices(); 
    std::vector<glm::vec3> vert = mySphere.getVertices();

    // create a vector to store the vertex data 
    std::vector<float> pvalues;
    for (const auto& v : vert) {
        pvalues.push_back(v.x);
        pvalues.push_back(v.y);
        pvalues.push_back(v.z);
    }

    int numIndices = mySphere.getNumIndices();
    int numVertices = mySphere.getNumVertices();

    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);

    // configure VBO for vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

    // Configure VBO for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);

    // link the vertex attributes with the buffer data
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

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.0f;
    pyrLocX = 0.0f; pyrLocY = 0.0f; pyrLocZ = 0.0f;

    setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glClearColor(0.0, 0.0, 0.0, 1.0);

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

    glDrawElements(GL_TRIANGLES, mySphere.getNumIndices(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

int main(void) {
    if (!glfwInit()) {
        std::cerr << "ERROR: GLFW could not initialize" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1080, 720, "Exercise", NULL, NULL);
    if (!window) {
        std::cerr << "ERROR: GLFW window could not be created" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR: GLAD could not initialize" << std::endl;
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
