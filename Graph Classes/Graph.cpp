#include<iostream>
#include<list>
#include<utility>
#include<unordered_map>
using namespace std;

class City
{
public:
    string cityname;
    City() {
        cityname = " ";
    }
    City(string cname) {
        cityname = cname;
    }
    string getCityname() {
        return cityname;
    }
    string setCityname(string cname) {
        cityname = cname;
        return cityname;
    }
    void update_cityname(string cname) {
        cityname = cname;
        cout << "City name changed succesfully";
    }
};

class graph
{
public:
    unordered_map<string, list<pair<string, int>>> mymap;

    bool checkedge(string city1, string city2) {
        if (checkcity(city1)) {
            for (auto edge : mymap[city1]) {
                if (edge.first == city2) {
                    return true;
                }
            }
        }
        return false;
    }

    void addedge(string city1, string city2, int km)
    {
        bool check1 = checkcity(city1);
        bool check2 = checkcity(city2);
        bool check3 = checkedge(city1, city2);
        if (check1 && check2 == true) {
            if (check3 == true) {
                cout << "The Edge between " << city1 << " and " << city2 << "already exists";
            }
            else {
                mymap[city1].push_back(make_pair(city2, km));
                mymap[city2].push_back(make_pair(city1, km));
                cout << "Edge between " << city1 << " and " << city2 << "added succesfully";
            }
        }
        else {
            cout << "Invalid cities entered ";
        }
    }

    void delete_edge(string city1, string city2) {
        bool check = checkedge(city1, city2);
        if (check == true) {
            for (auto it = mymap[city1].begin(); it != mymap[city1].end(); ) {
                if (it->first == city2) {
                    it = mymap[city1].erase(it);
                }
                else {
                    ++it;
                }
            }
            for (auto it = mymap[city2].begin(); it != mymap[city2].end(); ) {
                if (it->first == city1) {
                    it = mymap[city2].erase(it);
                }
                else {
                    ++it;
                }
            }
            cout << "Edge deleted successfully ";
        }
        else {
            cout << "One or both cities doesnt exist ";
        }
    }

    bool checkcity(string cityname) {
        if (mymap.find(cityname) != mymap.end()) {
            return true;
        }
        else {
            return false;
        }
    }
    void addcity(City newcity) {
        bool check = checkcity(newcity.getCityname());
        if (check == true) {
            cout << "City with this name already exists ";
        }
        else {
            mymap[newcity.getCityname()] = list<pair<string, int>>();
            cout << "City added successfully.";
        }
    }

    void deletecity(string cityname) {
        if (checkcity(cityname)) {
            mymap.erase(cityname);
            cout << "City deleted succesfully ";
        }
        else {
            cout << "City already doesnt exist ";
        }
    }

    void printadjcentlist()
    {
        for (auto c : mymap)
        {
            string city = c.first;
            list<pair<string, int>> details = c.second;
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
