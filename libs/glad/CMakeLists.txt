cmake_minimum_required (VERSION 3.25)

set(PROJECT_NAME
    GLADlib
)

set(LIBRARY_NAME
    glad
)

set(LIBRARY_HEADERS_DIR
    include 
)

set(LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/glad/glad.h
    ${LIBRARY_HEADERS_DIR}/KHR/khrplatform.h
)

set(LIBRARY_SOURCE_DIR
    src
)

set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/glad.c
)

project(${PROJECT_NAME} LANGUAGES C )

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCE})
target_include_directories(${LIBRARY_NAME} PUBLIC ${LIBRARY_HEADERS_DIR} )
