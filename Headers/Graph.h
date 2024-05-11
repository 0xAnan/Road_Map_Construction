#pragma once
#include<iostream>
#include<list>
#include<utility>
#include<unordered_map>
#include<queue>
using namespace std;
class City
{
public:
    string cityname;
    City();
    City(string cname);
    string getCityname();
    string setCityname(string cname);
    void update_cityname(string cname);
};





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
    queue<string> DFS(string start_city);
    queue<string> BFS_algo( graph g,string startcity);
    string Dijkstra_algo(graph g,string startcity);
};
 //GRAPH_H
