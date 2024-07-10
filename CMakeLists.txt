cmake_minimum_required(VERSION 3.25)

project(graphics VERSION 2.0.1 LANGUAGES CXX)

set(LIBRARIES_DIR libs/glad)
set(LIBRARIE_UTILS_DIR utils)
set(LIBRARIES glad)

add_subdirectory(${LIBRARIES_DIR})
add_subdirectory(${LIBRARIE_UTILS_DIR})

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SOIL2 REQUIRED)

set(SOURCE_MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SOURCE_TEXTURE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/texture)
set(SOURCE_MODELS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/models)
file(GLOB SOURCE_FILES ${SOURCE_MAIN_DIR}/*.cpp)

option(OPTION "compile just one programan" ON)

if(OPTION)

    set(UTILS_MAIN_DIR utils)
    
    set(EXECUTABLE_NAME program_6_3)    
    add_executable(${EXECUTABLE_NAME}
        ${SOURCE_MAIN_DIR}/${EXECUTABLE_NAME}.cpp
    )

    target_link_libraries(${EXECUTABLE_NAME} glad utilities glfw  glm::glm soil2 ${OPENGL_LIBRARIES})

    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/programs
    )

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/vertex_shader61.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/fragment_shader61.glsl DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models/cat.obj DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/texture/Cat_diffuse.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)


else()
    foreach(SOURCE_FILE ${SOURCE_FILES})
        get_filename_component(SOURCE_FILE_NAME ${SOURCE_FILE} NAME_WE)
        
        message(STATUS "Adding program: ${SOURCE_FILE_NAME}")
        
        add_executable(${SOURCE_FILE_NAME}
            ${SOURCE_FILE}
        )

        target_link_libraries(${SOURCE_FILE_NAME} PRIVATE glad glfw glm::glm soil2 ${OPENGL_LIBRARIES} utilities)
        
        set_target_properties(${SOURCE_FILE_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/programs
        )

        # Copy .glsl files to the same directory as the executable
        file(GLOB GLSL_FILES "${SOURCE_MAIN_DIR}/*.glsl")
        foreach(GLSL_FILE ${GLSL_FILES})
            file(COPY ${GLSL_FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
        endforeach()

        # Copy texture files to the same directory as the executable
        file(GLOB ALL_FILES "${SOURCE_TEXTURE_DIR}/*")
        foreach(FILE ${ALL_FILES})
            file(COPY ${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
        endforeach()
 
        # Copy texture files to the same directory as the executable
        file(GLOB ALL_FILES "${SOURCE_MODELS_DIR}/*")
        foreach(FILE ${ALL_FILES})
            file(COPY ${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/programs)
        endforeach()
        
    endforeach()
endif()

