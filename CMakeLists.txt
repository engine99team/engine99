cmake_minimum_required(VERSION 3.12)
project(engine99 C)
set(CMAKE_C_STANDARD 99)

set(GAME_DIR game)
set(MODULES core physics graphics ${GAME_DIR})

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders/)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

include_directories(ThirdParty)
file(GLOB THIRD_PARTY_SRC ThirdParty/*.c)
add_library(ThirdPartyLibs ${THIRD_PARTY_SRC})

foreach(MODULE ${MODULES})
    file(GLOB MODULE_SRC ${MODULE}/src/*.c)
    set(MODULE_INCLUDES ${MODULE}/include/)
    include_directories(${MODULE_INCLUDES})
    add_library(${MODULE} ${MODULE_SRC})
endforeach()

add_executable(engine99 main.c)
target_link_libraries(engine99 ${MODULES} ThirdPartyLibs ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})