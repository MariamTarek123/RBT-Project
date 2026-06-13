cmake_minimum_required(VERSION 3.16)
project(RBT_GUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt's MOC, RCC, and UIC automated compilers
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Try finding Qt6 first, then fallback to Qt5
find_package(Qt6 COMPONENTS Widgets REQUIRED QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
endif()

# Source and header files lists
set(SOURCES
    RBT-Project/src/MatrixManager.cpp
    RBT-Project/src/RBNode.cpp
    RBT-Project/src/RBTree.cpp
    RBT-Project/src/main.cpp
    RBT-Project/src/MainWindow.cpp
    RBT-Project/src/GridWidget.cpp
    RBT-Project/src/TreeView.cpp
)

set(HEADERS
    RBT-Project/include/MatrixManager.h
    RBT-Project/include/RBNode.h
    RBT-Project/include/RBTree.h
    RBT-Project/include/MainWindow.h
    RBT-Project/include/GridWidget.h
    RBT-Project/include/TreeView.h
)

add_executable(RBT_GUI ${SOURCES} ${HEADERS})

target_include_directories(RBT_GUI PRIVATE RBT-Project/include)

if(Qt6_FOUND)
    target_link_libraries(RBT_GUI PRIVATE Qt6::Widgets)
else()
    target_link_libraries(RBT_GUI PRIVATE Qt5::Widgets)
endif()
