
#pragma once
#include"Headers/Graph.h";
#include"Headers/User_Interface.h";
#include<list>
#include<utility>
#include<unordered_map>
#include<queue>
#include <iostream>
#include<string>
/*
void UI::updateGraphData()
{
    int i; // Initialize the variable 'i'
    string nameOfCity; // Declare 'nameOfCity' outside the switch

    do {
        cout << "Press 1 to add a city\n"
            << "Press 2 to add an edge\n"
            << "Press 3 to delete a city\n"
            << "Press 4 to delete an edge\n"
            << "Press 5 to exit\n";

        cout << "Enter your choice: ";
        cin >> i; // Get user input for 'i'

            if (i == 1) {
            City newcity;
                cout << "Enter the name of the city: ";
                cin >> nameOfCity;
                newcity.setCityname(nameOfCity);
                myGraph.addcity(newcity);
            }
            else if (i == 2) {
                string nameOfCity1, nameOfCity2;
                int distance;
                cout << "Enter the name of city 1: ";
                cin >> nameOfCity1;
                cout << "Enter the name of city 2: ";
                cin >> nameOfCity2;
                cout << "Enter the distance (in KM): ";
                cin >> distance;
                myGraph.addedge(nameOfCity1, nameOfCity2, distance);
            }
            else if (i == 3)
            {
                string nameCity;
                cout << "enter the name of city: ";
                cin >> nameCity;
                myGraph.deletecity(nameCity);
            }
            else if (i == 4)
            {
                string cityName1, cityName2;
                cout << "to delete edge you must enter two city /n";
                cout << "enter  the city 1:";
                cin >> cityName1;
                cout << "enter  the city 2:";
                cin >> cityName2;
                myGraph.delete_edge(cityName1, cityName2);
            }
            else {
                cout << "Invalid choice. Please select a valid option.\n";
            }

    } while (i != 5); // Exit when user selects option 5
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
0 fail
1 success
2 the city was existing
*/
int UI::disp_AddCity(string nameOfCity)
{


    // cout << "Enter the name of the city: ";
    //  cin >> nameOfCity;
    if (!myGraph.checkcity(nameOfCity))
    {


        myGraph.addcity(nameOfCity);
        return 1;
    }
    else if (myGraph.checkcity(nameOfCity))\
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
/*
0 fail
1 success
2 city 1 or 2 was not existing
3 the edge was existing
*/
int UI::AddEdge(string nameOfCity1,string nameOfCity2,int distance)
{

    //cout << "Enter the name of city 1: ";
    //cin >> nameOfCity1;
    //cout << "Enter the name of city 2: ";
    // cin >> nameOfCity2;
    //       cout << "Enter the distance (in KM): ";
    //     cin >> distance;
    if (myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)) {
        if (!(myGraph.checkedge(nameOfCity1, nameOfCity2)))
        {

            myGraph.addedge(nameOfCity1, nameOfCity2, distance);
            return 1;
        }
        else if (myGraph.checkedge(nameOfCity1, nameOfCity2))
        {
            return 3;
        }
        else
            return 0;
    }
    else if(!(myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
/*
0 fail
1 success
2 the city was not existing
*/
int UI::DeleteCity(string nameCity)
{
    // cout << "enter the name of city: ";
    //   cin >> nameCity;
    if (myGraph.checkcity(nameCity)) {
        myGraph.deletecity(nameCity);
        return 1;
    }
    else if (!myGraph.checkcity(nameCity))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
/*
0 fail
1 success
2 city 1 or 2 was not existing
3 the edge was not existing
*/
int UI::DeleteEdge(string nameOfCity1, string nameOfCity2)
{
    //cout << "to delete edge you must enter two city /n";
    //cout << "enter  the city 1:";
    //cin >> cityName1;
    //cout << "enter  the city 2:";
    //cin >> cityName2;
    if (myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2))
    {
        if (myGraph.checkedge(nameOfCity1, nameOfCity2))
        {
            myGraph.delete_edge(nameOfCity1, nameOfCity2);
            return 1;
        }
        else if (!(myGraph.checkedge(nameOfCity1, nameOfCity2)))
        {
            return 3;
        }
        else
        {
            return 0;
        }

    }
    else if (!(myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
/*
0 fail
1 success
2 city 1 or 2 was not existing
3 the edge was not existing
*/
int UI::EditEdge(string nameOfCity1, string nameOfCity2,int distance)
{
    if (myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2))
    {
        if (myGraph.checkedge(nameOfCity1, nameOfCity2))
        {
            myGraph.update_edge(nameOfCity1, nameOfCity2,  distance);
            return 1;
        }
        else if (!(myGraph.checkedge(nameOfCity1, nameOfCity2)))
        {
            return 3;
        }
        else
        {
            return 0;
        }

    }
    else if (!(myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
/*
0 fail
1 success
2 the city was not existing
3 the new name was taken
*/
int UI::UpName(string nameOfCity1, string nameOfCity2)
{
    if (myGraph.checkcity(nameOfCity1))
    {
        if (!(myGraph.checkcity(nameOfCity2)))
        {
            myGraph.update_cityname(nameOfCity1,nameOfCity2);
            return 1;
        }
        else if (myGraph.checkcity(nameOfCity2))
        {
            return 3;
        }
        else
        {
            return 0;
        }
    }
    else if (!(myGraph.checkcity(nameOfCity1)))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
/*
0 fail
1 success
*/
int UI::ClearMap()
{
    if (myGraph.checkmap())
    {
        myGraph.clearmap();
        return 1;
    }
    else
        return 0;
}
/*
0 fail
1 success
2 the city was existing
*/
int UI::TraverDfs(string nameofcity)
{
    if (myGraph.checkcity(nameofcity))
    {
        myGraph.DFS(nameofcity);
        return 1;
    }
    else if (!myGraph.checkcity(nameofcity))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
/*
0 fail
1 success
2 the city was existing
*/
int UI::TraverBfs(graph traverGraph,string nameofcity)
{
    if (myGraph.checkcity(nameofcity))
    {

        myGraph.BFS_algo(traverGraph,nameofcity);
        return 1;
    }
    else if (!myGraph.checkcity(nameofcity))
    {
        return 2;
    }
    else
    {
        return 0;
    }
}//
// Created by pc on 4/24/2024.
//
