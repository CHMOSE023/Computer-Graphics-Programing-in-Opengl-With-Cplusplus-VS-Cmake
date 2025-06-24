#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Utils.hpp>


GLuint buffer[3];
GLuint simpleComputeShader;

int v1[] = { 10, 12, 16, 18, 50, 17};
int v2[] = { 30, 14, 80, 20, 51, 12 };
int res[6];

void init() {
	simpleComputeShader = Utils::createShaderProgramCP("./shaders/compute_shader161.glsl");

	glGenBuffers(3, buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(v1), v1, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(v2), v2, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(res), res, GL_STATIC_READ);

	Utils::displayComputeShaderLimits();
}

void computeSum() {
	glUseProgram(simpleComputeShader);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buffer[0]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buffer[1]);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buffer[2]);

	glDispatchCompute(6,1,1);
	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer[2]);
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(res), res);
}

int main(void) {

	int wait;
    if (!glfwInit()) {
        std::cerr << "ERROR: GLFW could not initialize" << std::endl;
        exit(EXIT_FAILURE);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program_16_1 ", nullptr, nullptr);    
	
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

	init();
	computeSum();
	
	std::cout << v1[0] << " " << v1[1] << " " << v1[2] << " " << v1[3] << " " << v1[4] << " " << v1[5] << std::endl;
	std::cout << v2[0] << " " << v2[1] << " " << v2[2] << " " << v2[3] << " " << v2[4] << " " << v2[5] << std::endl;
	std::cout << res[0] << " " << res[1] << " " << res[2] << " " << res[3] << " " << res[4] << " " << res[5] << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate();
	
	std::cin >> wait;
	
	exit(EXIT_SUCCESS);
}
