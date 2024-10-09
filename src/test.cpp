#define GLFW_INCLUDE_NONE
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <MeshFactory.hpp>
#include <MeshTorus.hpp>
#include <Utils.hpp>
#include <Torus.hpp>
#include <MeshPyramid.hpp>
#include <modelImporter.hpp>

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

float aspectRatio;

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;

float deltaTime = 1.0f;	// Time between current frame and last frame
float lastFrame = 1.0f; // Time of last frame


//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void setupMesh();
void calculateDeltaTime();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);


MeshFactory factory;
GLuint renderingProgram;


void init(){

    renderingProgram = Utils::createShaderProgram(
            "./shaders/vertex_shader1.glsl",
            "./shaders/fragment_shader1.glsl");

    cameraPos   = glm::vec3(0.0f, 0.0f,  -3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
 
    setupMesh();
}

// Function to updae the display
void display(GLFWwindow* window) {

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Establecer el color de limpieza primero
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Incluir el buffer de profundidad

    
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::mat4(1.0f);


    model = glm::translate(model, glm::vec3(1.0f, 0.0f, -8.0f));

//    float angle = glm::radians(20.0f * (float)glfwGetTime()); // Cambiar con el tiempo

//    model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));

    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));

        
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    
    auto mesh = factory.createMesh("torus");
    mesh->setupMesh();

    glUseProgram(renderingProgram);

    unsigned int modelLoc = glGetUniformLocation(renderingProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(renderingProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(renderingProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    mesh->render();
    model = glm::mat4(1.0f);
    

    model = glm::translate(model, glm::vec3(5.0f, 0.0f, -8.0f));

//    float angle = glm::radians(20.0f * (float)glfwGetTime()); // Cambiar con el tiempo
//    model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));

    model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
    
    auto mesh1 = factory.createMesh("pyramid");
    mesh1->setupMesh();

    //glUseProgram(renderingProgram);
    //unsigned int modelLoc = glGetUniformLocation(renderingProgram, "model");
    //unsigned int viewLoc = glGetUniformLocation(renderingProgram, "view");
    //unsigned int projectionLoc = glGetUniformLocation(renderingProgram, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
 //   glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    mesh1->render();

}


int main() {
    
    if (!glfwInit()) {exit(EXIT_FAILURE);}

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " Mesh ", nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwMaximizeWindow(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "failed to initialize GLAD " << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    init();

    while (!glfwWindowShouldClose(window)) {
        calculateDeltaTime();
        display(window);
        processInput(window);          
        glfwPollEvents();
        glfwSwapBuffers(window); 
    }

    glfwTerminate();
   
    return 0;
}

void setupMesh(){

    Torus myTorus(0.6f, 1.0f, 36, 72);
	
    int numTorusVertices = myTorus.getNumVertices();
	int numTorusIndices = myTorus.getNumIndices();
	std::vector<unsigned int> ind = myTorus.getIndices();

    std::vector<glm::vec3> vert = myTorus.getVertices();
	std::vector<glm::vec3> norm = myTorus.getNormals();
    
    std::vector<float> vertexPositions{};

  	for (int i = 0; i < numTorusVertices; i++) {
		vertexPositions.push_back(vert[i].x);
		vertexPositions.push_back(vert[i].y);
		vertexPositions.push_back(vert[i].z);
		vertexPositions.push_back(norm[i].x);
		vertexPositions.push_back(norm[i].y);
		vertexPositions.push_back(norm[i].z);
	}

    factory.registerMeshType("torus", [vertexPositions, ind](){ 
        return std::make_unique<MeshTorus>(vertexPositions, ind);
    });

    ModelImporter pyramid;
    pyramid.parseObjFile("./models/pyr.obj");

	int numPyramidVertices = pyramid.getNumVertices();
	std::vector<float> vertfloat = pyramid.getVertices();
	std::vector<float> normfloat = pyramid.getNormals();

	vert.clear();
	norm.clear();
    vertexPositions.clear();

    for(int i = 0; i < numPyramidVertices; i++) {
        vert.push_back(glm::vec3(vertfloat[i * 3], vertfloat[i * 3 + 1], vertfloat[i * 3 + 2]));
        norm.push_back(glm::vec3(normfloat[i * 3], normfloat[i * 3 + 1], normfloat[i * 3 + 2]));
    }

	for (int i = 0; i < numPyramidVertices; i++) {
		vertexPositions.push_back((vert[i]).x);
		vertexPositions.push_back((vert[i]).y);
	    vertexPositions.push_back((vert[i]).z);
	    vertexPositions.push_back((norm[i]).x);
	    vertexPositions.push_back((norm[i]).y);
	    vertexPositions.push_back((norm[i]).z);
	}

    factory.registerMeshType("pyramid", [vertexPositions](){ 
        return std::make_unique<MeshPyramid>(vertexPositions);
    });



}

void calculateDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void processInput(GLFWwindow *window)
{
    const float cameraSpeed = 5.05f * deltaTime; // adjust accordingly
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    //std::cout << " hello " << std::endl;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    aspectRatio = (float)width / (float)height;
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
}
