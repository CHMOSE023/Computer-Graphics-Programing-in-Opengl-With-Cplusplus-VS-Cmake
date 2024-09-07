#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Utils.hpp>
#include <Torus.hpp>

void installLights(glm::mat4 v_matrix);

#define numVAOs 1
#define numVBOs 3  //changed from 2 to 3

float cameraX, cameraY, cameraZ;

GLuint renderingProgram;
GLuint worldTexture;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint projLoc, mvLoc, nLoc;
int width, height;
float aspect;

//location for shader uniform variables
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc;
GLuint mAmbLoc, mDiffLoc, mSpecLoc, mShiLoc;

glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, lightPosV;
float lightPos[3];

//initial light location
glm::vec3 initialLightLoc(5.0f, 2.0f, 2.0f);

// white light properties

float globalAmbient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

float* matAmb = Utils::goldAmbient();
float* matDif = Utils::goldDiffuse();
float* matSpe = Utils::goldSpecular();
float matShi = Utils::goldShininess();

Torus myTorus(0.5f, 1.0f, 36, 72);  // Initialize a torus with inner radius 1.0, outer radius 0.5, 48 sides, and 48 rings

void setupVertices() {

    std::vector<unsigned int> ind = myTorus.getIndices();
    std::vector<glm::vec3> vert = myTorus.getVertices();
    std::vector<glm::vec2> texCoords = myTorus.getTexCoords();
    std::vector<glm::vec3> normals = myTorus.getNormals();

    // Crear un vector para almacenar los datos de vértices
    std::vector<float> pvalues;
    for (const auto& v : vert) {
        pvalues.push_back(v.x);
        pvalues.push_back(v.y);
        pvalues.push_back(v.z);
    }

    // Crear un vector para almacenar los datos de coordenadas de textura
    std::vector<float> tvalues;
    for (const auto& t : texCoords) {
        tvalues.push_back(t.s);
        tvalues.push_back(t.t);
    }
    
    // Crear un vector para almacenar los datos de normales
    std::vector<float> nvalues;
    for (const auto& n : normals) {
        nvalues.push_back(n.x);
        nvalues.push_back(n.y);
        nvalues.push_back(n.z);
    }

    int numVertices = myTorus.getNumVertices();
    int numIndices = myTorus.getNumIndices();

    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);

    // Configurar VBO para los vértices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

    // Configurar VBO para los índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);

    // Configurar VBO para las coordenadas de las normales
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);

}

void init(GLFWwindow *window) {
    
    renderingProgram = Utils::createShaderProgram("vertex_shader72.glsl", "fragment_shader72.glsl");

    if (Utils::checkOpenGLError()) {
        std::cerr << "ERROR: Could not create the shader program" << std::endl;
    }

    glfwGetFramebufferSize(window, &width, &height);
    aspect = static_cast<float>(width) / static_cast<float>(height);

    pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 6.0f;

    worldTexture = Utils::loadTexture("colorbrick.jpg");

    setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    glUseProgram(renderingProgram);

    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    nLoc = glGetUniformLocation(renderingProgram, "norm_matrix");

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    // Aplicar la rotación de 45 grados alrededor del eje Y
    float angle = glm::radians(-45.0f);
    //mMat = glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
    mMat = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));
    
    //currentLightPos = glm::vec3(initialLightLoc.x * cos(currentTime), initialLightLoc.y, initialLightLoc.z * sin(currentTime));
    currentLightPos = glm::vec3(initialLightLoc.x, initialLightLoc.y, initialLightLoc.z);
    installLights(vMat);
    
    mvMat = vMat * mMat;

    invTrMat = glm::transpose(glm::inverse(mvMat));


    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // link the vertex attributes with the buffer data
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0); // link the texture attributes with the buffer data
    glEnableVertexAttribArray(1);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, worldTexture);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawElements(GL_TRIANGLES, myTorus.getNumIndices(), GL_UNSIGNED_INT, 0);
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

    //GLFWwindow* window = glfwCreateWindow(1080, 720, "program_7_1", NULL, NULL);
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program_7_2 ", nullptr, nullptr);    
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


void installLights(glm::mat4 v_matrix) {
    lightPosV = glm::vec3(v_matrix * glm::vec4(currentLightPos, 1.0));
    lightPos[0] = lightPosV.x;
    lightPos[1] = lightPosV.y;
    lightPos[2] = lightPosV.z;

    // get the location of the globalAmbient in the shader
    globalAmbLoc = glGetUniformLocation(renderingProgram, "globalAmbient");
    // get the location of the ambient light in the shader
    ambLoc = glGetUniformLocation(renderingProgram, "light.ambient");
    // get the location of the diffuse light in the shader
    diffLoc = glGetUniformLocation(renderingProgram, "light.diffuse");
    // get the location of the specular light in the shader
    specLoc = glGetUniformLocation(renderingProgram, "light.specular");
    // get the location of the light position in the shader
    posLoc = glGetUniformLocation(renderingProgram, "light.position");

    // get the location of the ambient material in the shader
    mAmbLoc = glGetUniformLocation(renderingProgram, "material.ambient");
    // get the location of the diffuse material in the shader
    mDiffLoc = glGetUniformLocation(renderingProgram, "material.diffuse");
    // get the location of the specular material in the shader
    mSpecLoc = glGetUniformLocation(renderingProgram, "material.specular");
    // get the location of the shininess material in the shader
    mShiLoc = glGetUniformLocation(renderingProgram, "material.shininess");

    // set the uniform light and material values in the shader
    glProgramUniform4fv(renderingProgram, globalAmbLoc, 1, globalAmbient);
    glProgramUniform4fv(renderingProgram, ambLoc, 1, lightAmbient);
    glProgramUniform4fv(renderingProgram, diffLoc, 1, lightDiffuse);

    glProgramUniform4fv(renderingProgram, specLoc, 1, lightSpecular);
    glProgramUniform4fv(renderingProgram, posLoc, 1, lightPos);
    glProgramUniform4fv(renderingProgram, mAmbLoc, 1, matAmb);
    glProgramUniform4fv(renderingProgram, mDiffLoc, 1, matDif);
    glProgramUniform4fv(renderingProgram, mSpecLoc, 1, matSpe);
    glProgramUniform1f(renderingProgram, mShiLoc, matShi);

}

