cmake_minimum_required(VERSION 3.25)

include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)

project(graphics VERSION 2.0.1 LANGUAGES CXX)


set(LIBRARIES_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/soild2
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

foreach(dir ${LIBRARIES_DIR})
    add_subdirectory(${dir})
endforeach()

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

set(SOURCE_MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCE_TEXTURE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/textures)
set(SOURCE_MODELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/models)
set(SOURCE_SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)

file(GLOB SOURCE_FILES ${SOURCE_MAIN_DIR}/*.cpp)

option(OPTION "compile all programans" ON)

#To compile only one program, specify the name of the program and its dependency files.
if(NOT OPTION)

    set(UTILS_MAIN_DIR utils)
    # here you can add the name of the program you want to compile
    set(EXECUTABLE_NAME program_2_1)    
    add_executable(${EXECUTABLE_NAME}
        ${SOURCE_MAIN_DIR}/${EXECUTABLE_NAME}.cpp
    )

    #target_link_libraries(${EXECUTABLE_NAME} glad utilities glfw  glm::glm soil3 ${OPENGL_LIBRARIES})
    target_link_libraries(${EXECUTABLE_NAME} glad glfw ${OPENGL_LIBRARIES})


    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/programs
    )

    #here you can add the name of the files you want to copy to the same directory as the executable
    #this is necessary for the program to run correctly
    #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/vertex_shader73.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
    #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/fragment_shader73.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
    #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/Skull.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
    #file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/texture/Skull.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)


else()
    
    set(RUNTIME_OUTPUT_DIRECTORY_PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/programs)
    set(RUNTIME_OUTPUT_DIRECTORY_SHADERS ${CMAKE_CURRENT_BINARY_DIR}/programs/shaders)
    set(RUNTIME_OUTPUT_DIRECTORY_TEXTURES ${CMAKE_CURRENT_BINARY_DIR}/programs/textures)
    set(RUNTIME_OUTPUT_DIRECTORY_MODELS ${CMAKE_CURRENT_BINARY_DIR}/programs/models)

    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_PROGRAMS})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_SHADERS})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_TEXTURES})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_MODELS})


    foreach(SOURCE_FILE ${SOURCE_FILES})
        get_filename_component(SOURCE_FILE_NAME ${SOURCE_FILE} NAME_WE)
        
        message(STATUS "Adding program: ${SOURCE_FILE_NAME}")
        
        add_executable(${SOURCE_FILE_NAME}
            ${SOURCE_FILE}
        )

        target_link_libraries(${SOURCE_FILE_NAME} PRIVATE glad glfw glm::glm soil3 ${OPENGL_LIBRARIES} utilities)
        
        set_target_properties(${SOURCE_FILE_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/programs
        )

        # Copy .glsl files to the same directory as the executable
        file(GLOB GLSL_FILES "${SOURCE_SHADERS_DIR}/*.glsl")
        foreach(GLSL_FILE ${GLSL_FILES})
            file(COPY ${GLSL_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs/shaders)
        endforeach()

        # Copy texture files to the same directory as the executable
        file(GLOB ALL_FILES "${SOURCE_TEXTURE_DIR}/*")
        foreach(FILE ${ALL_FILES})
            file(COPY ${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs/textures)
        endforeach()
 
        # Copy texture files to the same directory as the executable
        file(GLOB ALL_FILES "${SOURCE_MODELS_DIR}/*")
        foreach(FILE ${ALL_FILES})
            file(COPY ${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs/models)
        endforeach()
        
    endforeach()
endif()

