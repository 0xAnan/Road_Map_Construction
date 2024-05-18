#pragma once
#include<iostream>
#include<list>
#include<utility>
#include<unordered_map>
#include <stack>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <sstream>
#include<queue>
using namespace std;




class graph{
public:
    unordered_map<string, list<pair<string, int>>> mymap;
    //----------------------------------------------------CITY----------------------------------------------------//
    void addcity(string newcity);

    bool checkcity(string cityname);

    void deletecity(string cityname);

    void update_cityname(string cityname,string newname);
    //----------------------------------------------------EDGE----------------------------------------------------//
    void addedge(string city1, string city2, int km);

    bool checkedge(string city1, string city2);

    void delete_edge(string city1, string city2);

    void update_edge(string city1,string city2,int km);

    //-----------------------------------------------------MAP-----------------------------------------------------//
    bool checkmap();

    void clearmap();

    int  totaldistance();

    void printadjcentlist();

    int number_of_cities();

    int citycount=0;

    //-----------------------------------------------------ALGORITHMS---------------------------------------------------//

    string DFS(string start_city);

    string BFS(string startcity);

    string Dijkstra(string startcity);

    string findMinKey(const unordered_map<string, int>& key, const unordered_map<string, bool>& visited);

    pair<string, list<string>> Prims(unordered_map<string, list<pair<string, int>>> mymap);

    void Kruskal();

    string topological_sort();
};
 //GRAPH_H
