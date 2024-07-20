cmake_minimum_required (VERSION 3.25)

project(glad VERSION 1.0.0 LANGUAGES C)

set(LIBRARY_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h
)

set(LIBRARY_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
)

add_library(glad SHARED ${LIBRARY_SOURCE})
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
