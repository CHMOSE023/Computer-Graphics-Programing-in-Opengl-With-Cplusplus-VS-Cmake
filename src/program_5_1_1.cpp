#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Utils.hpp>

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;
GLuint renderingProgram;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint projLoc, mvLoc, tfLoc;
GLuint brickTexture;

int width, height;
float aspect;

float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;

glm::mat4 pMat, vMat, mMat, mvMat;

void setupVertices(void) {
    float pyramidPositions[54] = {
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f
    };

    float pyTexCoords[36] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // Primer triángulo
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // Segundo triángulo
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // Tercer triángulo
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // Cuarto triángulo
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // Quinto triángulo
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f  // Sexto triángulo
    };
    
    /*
        float pyTexCoords[36] = {
            0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f,
            1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 1.0f
        };
    */
    glGenVertexArrays(numVAOs, vao);  
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);  
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyTexCoords), pyTexCoords, GL_STATIC_DRAW);
}

void init(GLFWwindow *window) {    
    renderingProgram = Utils::createShaderProgram("vertex_shader51.glsl", "fragment_shader51.glsl");
    
    if (Utils::checkOpenGLError()) {
        std::cout << "ERROR: Could not create the shader program" << std::endl;
    }

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 6.0f;
    pyrLocX = 0.0f; pyrLocY = 0.0f; pyrLocZ = 2.0f;
    
    brickTexture = Utils::loadTexture("foto.png");

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
    
    // Aplicar rotaciones acumuladas
    mMat = glm::rotate(mMat, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(mMat, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    mMat = glm::rotate(mMat, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));

    mvMat = vMat * mMat;
    
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    
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

/*
 * This function is a callback for GLFW that handles key press and key hold events.
 * The following keys are mapped to corresponding actions:
 * 
 * 'A': Decrease the rotation around the Y-axis by 5.0f.
 * 'D': Increase the rotation around the Y-axis by 5.0f.
 * 'W': Decrease the rotation around the X-axis by 5.0f.
 * 'S': Increase the rotation around the X-axis by 5.0f.
 * 'Q': Decrease the rotation around the Z-axis by 5.0f.
 * 'E': Increase the rotation around the Z-axis by 5.0f.
 */

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_A:
                rotY -= 5.0f;
                break;
            case GLFW_KEY_D:
                rotY += 5.0f;
                break;
            case GLFW_KEY_W:
                rotX -= 5.0f;
                break;
            case GLFW_KEY_S:
                rotX += 5.0f;
                break;
            case GLFW_KEY_Q:
                rotZ -= 5.0f;
                break;
            case GLFW_KEY_E:
                rotZ += 5.0f;
                break;
        }
    }
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

    init(window);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
