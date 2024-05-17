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
    int citycount=0;
    bool checkmap();
    bool checkedge(string city1, string city2);
    void addedge(string city1, string city2, int km);
    void delete_edge(string city1, string city2);
    bool checkcity(string cityname);
    void addcity(string newcity);
    void deletecity(string cityname);
    int number_of_cities();
    void printadjcentlist();
    void clearmap();
    void update_cityname(string cityname,string newname);
	void update_edge(string city1,string city2,int km);

    string DFS(string start_city);
    string BFS(string startcity);
    string Dijkstra(string startcity);
    string findMinKey(const unordered_map<string, int>& key, const unordered_map<string, bool>& visited);
    pair<string, list<string>> Prims(unordered_map<string, list<pair<string, int>>> mymap);

    void Kruskal();

    string topological_sort();
};
 //GRAPH_H
