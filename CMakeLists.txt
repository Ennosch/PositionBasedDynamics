cmake_minimum_required(VERSION 3.14)
#cmake_minimum_required(VERSION 3.0.2)
project(QtOpenGL)
#https://cmake.org/cmake/help/v3.0/manual/cmake-qt.7.html#manual:cmake-qt(7)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};/Users/enno/Qt/5.10.1/clang_64")
include_directories(include)


find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5OpenGL CONFIG REQUIRED)

message("FOUND Qt5OpenGL-----------" ${Qt5OpenGL_FOUND})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_VERBOSE_MAKEFILE TRUE)


set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_AUTOMOC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_AUTORCC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(QtOpenGL MACOSX_BUNDLE
    src/main.cpp
    src/inputManager.cpp
    src/Scene.cpp
    src/Window.cpp
    src/transform.cpp


    resources.qrc

    include/inputManager.h
    include/Scene.h
    include/transform.h
    include/Window.h

    ui_ControlWidget.h

)

#target_link_libraries(QtOpenGL Qt5::Widgets )
#target_link_libraries(QtOpenGL Qt5::Widgets ${Qt5OpenGL_LIBS})

target_link_libraries(QtOpenGL Qt5::Widgets Qt5::OpenGL)
