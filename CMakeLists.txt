cmake_minimum_required(VERSION 3.12)
project(engine99 C)
set(CMAKE_C_STANDARD 99)

set(GAME_DIR game)
set(MODULES core physics graphics)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(ZLIB REQUIRED)
set(platform_flags "")
set(_SHARED SHARED)
if(APPLE)
    set(platform_flags "-framework OpenGL -ldl -lpthread")
    include_directories("/usr/local/include")
    # Fix linking on 10.14+. See https://stackoverflow.com/questions/54068035
    LINK_DIRECTORIES(/usr/local/lib)
    set(_SHARED STATIC)
elseif(UNIX)
    set(_GNU_SOURCE 1)
    set(platform_flags "-ldl -lpthread")
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${GAME_DIR}/shaders/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${GAME_DIR}/textures/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/textures/)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})

include_directories(ThirdParty)
file(GLOB THIRD_PARTY_SRC ThirdParty/*.c)
add_library(ThirdPartyLibs ${_SHARED} ${THIRD_PARTY_SRC})
foreach(MODULE ${MODULES})
    file(GLOB MODULE_SRC ${MODULE}/src/*.c)
    set(MODULE_INCLUDES ${MODULE}/include/)
    include_directories(${MODULE_INCLUDES})
    add_library(${MODULE} ${_SHARED} ${MODULE_SRC})
endforeach()

file(GLOB GAME_SRC ${GAME_DIR}/src/*.c)
set(GAME_INCLUDES ${GAME_DIR}/include/)
include_directories(${GAME_INCLUDES})
add_library(GameModules ${_SHARED} ${GAME_SRC})

add_executable(engine99 main.c)
target_link_libraries(engine99 GameModules ${MODULES} ThirdPartyLibs
        ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
        ${ZLIB_LIBRARIES} m ${platform_flags} cglm)
