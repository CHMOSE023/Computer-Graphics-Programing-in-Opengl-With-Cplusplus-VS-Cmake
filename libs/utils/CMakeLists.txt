cmake_minimum_required(VERSION 3.25)
  
project(mi_proyecto)  
 
set(MAIN_SOURCE_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Sphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Sphere2.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Torus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/modelImporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MeshFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MeshTorus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MeshPyramid.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MeshSphere.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HalfSphere.cpp
)
 
add_library(utilities STATIC ${MAIN_SOURCE_FILES})
 
target_include_directories(utilities PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include/public)
target_include_directories(utilities PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private)
target_include_directories(utilities PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include ) 

target_link_libraries(utilities PRIVATE
    glad
    glfw
    glm::glm
    ${OPENGL_LIBRARIES}
    soil3
)