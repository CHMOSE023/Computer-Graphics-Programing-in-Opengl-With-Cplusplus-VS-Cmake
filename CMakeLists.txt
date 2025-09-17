cmake_minimum_required(VERSION 3.8) 

project(graphics)
 
set(LIBRARIES_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glad
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/soild2
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
) 

#  全局定义 M_PI 宏
add_definitions(-DM_PI=3.14159265358979323846)

foreach(dir ${LIBRARIES_DIR})
    add_subdirectory(${dir})
endforeach()
 
 # OpenGL.lib
find_package(OpenGL REQUIRED)
     
set(SOURCE_MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCE_TEXTURE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/textures)
set(SOURCE_MODELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/models)
set(SOURCE_SHADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)
set(SOURCE_CUBEMAP1_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/textures/cubeMap1)
set(SOURCE_CUBEMAP2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/textures/cubeMap2)

file(GLOB SOURCE_FILES ${SOURCE_MAIN_DIR}/*.cpp)

option(OPTION "compile all programans" OFF)
 
if(NOT OPTION) 

    set(UTILS_MAIN_DIR utils) 
    set(EXECUTABLE_NAME program_2_1)    
    add_executable(${EXECUTABLE_NAME}
        ${SOURCE_MAIN_DIR}/${EXECUTABLE_NAME}.cpp
    ) 
    target_link_libraries(${EXECUTABLE_NAME} glad glfw ${OPENGL_LIBRARIES}) 
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/programs
    ) 
else()

    set(RUNTIME_OUTPUT_DIRECTORY_PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/programs)
    set(RUNTIME_OUTPUT_DIRECTORY_SHADERS ${CMAKE_CURRENT_BINARY_DIR}/programs/shaders)
    set(RUNTIME_OUTPUT_DIRECTORY_TEXTURES ${CMAKE_CURRENT_BINARY_DIR}/programs/textures)
    set(RUNTIME_OUTPUT_DIRECTORY_CUBEMAP1 ${CMAKE_CURRENT_BINARY_DIR}/programs/textures/cubeMap1)
    set(RUNTIME_OUTPUT_DIRECTORY_CUBEMAP2 ${CMAKE_CURRENT_BINARY_DIR}/programs/textures/cubeMap2)
    set(RUNTIME_OUTPUT_DIRECTORY_MODELS ${CMAKE_CURRENT_BINARY_DIR}/programs/models)

    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_PROGRAMS})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_SHADERS})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_TEXTURES})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_MODELS})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_CUBEMAP1})
    file(MAKE_DIRECTORY ${RUNTIME_OUTPUT_DIRECTORY_CUBEMAP2}) 

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

        # Copy cubemap files to the same directory as the executable
        file(GLOB ALL_FILES "${SOURCE_CUBEMAP1_DIR}/*")
        foreach(FILE ${ALL_FILES})
            file(COPY ${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs/textures/cubeMap1)
        endforeach()

         # Copy cubemap files to the same directory as the executable
        file(GLOB ALL_FILES "${SOURCE_CUBEMAP2_DIR}/*")
        foreach(FILE ${ALL_FILES})
            file(COPY ${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs/textures/cubeMap2)
        endforeach()       
 
    endforeach()
endif()

