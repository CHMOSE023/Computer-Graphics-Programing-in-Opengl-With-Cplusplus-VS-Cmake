cmake_minimum_required(VERSION 3.25)
project(soil3 LANGUAGES CXX C VERSION 1.0.0)

option(SOIL2_BUILD_TESTS "Build tests")

find_package(OpenGL REQUIRED)

add_library(soil3 STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/image_DXT.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/image_helper.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/SOIL2.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/wfETC.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/common/common.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/pkm_helper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/pvr_helper.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stb_image.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stb_image_write.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_DDS.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_DDS_c.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_ext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_ext_c.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_pkm.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_pkm_c.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_pvr.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/stbi_pvr_c.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/wfETC.h"
)

target_compile_options(soil3 PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-fPIC>
    $<$<CXX_COMPILER_ID:GNU>:-fPIC>
)

target_include_directories(soil3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common/>
)

target_link_libraries(soil3 PRIVATE OpenGL::GL)

#if(soil3_BUILD_TESTS)
#    find_package(SDL2 REQUIRED)

#    add_executable(soil3_test
#        ${CMAKE_CURRENT_SOURCE_DIR}/test/test_soil3.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/common/common.cpp
#    )
#    target_link_libraries(soil3_test SOIL2 SDL2::SDL2 OpenGL::GL)

#    add_executable(soil3_perf_test
#        ${CMAKE_CURRENT_SOURCE_DIR}/perf_test/test_perf_soil3.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/common/common.cpp
#    )
#    target_link_libraries(soil3_perf_test SOIL2 SDL2::SDL2 OpenGL::GL)

    # Create symlink to test images
#    add_custom_command(
#        TARGET soil3_test PRE_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy_directory
#       ${CMAKE_SOURCE_DIR}/bin/ $<TARGET_FILE_DIR:soil3_test>
#    )
#endif()
