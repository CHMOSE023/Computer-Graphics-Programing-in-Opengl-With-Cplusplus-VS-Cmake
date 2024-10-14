#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Torus.hpp>
#include <Utils.hpp>
#include <modelImporter.hpp>
#include <MeshPyramid.hpp>
#include <MeshTorus.hpp>
#include <Torus.hpp>
#include <MeshFactory.hpp>
#include <Sphere.hpp>
#include <MeshSphere.hpp>

using namespace std;

void passOne(void);
void passTwo(void);

float toRadians(float degrees) { return (degrees * 2.0f * 3.14159f) / 360.0f; }

#define numVAOs 1
#define numVBOs 5

MeshFactory factory;

GLuint renderingProgram1, renderingProgram2 , renderingProgram3;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

ModelImporter pyramid;
Torus myTorus(0.6f, 1.0f, 36, 72);
int numPyramidVertices, numTorusVertices, numTorusIndices;

glm::vec3 torusLoc(2.2f, -0.6f, -0.8f);
glm::vec3 pyrLoc(-1.0f, 0.1f, 0.3f);
glm::vec3 cameraLoc(-2.0f, 1.2f, 8.0f);
glm::vec3 lightLoc(-3.8f, 2.2f, 1.1f);


// white light
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

// gold material
float* gMatAmb = Utils::goldAmbient();
float* gMatDif = Utils::goldDiffuse();
float* gMatSpe = Utils::goldSpecular();
float gMatShi = Utils::goldShininess();

// bronze material
float* bMatAmb = Utils::bronzeAmbient();
float* bMatDif = Utils::bronzeDiffuse();
float* bMatSpe = Utils::bronzeSpecular();
float bMatShi = Utils::bronzeShininess();

float thisAmb[4], thisDif[4], thisSpe[4], matAmb[4], matDif[4], matSpe[4];
float thisShi, matShi;

// shadow stuff
int scSizeX, scSizeY;
GLuint shadowTex, shadowBuffer;
glm::mat4 lightVmatrix;
glm::mat4 lightPmatrix;
glm::mat4 shadowMVP1;
glm::mat4 shadowMVP2;
glm::mat4 b;

// variable allocation for display
GLuint mvLoc, projLoc, nLoc, sLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;
glm::vec3 currentLightPos, transformed;
float lightPos[3];
GLuint globalAmbLoc, ambLoc, diffLoc, specLoc, posLoc, mambLoc, mdiffLoc, mspecLoc, mshiLoc;
glm::vec3 origin(0.0f, 0.0f, 0.0f);
glm::vec3 origin2(0.0f, 0.0f, -4.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);


float deltaTime = 0.1f; // Tiempo entre el frame actual y el anterior
float lastFrame = 0.1f; // Tiempo del último frame
float cameraSpeed = 2.5f; // Velocidad de movimiento de la cámara

float Zoom = 45.0f;
float yaw = -90.0f; // Iniciar con -90 grados para mirar hacia adelante
float pitch = 0.0f;   // El ángulo en el eje Y (pitch)
float lastX, lastY; // Iniciar el mouse en el centro de la pantalla
bool firstMouse = true;
float sensitivity = 0.1; // Sensibilidad del mouse


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void installLights(int renderingProgram, glm::mat4 vMatrix);
void setupVertices(void);
void setupShadowBuffers(GLFWwindow* window);
void init(GLFWwindow* window);
void processInput(GLFWwindow *window);
void display(GLFWwindow* window, double currentTime);
void passOne(void); 
void passTwo(void);
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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "program_8_1", NULL, NULL);
 	//GLFWwindow* window = glfwCreateWindow(800, 800, "Chapter8 - program1", NULL, NULL);

	//std::cout << " width: " << mode->width << " height: " << mode->height << std::endl;
   
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

	glViewport(0, 0, mode->width, mode->height);

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
    origin2 = glm::normalize(front); // Normalizar el vector para asegurar que tenga una magnitud de 1

}

void installLights(int renderingProgram, glm::mat4 vMatrix) {
	transformed = glm::vec3(vMatrix * glm::vec4(currentLightPos, 1.0));
	lightPos[0] = transformed.x;
	lightPos[1] = transformed.y;
	lightPos[2] = transformed.z;

	matAmb[0] = thisAmb[0]; matAmb[1] = thisAmb[1]; matAmb[2] = thisAmb[2]; matAmb[3] = thisAmb[3];
	matDif[0] = thisDif[0]; matDif[1] = thisDif[1]; matDif[2] = thisDif[2]; matDif[3] = thisDif[3];
	matSpe[0] = thisSpe[0]; matSpe[1] = thisSpe[1]; matSpe[2] = thisSpe[2]; matSpe[3] = thisSpe[3];
	matShi = thisShi;

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

void setupVertices(void) {

	// pyramid definition
    pyramid.parseObjFile("./models/pyr.obj");

	numPyramidVertices = pyramid.getNumVertices();
	std::vector<float> vertfloat = pyramid.getVertices();
	std::vector<float> normfloat = pyramid.getNormals();

	std::vector<glm::vec3> vert;
	std::vector<glm::vec3> norm;
    std::vector<float> vertexPositions{};

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
	
	vert.clear();
	norm.clear();
    vertexPositions.clear();
	
	// torus definition
	numTorusVertices = myTorus.getNumVertices();
	numTorusIndices = myTorus.getNumIndices();
	std::vector<unsigned int> ind = myTorus.getIndices();
	
	vert = myTorus.getVertices();
	norm = myTorus.getNormals();

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

    vertexPositions.clear();
	ind.clear();
	Sphere mySphere(0.1f, 36, 18);
    ind = mySphere.getIndices();
    std::vector<glm::vec3> vertices = mySphere.getVertices();
    std::vector<glm::vec3> normals = mySphere.getNormals();


    for (int i = 0; i < vertices.size(); i++)
    {
        vertexPositions.push_back(vertices[i].x);
        vertexPositions.push_back(vertices[i].y);
        vertexPositions.push_back(vertices[i].z);
        vertexPositions.push_back(normals[i].x);
        vertexPositions.push_back(normals[i].y);
        vertexPositions.push_back(normals[i].z);
    }

    factory.registerMeshType("sphere", [vertexPositions, ind](){ 
        return std::make_unique<MeshSphere>(vertexPositions, ind);
    });

}

void setupShadowBuffers(GLFWwindow* window) {
	glfwGetFramebufferSize(window, &width, &height);
	scSizeX = width;
	scSizeY = height;

	glGenFramebuffers(1, &shadowBuffer);

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32,
		scSizeX, scSizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	// may reduce shadow border artifacts
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void init(GLFWwindow* window) {
	renderingProgram1 = Utils::createShaderProgram("./shaders/vert1Shader.glsl", "./shaders/frag1Shader.glsl");
	renderingProgram2 = Utils::createShaderProgram("./shaders/vert2Shader.glsl", "./shaders/frag2Shader.glsl");
	renderingProgram3 = Utils::createShaderProgram("./shaders/vertex_shader1.glsl", "./shaders/fragment_shader1.glsl");

	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 100.0f);

	setupVertices();
	setupShadowBuffers(window);

	b = glm::mat4(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void processInput(GLFWwindow *window) {
    
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Mover hacia adelante (W)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraLoc += cameraSpeed * deltaTime * origin2; // Adelante

    // Mover hacia atrás (S)
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraLoc -= cameraSpeed * deltaTime * origin2; // Atrás

    // Mover hacia la izquierda (A)
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraLoc -= glm::normalize(glm::cross(origin2, up)) * cameraSpeed * deltaTime; // Izquierda

    // Mover hacia la derecha (D)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraLoc += glm::normalize(glm::cross(origin2,up)) * cameraSpeed * deltaTime;

  
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		lightLoc.z += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		lightLoc.z -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		lightLoc.x += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		lightLoc.x -= 0.05f;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		lightLoc.y += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightLoc.y -= 0.05f;
	
}


void display(GLFWwindow* window, double currentTime) {

    float currentFrame = static_cast<float>(currentTime);
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	
	currentLightPos = glm::vec3(lightLoc);

	lightVmatrix = glm::lookAt(currentLightPos, origin, up);
	lightPmatrix = glm::perspective(glm::radians(90.0f), (float)scSizeX / (float)scSizeY, 0.1f, 1000.0f);
	

	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glViewport(0, 0, width, height);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowTex, 0);

	glDrawBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);	// for reducing
	glPolygonOffset(3.0f, 5.0f);		//  shadow artifacts

	passOne();

	glDisable(GL_POLYGON_OFFSET_FILL);	// artifact reduction, continued

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	glDrawBuffer(GL_FRONT);

	passTwo();

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void passOne(void) {
	auto mesTorus = factory.createMesh("torus");
	mesTorus->setupMesh();

	glUseProgram(renderingProgram1);

	// draw the torus
	mMat = glm::translate(glm::mat4(1.0f), torusLoc);
	mMat = glm::rotate(mMat, toRadians(105.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	sLoc = glGetUniformLocation(renderingProgram1, "shadowMVP");
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	mesTorus->render();
	
	// draw the pyramid
	auto mesPyramid = factory.createMesh("pyramid");
	mesPyramid->setupMesh();

	mMat = glm::translate(glm::mat4(1.0f), pyrLoc);
	mMat = glm::rotate(mMat, toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	shadowMVP1 = lightPmatrix * lightVmatrix * mMat;
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP1));


	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	mesPyramid->render();


}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void passTwo(void) {
	auto mesTorus = factory.createMesh("torus");
	auto mesPyramid = factory.createMesh("pyramid");


	glUseProgram(renderingProgram2);

	mvLoc = glGetUniformLocation(renderingProgram2, "mv_matrix");
	projLoc = glGetUniformLocation(renderingProgram2, "proj_matrix");
	nLoc = glGetUniformLocation(renderingProgram2, "norm_matrix");
	sLoc = glGetUniformLocation(renderingProgram2, "shadowMVP");

	// draw the torus

	thisAmb[0] = bMatAmb[0]; thisAmb[1] = bMatAmb[1]; thisAmb[2] = bMatAmb[2];  // bronze
	thisDif[0] = bMatDif[0]; thisDif[1] = bMatDif[1]; thisDif[2] = bMatDif[2];
	thisSpe[0] = bMatSpe[0]; thisSpe[1] = bMatSpe[1]; thisSpe[2] = bMatSpe[2];
	thisShi = bMatShi;

	vMat = glm::lookAt(cameraLoc, cameraLoc + origin2 , up);
	pMat = glm::perspective(glm::radians(Zoom), aspect, 0.1f, 100.0f);

	mMat = glm::translate(glm::mat4(1.0f), torusLoc);
	mMat = glm::rotate(mMat, toRadians(105.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram2, vMat);

	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

	mesTorus->setupMesh();

	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	mesTorus->render();

	// draw the pyramid

	thisAmb[0] = gMatAmb[0]; thisAmb[1] = gMatAmb[1]; thisAmb[2] = gMatAmb[2];  // gold
	thisDif[0] = gMatDif[0]; thisDif[1] = gMatDif[1]; thisDif[2] = gMatDif[2];
	thisSpe[0] = gMatSpe[0]; thisSpe[1] = gMatSpe[1]; thisSpe[2] = gMatSpe[2];
	thisShi = gMatShi;

	mMat = glm::translate(glm::mat4(1.0f), pyrLoc);
	mMat = glm::rotate(mMat, toRadians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	mMat = glm::rotate(mMat, toRadians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));


	currentLightPos = glm::vec3(lightLoc);
	installLights(renderingProgram2, vMat);

	mvMat = vMat * mMat;
	invTrMat = glm::transpose(glm::inverse(mvMat));
	shadowMVP2 = b * lightPmatrix * lightVmatrix * mMat;

	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(invTrMat));
	glUniformMatrix4fv(sLoc, 1, GL_FALSE, glm::value_ptr(shadowMVP2));

	mesPyramid->setupMesh();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	mesPyramid->render();

	// draw the light source as a sphere
	auto meshSphere = factory.createMesh("sphere");
	meshSphere->setupMesh();

	mMat = glm::translate(glm::mat4(1.0f), currentLightPos);

	glUseProgram(renderingProgram3);

    unsigned int modelLoc = glGetUniformLocation(renderingProgram3, "model");
    unsigned int viewLoc = glGetUniformLocation(renderingProgram3, "view");
    unsigned int projectionLoc = glGetUniformLocation(renderingProgram3, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mMat));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(vMat));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(pMat));
	
	meshSphere->render();
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
	aspect = (float)newWidth / (float)newHeight;
	glViewport(0, 0, newWidth, newHeight);
	pMat = glm::perspective(1.0472f, aspect, 0.1f, 100.0f);

}


