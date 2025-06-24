#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <Utils.hpp>
#include <sstream>

// Constantes
const int WIDTH = 512;
const int HEIGHT = 512;

// Recursos globales
GLuint vao, vbo, ebo;
GLuint tex;
GLuint quadShader, computeShader;

void init() {
    // Quad simple
    float quadVertices[] = {
        // pos       // uv
        -1, -1,  0, 0,
         1, -1,  1, 0,
         1,  1,  1, 1,
        -1,  1,  0, 1
    };
    
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Crear textura
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, WIDTH, HEIGHT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Cargar shaders
    quadShader = Utils::createShaderProgram("shaders/vertex_shader162.glsl", "shaders/fragment_shader162.glsl");
    computeShader = Utils::createShaderProgramCP("shaders/compute_shader162.glsl");
}

// ==== FUNCIÓN DISPLAY ====
void display() {
    // Ejecutar compute shader
    glUseProgram(computeShader);
    glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
    glUniform2i(glGetUniformLocation(computeShader, "image_size"), WIDTH, HEIGHT);
    glDispatchCompute(WIDTH, HEIGHT, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Dibujar quad con textura
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(quadShader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);    
    
    glBindVertexArray(vao);
    glUniform1i(glGetUniformLocation(quadShader, "tex"), 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ==== FUNCIÓN PRINCIPAL ====
int main() {
    if (!glfwInit()) {
        std::cerr << "Error inicializando GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Compute Shader Texture Color", nullptr, nullptr);
    if (!window) {
        std::cerr << "No se pudo crear la ventana GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Inicializar recursos
    init();

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpieza
    glDeleteProgram(computeShader);
    glDeleteProgram(quadShader);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &tex);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
