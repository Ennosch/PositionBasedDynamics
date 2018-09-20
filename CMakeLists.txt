cmake_minimum_required(VERSION 3.0.2)
project(QtOpenGL)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

#set(CMAKE_PREFIX_PATH ~/Qt/5.10.1/clang_64/lib/cmake)
#message('log------------------------:' ${CMAKE_PREFIX_PATH})

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)
find_package(assimp)
find_package(eigen3)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCES "src/*.cpp")
file(GLOB INCLUDES "include/*.h")
add_executable(PlayCmake
    ${SOURCES}
    ${INCLUDES}
    resources.qrc
)

target_link_libraries(PlayCmake Qt5::Widgets Qt5::OpenGL assimp)
