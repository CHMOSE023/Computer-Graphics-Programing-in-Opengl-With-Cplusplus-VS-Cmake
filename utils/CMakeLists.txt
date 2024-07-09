cmake_minimum_required(VERSION 3.25)
project(mi_proyecto)  

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SOIL2 REQUIRED)

#target_link_libraries(${SOURCE_FILE_NAME} glad glfw glm::glm soil2 ${OPENGL_LIBRARIES} utilities)

set(MAIN_SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/Sphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Torus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utils.cpp
)

add_library(utilities SHARED ${MAIN_SOURCE_FILES})

target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(utilities PRIVATE
    glad
    glfw
    glm::glm
    ${OPENGL_LIBRARIES}
    soil2
)