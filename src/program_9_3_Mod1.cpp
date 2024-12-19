#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Torus.hpp>
#include <Utils.hpp>
#include <modelImporter.hpp>
#include <iostream>

#define numVAOs 2
#define numVBOs 5

glm::vec3 cameraLoc = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 origin = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

float torLocX, torLocY, torLocZ;
float cameraX, cameraY, cameraZ;
GLuint renderingProgram;
GLuint renderingProgramCubeMap;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint brickTexture, skyboxTexture;
float rotAmt = 0.0f;

// variable allocation for display
GLuint mvLoc, projLoc, nLoc, eyeLoc, lightLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

glm::mat4 invTrMat;

Torus myTorus(0.6f, 1.0f, 36, 72);
int numTorusVertices, numTorusIndices;

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }


float deltaTime = 0.1f; // Tiempo entre el frame actual y el anterior
float lastFrame = 0.1f; // Tiempo del último frame
float cameraSpeed = 2.5f; // Velocidad de movimiento de la cámara

float Zoom = 45.0f;
float yaw = -90.0f; // Iniciar con -90 grados para mirar hacia adelante
float pitch = 0.0f;   // El ángulo en el eje Y (pitch)
float lastX, lastY; // Iniciar el mouse en el centro de la pantalla
bool firstMouse = true;
float sensitivity = 0.1; // Sensibilidad del mouse

void init(GLFWwindow* window);
void setupVertices(void);
void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);
void window_size_callback(GLFWwindow* win, int newWidth, int newHeight);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

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
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program_9_3_Mod1 ", nullptr, nullptr);    
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

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback); 
    glfwSetScrollCallback(window, scroll_callback);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
   
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
		return;
    }

    // Calcular el desplazamiento del ratón desde la última posición registrada
    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos; // Invertir el valor Y ya que los ejes Y de la pantalla y OpenGL están invertidos
    lastX = xpos;
    lastY = ypos;

    // Aplicar la sensibilidad al desplazamiento del ratón
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // Ajustar yaw (eje horizontal) y pitch (eje vertical)
    yaw += xOffset;
    pitch += yOffset;

    // Limitar el ángulo del pitch para que no voltee más allá de los 90 grados
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Calcular la nueva dirección del frente de la cámara
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    //cameraFront = glm::normalize(front); // Normalizar el vector para asegurar que tenga una magnitud de 1
    origin = glm::normalize(front); // Normalizar el vector para asegurar que tenga una magnitud de 1

}

void processInput(GLFWwindow *window) {
    
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Mover hacia adelante (W)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraLoc += cameraSpeed * deltaTime * origin; // Adelante

    // Mover hacia atrás (S)
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraLoc -= cameraSpeed * deltaTime * origin; // Atrás

    // Mover hacia la izquierda (A)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraLoc -= glm::normalize(glm::cross(origin, up)) * cameraSpeed * deltaTime; // Izquierda

    // Mover hacia la derecha (D)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraLoc += glm::normalize(glm::cross(origin,up)) * cameraSpeed * deltaTime;
}

void setupVertices(void) {
	float cubeVertexPositions[108] =
	{	-1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};

    float vertexPositions[] = {
        // Cara trasera (normales hacia -Z)
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        // Cara delantera (normales hacia +Z)
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

        // Cara izquierda (normales hacia -X)
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

        // Cara derecha (normales hacia +X)
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

        // Cara inferior (normales hacia -Y)
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        // Cara superior (normales hacia +Y)
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f
    };

	glGenVertexArrays(2, vao);
    
    glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

    glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void init(GLFWwindow* window) {
	//renderingProgram = Utils::createShaderProgram("./shaders/vertex_shader93Mod.glsl", "./shaders/fragment_shader93Mod.glsl"); //reflection
	renderingProgram = Utils::createShaderProgram("./shaders/vertex_shader93Mod.glsl", "./shaders/fragment_shader93Mod1.glsl"); //refraction
    renderingProgramCubeMap = Utils::createShaderProgram("./shaders/vertex_shader92.glsl", "./shaders/fragment_shader92.glsl");
		
    glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

	setupVertices();

	skyboxTexture = Utils::loadCubeMap("./textures/cubeMap2");
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	torLocX = 0.0f; torLocY = -0.75f; torLocZ = 0.0f;
    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 5.0f;
    cameraLoc.x = 0.0f; cameraLoc.y = 0.0f; cameraLoc.z = 5.0f;
}

void display(GLFWwindow* window, double currentTime) {
	float currentFrame = static_cast<float>(currentTime);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // draw cube map
    glUseProgram(renderingProgramCubeMap);

    vMat = glm::lookAt(cameraLoc, cameraLoc + origin , up);    
    pMat = glm::perspective(glm::radians(Zoom), aspect, 0.1f, 1000.0f);    
    
    glUniformMatrix4fv(glGetUniformLocation(renderingProgramCubeMap, "mv_matrix"), 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgramCubeMap, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));

    glBindVertexArray(vao[0]);	
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
   
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	// cube is CW, but we are viewing the inside
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

	// draw cube 
    glEnable(GL_DEPTH_TEST);
 	glUseProgram(renderingProgram);

    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(torLocX, torLocY, torLocZ));
    vMat = glm::lookAt(cameraLoc, cameraLoc + origin , up);    
    pMat = glm::perspective(glm::radians(Zoom), aspect, 0.1f, 100.0f);    
    
    mvMat = vMat * mMat;    
    invTrMat = glm::transpose(glm::inverse(mMat));

	glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "model_matrix"), 1, GL_FALSE, glm::value_ptr(mMat));
	glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "view_matrix"), 1, GL_FALSE, glm::value_ptr(vMat));
	glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "proj_matrix"), 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(glGetUniformLocation(renderingProgram, "norm_matrix"), 1, GL_FALSE, glm::value_ptr(invTrMat));
    glUniform3f(glGetUniformLocation(renderingProgram, "eyePos"), cameraLoc.x, cameraLoc.y, cameraLoc.z);

    glBindVertexArray(vao[1]);
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);


}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}



