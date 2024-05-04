#include<iostream>
#include<list>
#include<utility>
#include<unordered_map>
using namespace std;
class graph
{
public:
    unordered_map<string, list<pair<string, int>>> mymap;
    void addedge(string city1, string city2,int km)
    {
        mymap[city1].push_back(make_pair(city2, km));
        mymap[city2].push_back(make_pair(city1, km));

    }
    void printadjcentlist()
    {
        for (auto c : mymap)
        {
            string city = c.first;
            list<pair<string, int>> details=c.second;
            for (auto dis : details)
            {
                cout << " destenation: " << dis.first << endl;
                cout << " distance from " << c.first << " to " << dis.first;
                cout << dis.second << " KM" << endl;
                cout << "-------------------------------" << endl;
            }
        }
    }
};
