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
float lightLocX, lightLocY, lightLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];

GLuint mvpLoc, mvLoc, projLoc, nLoc;
GLuint globalAmbLoc, ambLoc, diffLoc,
specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;


int width, height;
float aspect;
glm::mat4 projectMat, viewMat, modelMat, mvpMat, mvMat, invTrMat;

glm::vec3 currentLightPos, transformed;

float angle = 0.0f;

float lightPos[3];
float lightMovement = 0.1f;
double lastTime = 0.0;

float globalAmbient[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// silver material
float* matAmb = Utils::silverAmbient();
float* matDif = Utils::silverDiffuse();
float* matSpe = Utils::silverSpecular();
float matShi = Utils::silverShininess();

GLuint squareMoonTexture;
GLuint squareMoonHeight;
GLuint squareMoonNormals;

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
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program_12_4", nullptr, nullptr);    
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


void installLights(glm::mat4 vMatrix) {
	glm::vec3 transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	// get the locations of the light and material fields in the shader
	globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
	ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
	diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
	specLoc = glGetUniformLocation(renderingProgram, "light.specular");
	posLoc = glGetUniformLocation(renderingProgram, "light.position");
	mambLoc = glGetUniformLocation(renderingProgram, "material.ambient");
	mdiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
	mspecLoc = glGetUniformLocation(renderingProgram, "material.specular");
	mshiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

	//  set the uniform light and material values in the shader
	glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, posLoc, 1, lightPos);
	glProgramUniform4fv(renderingProgram, mambLoc, 1, matAmb);
	glProgramUniform4fv(renderingProgram, mdiffLoc, 1, matDif);
	glProgramUniform4fv(renderingProgram, mspecLoc, 1, matSpe);
	glProgramUniform1f(renderingProgram, mshiLoc, matShi);
}

void init(GLFWwindow* window) {
	renderingProgram = Utils::createShaderProgram("./shaders/vertex_shader124.glsl", "./shaders/tessC_shader124.glsl", "./shaders/tessE_shader124.glsl", "./shaders/fragment_shader124.glsl");
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 0.5f;
	terLocX = 0.0f; terLocY = 0.0f; terLocZ = 0.0f;
    lightLocX = 0.0f; lightLocY = 0.0f; lightLocZ = 2.0f;


	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	projectMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
	
    squareMoonTexture = Utils::loadTexture("./textures/moonMap.jpg");
    squareMoonHeight = Utils::loadTexture("./textures/moonBump.jpg");
    squareMoonNormals = Utils::loadTexture("./textures/moonNormal.jpg");

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(renderingProgram);

    lightLocX += lightMovement *( currentTime - lastTime);
    if (lightLocX > 0.5f) lightLocX = -0.1f;
    else if (lightLocX < -0.5f) lightMovement = 0.1f;
    lastTime = currentTime;

    mvpLoc = glGetUniformLocation(renderingProgram, "mvp_matrix");
	mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

    viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(terLocX, terLocY, terLocZ));
    modelMat = glm::rotate(modelMat, toRadians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));    
    modelMat = glm::rotate(modelMat, toRadians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));    
    
    // Asegúrate de que el ángulo no exceda 360 grados

    angle += 0.1f; // Ajusta la velocidad de rotación según sea necesario
    
    if (angle > 360.0f) {
        angle -= 360.0f;
    }

    modelMat = glm::rotate(modelMat, toRadians(angle), glm::vec3(0.0f, 0.0f, 1.0f));    
    

    mvMat = viewMat * modelMat;
    mvpMat = projectMat * viewMat * modelMat;
    
    invTrMat = glm::transpose(glm::inverse(mvMat));
    

    currentLightPos = glm::vec3(lightLocX, lightLocY, lightLocZ);
    installLights(viewMat);

    
    
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvpMat));
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, squareMoonTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, squareMoonHeight);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, squareMoonNormals);
    
    glFrontFace(GL_CCW);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  // FILL or LINE
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64*64);

}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	projectMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}