cmake_minimum_required(VERSION 3.28)
project(Road_Map_Construction)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


set(Qt6_DIR "D:\\Qt\\Qt6.7.0\\6.7.0\\mingw_64")


find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)


add_executable(Road_Map_Construction "Main Classes/main.cpp"
        "Main Classes/FIle_Handler.cpp"
        "Headers/FIle_Handler.h"
        "Headers/Graph.h"
        "Headers/User_Interface.h"
        "Graph Classes/Graph.cpp"
        "Graph Classes/BFS.cpp"
        "Graph Classes/DFS.cpp"
        "Graph Classes/Prims_Algorithm.cpp"
        "User Interface Classes/Graph_Managment.cpp"
        "User Interface Classes/Graph_Algorithms.cpp"
        "GUI Classes/graphwidget.h"
)


target_link_libraries(Road_Map_Construction PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Charts)