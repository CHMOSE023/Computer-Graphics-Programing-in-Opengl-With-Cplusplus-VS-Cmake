// Include necessary libraries
#include <glad/glad.h>  // Library for handling the loading of OpenGL functions
#include <GLFW/glfw3.h> // Library for handling window and user input
#include <iostream>     // Library for input/output operations

// Initialization function (currently empty)
void init(GLFWwindow* window){
}

// Function to update the display
void display(GLFWwindow* window){
    glClearColor(1.0f,0.0f,0.0f,1.0f); // Set the background color
    glClear(GL_COLOR_BUFFER_BIT);       // Clear the color buffer with the background color
}

// Main function
int main() {
    
    // Initialize GLFW, terminate program if failed
    if (!glfwInit()) {exit(EXIT_FAILURE);}

    // Set window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  // Major OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // Minor OpenGL version
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // OpenGL profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Forward compatibility

    // Create the window
    GLFWwindow* window = glfwCreateWindow(1080, 720, "opengl_program_2_1", NULL, NULL);
    // Terminate program if window creation failed
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set the OpenGL context for the window
    glfwMakeContextCurrent(window);

    // Initialize GLAD to load OpenGL functions
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "failed to initialize GLAD " << std::endl;
        return -1;
    }
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        display(window);          // Update the display
        glfwPollEvents();         // Process input events
        glfwSwapBuffers(window);  // Swap the window buffers
    }

    // Terminate GLFW when program ends
    glfwTerminate();
    return 0;
}