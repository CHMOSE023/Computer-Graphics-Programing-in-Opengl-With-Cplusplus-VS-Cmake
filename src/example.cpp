#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Utils.hpp>
#include <Sphere.hpp>

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLuint projLoc, mvLoc;
int width, height;
float aspect;

glm::mat4 pMat, vMat, mMat, mvMat;
Sphere mySphere(1.0f, 36, 18);  // Inicialización de la esfera

void setupVertices() {
    // Obtener los índices y vértices de la esfera
    std::vector<unsigned int> ind = mySphere.getIndices();
    std::vector<glm::vec3> vert = mySphere.getVertices();

    // Aplanar los vértices para el buffer
    std::vector<float> pvalues;
    for (const auto& v : vert) {
        pvalues.push_back(v.x);
        pvalues.push_back(v.y);
        pvalues.push_back(v.z);
    }

    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);

    glGenBuffers(numVBOs, vbo);

    // Configurar VBO para vértices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);

    // Configurar IBO para índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(unsigned int), &ind[0], GL_STATIC_DRAW);

    // Enlazar el atributo de los vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void init(GLFWwindow *window) {
    // Crea el programa de shaders usando los archivos de shaders especificados
    renderingProgram = Utils::createShaderProgram("./shaders/vertex_shader61.glsl", "./shaders/fragment_shader61.glsl");

    // Verifica si hubo errores al crear el programa de shaders
    if (Utils::checkOpenGLError()) {
        std::cerr << "ERROR: Could not create the shader program" << std::endl;
    }

    // Obtiene el tamaño del framebuffer para calcular la relación de aspecto
    glfwGetFramebufferSize(window, &width, &height);
    aspect = static_cast<float>(width) / static_cast<float>(height);

    // Configura la matriz de proyección con un campo de visión de 60 grados, relación de aspecto, y plano cercano y lejano
    pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);

    // Inicializa la posición de la cámara y el objeto
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 2.0f;
    pyrLocX = 0.0f; pyrLocY = 0.0f; pyrLocZ = 0.0f;

    // Configura los vértices y los índices para la esfera
    setupVertices();
}

void display(GLFWwindow *window, double currentTime) {
    // Limpia el buffer de profundidad y el buffer de color
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Configura el color de fondo
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Utiliza el programa de shaders
    glUseProgram(renderingProgram);

    // Obtiene las ubicaciones de las variables uniformes en el shader
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");

    // Configura la matriz de vista moviendo la cámara a la posición especificada
    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    // Configura la matriz de modelo moviendo el objeto a la posición especificada
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));

    // Calcula la matriz modelo-vista combinando las matrices de vista y modelo
    mvMat = vMat * mMat;

    // Pasa las matrices de proyección y modelo-vista al shader
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

    // Enlaza el VAO que contiene los vértices y los índices de la esfera
    glBindVertexArray(vao[0]);

    // Habilita el test de profundidad
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Dibuja los segmentos de línea de los triángulos
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, mySphere.getNumIndices(), GL_UNSIGNED_INT, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Desenlaza el VAO
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
