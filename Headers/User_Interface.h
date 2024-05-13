#pragma once
#include "graph.h"
#include<string>

class User_Inrerface
{

public:
    void updateGraphData();
    int AddCity(string nameOfCity, graph& myGraph);
    int AddEdge(string nameOfCity1, string nameOfCity2, int distance, graph& myGraph);
    int DeleteEdge(string cityName1, string cityName2, graph& myGraph);
    int DeleteCity(string nameCity, graph& myGraph);
    int EditEdge(string nameOfCity1, string nameOfCity2, int distance, graph& myGraph);
    int UpdateName(string nameOfCity1, string nameOfCity2, graph& myGraph);
    int ClearMap( graph& myGraph);
    string TraverseBfs(string nameofcity, graph &myGraph);

    string Dijkstra(string nameofcity, graph &myGraph);

    string TraverseDfs(string nameofcity, graph &myGraph);
};

