//////////////////////////////////////////////////////////////////////
//  Program 2.1                                                     //
//  This program is a basic OpenGl application using GLFW           //
//  for window management and GLAD for loading OpenGL functions.    //
//  The program creates a window and clears the window with a       //
//  red color.                                                      //
//                                                                  //      
//  you can find more information about how                         //
//  this program works in the wiki of this repository.              //
//                                                                  //      
//  Francisco Zavala                                                //
//  24/07/2024                                                      //
//////////////////////////////////////////////////////////////////////


// Include necessary libraries
#include <glad/glad.h>  // Library for handling the loading of OpenGL functions
#include <GLFW/glfw3.h> // Library for handling window and user input
#include <iostream>     

// Initialization function (currently empty)
void init(GLFWwindow* window){
}

// Function to update the display
void display(GLFWwindow* window){
    glClear(GL_COLOR_BUFFER_BIT);       
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

int main() {
    
    // Initialize GLFW, terminate program if failed
    if (!glfwInit()) {exit(EXIT_FAILURE);}

    // Set window properties
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                  
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            

    //Create the window
    //GLFWwindow* window = glfwCreateWindow(1080, 720, "program_2_1", NULL, NULL);
    
    //monitor properties
    
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, " program_2_1 ", nullptr, nullptr);    
    std::cout << "width " <<mode->width <<std::endl;
    std::cout << "height " <<mode->height <<std::endl;
    std::cout << "refresh " << mode->refreshRate << std::endl;
    std::cout << "blueBits " << mode->blueBits << std::endl;
    std::cout << "redBits " << mode->redBits<< std::endl;
    std::cout << "greenBits " << mode->greenBits << std::endl;
       
    
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


        display(window);          
        glfwPollEvents();        
        glfwSwapBuffers(window); 
    }

    // Terminate GLFW when program ends
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}