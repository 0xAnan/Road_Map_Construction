//
// Created by pc on 4/24/2024.
//

#ifndef GRAPH_H
#define GRAPH_H
#include<iostream>
#include<list>
#include<utility>
#include<unordered_map>
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
    bool checkedge(string city1, string city2);
    void addedge(string city1, string city2, int km);
    void delete_edge(string city1, string city2);
    bool checkcity(string cityname);
    void addcity(City newcity);
    void deletecity(string cityname);
    void printadjcentlist();
};
#endif //GRAPH_H
