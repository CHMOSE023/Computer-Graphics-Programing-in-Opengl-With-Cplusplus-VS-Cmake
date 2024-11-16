#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Torus.hpp>
#include <Sphere.hpp>
#include <Utils.hpp>
#include <modelImporter.hpp>
#include <iostream>

#define numVAOs 1


float cameraX, cameraY, cameraZ;
float terLocX, terLocY, terLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint mvpLoc;
GLuint floorTex;
int width, height;
float aspect;
glm::mat4 projectMat, viewMat, modelMat, mvpMat, mvMat, normMat;


float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }
void init(GLFWwindow* window);
void installLights(glm::mat4 vMatrix);
void setupVertices(void);
void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);
void window_size_callback(GLFWwindow* win, int newWidth, int newHeight);

int main(void) {
    if (!glfwInit()) {
        std::cerr << "ERROR: GLFW could not initialize" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //GLFWwindow* window = glfwCreateWindow(1080, 720, "program_7_3", NULL, NULL);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program_12_2 ", nullptr, nullptr);    
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

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("./shaders/vertex_shader122.glsl", "./shaders/tessC_shader122.glsl", "./shaders/tessE_shader122.glsl", "./shaders/fragment_shader122.glsl");
	cameraX = 0.2f; cameraY = 0.2f; cameraZ = 3.5f;
	terLocX = 0.0f; terLocY = 0.0f; terLocZ = 0.0f;

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	projectMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	
    floorTex = Utils::loadTexture("./textures/floor.jpg");

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

    viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(terLocX, terLocY, terLocZ));
    modelMat = glm::rotate(modelMat, toRadians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));    
    mvpMat = projectMat * viewMat * modelMat;
    
    mvpLoc = glGetUniformLocation(renderingProgram, "mvp_matrix");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTex);
    
    glFrontFace(GL_CCW);

	glPatchParameteri(GL_PATCH_VERTICES, 16);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // FILL or LINE
	glDrawArrays(GL_PATCHES, 0, 16);

}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	projectMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}