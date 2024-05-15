#include"Headers/Graph.h"
#include"Headers/User_Interface.h"
#include <string>
/*
Cases:
1 fail
0 success
2 the city was existing
*/
int User_Inrerface::AddCity(string nameOfCity , graph& myGraph)
{
    if (!(myGraph.checkcity(nameOfCity)))
    {
        myGraph.addcity(nameOfCity);
        cout<<nameOfCity <<" Has Been Added Successfully\n";
        return 0;
    }
    cout<<"City Already Exists\n";
    return 2;
}
/*
Cases:
1 fail
0 success
2 city 1 or 2 was not existing
3 the edge was existing
4 Wrong Distance Input
5 Can't add edge form a city to itself
*/
int User_Inrerface::AddEdge(string nameOfCity1,string nameOfCity2,int distance, graph& myGraph)
{
    if (myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)) {
        if (!(myGraph.checkedge(nameOfCity1, nameOfCity2)))
        {
            if (distance == 0) {
                cout<<"Distance Is Empty Or Wrong\n";
                return 4;
            }
            else if (nameOfCity1 == nameOfCity2) {
                cout<<"Can't Connect A City To Itself\n";
                return 5;
            }
            else {
                myGraph.addedge(nameOfCity1, nameOfCity2, distance);
                cout<<"Edge Between "<<nameOfCity1<<" And "<<nameOfCity2<<"Has Been Added Successfully\n";
                return 0;
            }
        }
        else if (myGraph.checkedge(nameOfCity1, nameOfCity2))
        {
            cout<<"Edge Already Exists\n";
            return 3;
        }
        else
            cout<<"Adding Edge Failed\n";
            return 1;
    }
    else if(!(myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)))
    {
        cout<<"One Of The Cities Doesn't Exist\n";
        return 2;
    }
    else
    {
        cout<<"Adding Edge Failed\n";
        return 1;
    }
}
/*
1 fail
0 success
2 the city was not existing
*/
int User_Inrerface::DeleteCity(string nameCity, graph& myGraph)
{

    if (myGraph.checkcity(nameCity)) {
        myGraph.deletecity(nameCity);
        cout<<nameCity<<" Has Been Deleted Successfully\n";
        return 0;
    }
    else if (!myGraph.checkcity(nameCity))
    {
        cout<<"City Doesn't Exist\n";
        return 2;
    }
    else
    {
        cout<<"Deleting City Faild\n";
        return 1;
    }
}
/*
1 fail
0 success
2 city 1 or 2 was not existing
3 the edge was not existing
*/
int User_Inrerface::DeleteEdge(string nameOfCity1, string nameOfCity2, graph& myGraph)
{
    if (myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2))
    {
        if (myGraph.checkedge(nameOfCity1, nameOfCity2))
        {
            myGraph.delete_edge(nameOfCity1, nameOfCity2);
            cout<<"Edge Between "<<nameOfCity1<<" And "<<nameOfCity2<<"Has Been Deleted Successfully\n";
            return 0;
        }
        else if (!(myGraph.checkedge(nameOfCity1, nameOfCity2)))
        {
            cout<<"Edge Doesn't Exist\n";
            return 3;
        }
        else
        {
            cout<<"Deleting Edge Failed\n";
            return 1;
        }

    }
    else if (!(myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)))
    {
        cout<<"One Of The Cities Doesn't Exist\n";
        return 2;
    }
    else
    {
        cout<<"Deleting Edge Failed\n";
        return 1;
    }
}
/*
1 fail
0 success
2 city 1 or 2 was not existing
3 the edge was not existing
*/
int User_Inrerface::EditEdge(string nameOfCity1, string nameOfCity2,int distance, graph& myGraph)
{
    if (myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2))
    {
        if (myGraph.checkedge(nameOfCity1, nameOfCity2))
        {
            cout<<"Edge Between "<<nameOfCity1<<" And "<<nameOfCity2<<"Has Been Edited Successfully, New Value -> "<<distance<<endl;
            myGraph.update_edge(nameOfCity1, nameOfCity2,  distance);
            return 0;
        }
        else if (!(myGraph.checkedge(nameOfCity1, nameOfCity2)))
        {
            cout<<"Edge Doesn't Exist\n";
            return 3;
        }
        else
        {
            cout<<"Editing Edge Failed\n";
            return 1;
        }

    }
    else if (!(myGraph.checkcity(nameOfCity1) && myGraph.checkcity(nameOfCity2)))
    {
        cout<<"One Of The Cities Doesn't Exist\n";
        return 2;
    }
    else
    {
        cout<<"Editing Edge Failed\n";
        return 1;
    }
}
/*
1 fail
0 success
2 the city was not existing
3 the new name was taken
*/
int User_Inrerface::UpdateName(string nameOfCity1, string nameOfCity2, graph& myGraph)
{
    if (myGraph.checkcity(nameOfCity1))
    {
        if (!(myGraph.checkcity(nameOfCity2)))
        {
            cout<<nameOfCity1<<" Has Been Updated To "<<nameOfCity2<<endl;
            myGraph.update_cityname(nameOfCity1,nameOfCity2);
            return 0;
        }
        else if (myGraph.checkcity(nameOfCity2))
        {
            cout<<"Name Already Exists\n";
            return 3;
        }
        else
        {
            cout<<"Editing Name Of City Faild\n";
            return 1;
        }
    }
    else if (!(myGraph.checkcity(nameOfCity1)))
    {
        cout<<"City Doesn't Exist\n";
        return 2;
    }
    else
    {
        cout<<"Editing Name Of City Faild\n";
        return 1;
    }
}
/*
1 fail
0 success
*/
int User_Inrerface::ClearMap(graph& myGraph)
{
    if (myGraph.checkmap())
    {
        cout<<"Graph Has Been Cleared Successfully\n";
        myGraph.clearmap();
        return 0;
    }
    cout<<"Graph Couldn't Be Cleared\n";
    return 1;
}
/*
normal string -> success
"1" -> fail
*/
 string User_Inrerface::TraverseDfs(string nameofcity, graph& myGraph){
     if (myGraph.checkcity(nameofcity))
    {
     string s = myGraph.DFS(nameofcity);
        return s;
    }
        return "1";
}
/*
normal string -> success
"1" -> fail
*/
string User_Inrerface::TraverseBfs(string nameofcity, graph &myGraph)
{
    if (myGraph.checkcity(nameofcity))
    {
        string s = myGraph.BFS(nameofcity);
        cout<<"Traversing Cities Using Breadth First Search:\n"<<s<<endl;
        return s;
    }
    return "1";
}
string User_Inrerface::Dijkstra(string nameofcity, graph &myGraph)
{
    if (myGraph.checkcity(nameofcity))
    {
        string s = myGraph.Dijkstra(nameofcity).first;
        cout<<"Using Dijkstra's Algorithm:\n"<<s<<endl;
        return s;
    }
    return "1";
}

pair<string, list<string>> User_Inrerface::Prims(graph &myGraph)
{
        pair<string, list<string>> s = myGraph.Prims(myGraph.mymap);
        cout<<"Using Prim's Algorithm:\n"<<s.first<<endl;
        return s;
}
