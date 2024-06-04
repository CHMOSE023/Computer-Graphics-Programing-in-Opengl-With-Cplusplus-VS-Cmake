// Include necessary libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Define the number of Vertex Array Objects (VAOs) to be used
#define numVAOs 1
GLuint renderingProgram; // ID for the shader program
GLuint vao[numVAOs]; // Array to store the VAO IDs

// Function to create a shader program
GLuint glCreateShaderProgram()
{
    // Vertex shader source code
    const char *vshaderSource =
        "#version 430 \n"
        "void main(void) \n"
        "{ gl_Position = vec4(0.0, 0.0, 0.0, 1.0); }";

    // Fragment shader source code
    const char *fshaderSource =
        "#version 430 \n"
        "out vec4 color; \n"
        "void main(void) \n"
        "{ color = vec4(0.0, 0.0, 1.0, 1.0); }";

    // Create vertex and fragment shaders
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the shader source code to the shader objects
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);

    // Compile the shaders
    glCompileShader(vShader);
    glCompileShader(fShader);

    // Create a shader program and attach the shaders to it
    GLint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    // Return the ID of the shader program
    return vfProgram;
}

// Initialization function
void init(GLFWwindow *window)
{
    // Create the shader program
    renderingProgram = glCreateShaderProgram();
    // Generate the VAOs
    glGenVertexArrays(numVAOs, vao);
    // Bind the first VAO
    glBindVertexArray(vao[0]);
}

// Function to display the scene
void displaw(GLFWwindow* window, double currentTime){
    // Use the shader program
    glUseProgram(renderingProgram);
    // Set the point size
    glPointSize(30.0f);
    // Draw a single point
    glDrawArrays(GL_POINTS, 0, 1);
}

// Main function
int main(){

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set the OpenGL version to 4.3 and the profile to core
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(1080, 720, "opengl_program_2_2", NULL, NULL);
    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Load the OpenGL function pointers using GLAD
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "failed to initialize GLAD " << std::endl;
        return -1;
    }

    // Enable VSync
    glfwSwapInterval(1);
    // Initialize the scene
    init(window);
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Render the scene
        displaw(window, glfwGetTime());
        // Swap buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }
    // Clean up
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}