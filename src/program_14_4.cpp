#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Utils.hpp>
#include <modelImporter.hpp>

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

glm::mat4 pMat, vMat, mMat, mvMat, invTrMat;

GLuint stripesTexture;
const int texHeight = 200;
const int texWidth = 200;
const int texDepth = 200;
double tex3Dpattern[texHeight][texWidth][texDepth];

int numVertices;


void fillDataArray(GLubyte data[]) {
	for (int i = 0; i<texHeight; i++) {
		for (int j = 0; j<texWidth; j++) {
			for (int k = 0; k<texDepth; k++) {
				if (tex3Dpattern[i][j][k] == 1.0) {
					// yellow color
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 0] = (GLubyte)255; //red
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 1] = (GLubyte)255; //green
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 2] = (GLubyte)0; //blue
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 3] = (GLubyte)0; //alpha
				}
				else {
					// blue color
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 0] = (GLubyte)0; //red
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 1] = (GLubyte)0; //green
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 2] = (GLubyte)255; //blue
					data[i*(texWidth*texHeight * 4) + j*(texHeight * 4) + k * 4 + 3] = (GLubyte)0; //alpha
				}
			}
		}
	}
}

int build3DTexture() {

	GLuint textureID;
	GLubyte* data = new GLubyte[texHeight*texWidth*texDepth * 4];

	fillDataArray(data);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, texWidth, texHeight, texDepth);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, texWidth, texHeight, texDepth, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);

	delete[] data;
	return textureID;
}

void generate3DPattern1() {
	int xStep, yStep, zStep, sumSteps;
	for (int x = 0; x < texWidth; x++) {
		for (int y = 0; y < texHeight; y++) {
			for (int z = 0; z < texDepth; z++) {
				xStep = (x / 10) % 2;
				yStep = (y / 10) % 2;
				zStep = (z / 10) % 2;
				sumSteps = xStep + yStep + zStep;
				if ((sumSteps % 2) == 0)
					tex3Dpattern[x][y][z] = 0.0;
				else
					tex3Dpattern[x][y][z] = 1.0;
			}
		}
	}
}

void generate3DPattern2(){
	for (int x = 0; x<texHeight; x++) {
		for (int y = 0; y<texWidth; y++) {
			for (int z = 0; z<texDepth; z++) {
				if ((y / 10) % 2 == 0)
					tex3Dpattern[x][y][z] = 0.0;
				else
					tex3Dpattern[x][y][z] = 1.0;
			}
		}
	}

}

void setupVertices() {

    ModelImporter skull;
    skull.parseObjFile("./models/dolphin.obj");
    numVertices = skull.getNumVertices();

    std::vector<float> skullVertices = skull.getVertices();
    std::vector<float> normals = skull.getNormals();
    std::vector<float> textures = skull.getTexCoords();

    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    // Configurar VBO para los vértices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, skullVertices.size()  * sizeof(float), &skullVertices[0], GL_STATIC_DRAW);

    // Configurar VBO para las coordenadas de las texturas
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(float), &textures[0], GL_STATIC_DRAW);

    // Configurar VBO para las coordenadas de las normales
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
    
}

void init(GLFWwindow *window) {
    
    renderingProgram = Utils::createShaderProgram("./shaders/vertex_shader144.glsl", "./shaders/fragment_shader144.glsl");

    if (Utils::checkOpenGLError()) {
        std::cerr << "ERROR: Could not create the shader program" << std::endl;
    }

    glfwGetFramebufferSize(window, &width, &height);
    aspect = static_cast<float>(width) / static_cast<float>(height);

    pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 60.0f);

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 3.0f;

    setupVertices();

    generate3DPattern1();

    stripesTexture =  build3DTexture();

}

void display(GLFWwindow *window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glUseProgram(renderingProgram);

    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
    mMat = glm::rotate(glm::mat4(1.0f), -(float)45.1f, glm::vec3(0.0f, 1.0f, 0.0f));    
    mvMat = vMat * mMat;

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // link the vertex attributes with the buffer data
    glEnableVertexAttribArray(0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, stripesTexture);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);;
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glDrawArrays(GL_TRIANGLES, 0, numVertices);
}

int main(void) {
    if (!glfwInit()) {
        std::cerr << "ERROR: GLFW could not initialize" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program_14_4 ", nullptr, nullptr);    
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
