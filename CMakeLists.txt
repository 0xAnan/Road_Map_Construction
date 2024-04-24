cmake_minimum_required(VERSION 3.28)
project(Road_Map_Construction)

set(CMAKE_CXX_STANDARD 17)

add_executable(Road_Map_Construction "Main Classes/main.cpp"
        "Main Classes/FIle_Handler.cpp"
        "Header Files/FIle_Handler.h"
        "Header Files/Graph.h"
        "Header Files/User_Interface.h"
        "Graph Classes/Graph.cpp"
        "Graph Classes/BFS.cpp"
        "Graph Classes/DFS.cpp"
        "Graph Classes/Prims_Algorithm.cpp"
        "User Interface Classes/Graph_Managment.cpp"
        "User Interface Classes/Graph_Algorithms.cpp")
