cmake_minimum_required(VERSION 3.1.0)

project(tevian VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(tevian
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    myview.cpp
    myview.h
    myitem.cpp
    rest.cpp
    meta.cpp
    


)

target_link_libraries(tevian Qt5::Widgets)
target_link_libraries(tevian Qt5::Network)
