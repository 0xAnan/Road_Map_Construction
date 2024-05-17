#include "Headers/Graph.h"



//----------------------------------------------------CITY----------------------------------------------------//
void graph ::addcity(string newcity) {
    mymap.emplace(newcity,list<pair<string,int>>());
    citycount++;
}

int graph::number_of_cities(){
    return citycount;
}

bool graph::checkcity(string cityname) {
    transform(cityname.begin(), cityname.end(), cityname.begin(), ::tolower);
    for (const auto& pair : mymap) {
        string key = pair.first;
        transform(key.begin(), key.end(), key.begin(), ::tolower);
        if (key == cityname) {
            return true;
        }
    }
    return false;
}

void graph::update_cityname(string cityname, string newname) {
    if(checkcity(cityname) && !checkcity(newname)) {
        auto it = mymap.find(cityname);
        if(it != mymap.end()) {
            // Copy the edges and erase the old city
            list<pair<string, int>> edges = it->second;
            mymap.erase(it);
            mymap[newname] = edges;

            // Update the city name in all adjacency lists
            for(auto& pair : mymap) {
                for(auto& edge : pair.second) {
                    if(edge.first == cityname) {
                        edge.first = newname;
                    }
                }
            }
        }
    }
}


void graph::deletecity(string cityname) {
    if (checkcity(cityname)) {
        // Delete edges associated with cityname
        for (auto& pair : mymap) {
            string adjacentCity = pair.first;
            if (adjacentCity != cityname) {
                // If there is an edge from adjacentCity to cityname, delete it
                if (checkedge(adjacentCity, cityname)) {
                    delete_edge(adjacentCity, cityname);
                }
            }
        }

        // Delete cityname from the map
        mymap.erase(cityname);
    } else {
    }
}

//----------------------------------------------------EDGE----------------------------------------------------//
void graph:: addedge(string city1, string city2, int km)
{
    mymap[city1].push_back(make_pair(city2, km));
    mymap[city2].push_back(make_pair(city1, km));
}

bool graph:: checkedge(string city1, string city2) {
    if (checkcity(city1)) {
        for (auto edge : mymap[city1]) {
            if (edge.first == city2) {
                return true;
            }
        }
    }
    return false;
}

void graph:: delete_edge(string city1, string city2) {
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
}

void graph::update_edge(string city1,string city2,int km){

    if (checkedge(city1,city2)) {
        delete_edge(city1, city2);
    }
    mymap[city1].push_back(make_pair(city2, km));
    mymap[city2].push_back(make_pair(city1, km));
}

//-------------------------------------------------------MAP---------------------------------------------------------//
bool graph::checkmap() {
    if(citycount!=0) {
        return true;
    }
    else {
        return false;
    }
}

void graph:: clearmap(){
    if(checkmap())
        mymap.clear();
    citycount=0;

}

void graph:: printadjcentlist()
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
//-----------------------------------------------------ALGORITHMS-----------------------------------------------//
string graph::DFS(string start_city)
{
    unordered_set<string> visited;
    stack<string> stack;
    string output;
    stack.push(start_city);
    while (!stack.empty()) {
        string current_city = stack.top();
        stack.pop();

        if (visited.find(current_city) == visited.end()) {
            visited.insert(current_city);
            output+=current_city+" -> ";

            for (auto neighbor : mymap[current_city]) {
                if (visited.find(neighbor.first) == visited.end()) {
                    stack.push(neighbor.first);
                }
            }
        }
    }

    // Remove the trailing " -> " from the output string
    if (output.length() > 4) {
        output = output.substr(0, output.length() - 4);
    }

    return output;
}

string graph::BFS(string startcity)
    {
        unordered_map<string,bool>visted;
        queue<string>q;
        string result;
        visted[startcity]=true;
        q.push(startcity);
        while(! q.empty()){
            string currentcity=q.front();
            q.pop();
            result += currentcity+" -> ";
            for(auto neighbor : mymap[currentcity])
            {
                string neighborcity=neighbor.first;
                if(!visted[neighborcity])
                {
                    visted[neighborcity]=true;
                    q.push(neighborcity);
                }
            }
        }
    // Remove the trailing " -> " from the result string
    if (result.length() > 4) {
        result = result.substr(0, result.length() - 4);
    }
    return result;
    }

string graph::Dijkstra(string startcity){
    priority_queue<pair<int,string>,vector<pair<int,string>>, greater<pair<int,string>>> pq;
    unordered_map<string,int>distances;
    unordered_map<string,list<string>>paths;
    for(const auto & city : mymap){
    distances[city.first]=numeric_limits<int>::max();
        paths[city.first]=list<string>();
    }
    distances[startcity]=0;
    pq.push(make_pair(0,startcity));
    unordered_set<string>visited;
    while(! pq.empty()){
    pair<int,string> current=pq.top();
    pq.pop();
    if(visited.count(current.second)>0)continue;
    visited.insert(current.second);
    for(const auto & neighbor : mymap[current.second]){
    string neighbor_city=neighbor.first;
    int distance_to_neighbor=neighbor.second;
    int total_distance=distances[current.second] +  distance_to_neighbor;
    if(distances[neighbor_city] > total_distance){
        distances[neighbor_city]=total_distance;
        paths[neighbor_city]=paths[current.second];
        paths[neighbor_city].push_back(current.second);
    }else if(distances[neighbor_city]==total_distance)
    {
        paths[neighbor_city].push_back(current.second);
    }
        pq.push(make_pair(total_distance, neighbor_city));
    }
    }
    stringstream result;
    for(const auto & city : mymap){
    string city_name=city.first;
    int distance=distances[city_name];
    if(distance == numeric_limits<int>::max()){
    result << city_name << " : unreachable" <<endl;
    }else{
     result << endl<<city_name << " : "<<distance<<"KM" <<endl;
        if(!paths[city_name].empty())
        {
            result<<"Shortest Path:"<<endl;
            bool first =0;
            for(const auto &path: paths[city_name])
            {
                if (first == 0) {
                    result  <<path ;
                    first++;
                }else
                    result <<" -> " <<path ;
            }
            result << endl;
        }
    }
    }
    return result.str();
    }

string graph::findMinKey(const unordered_map<string, int>& key, const unordered_map<string, bool>& visited) {
    int min = INT_MAX;
    string minKey;

    for (const auto& pair : key) {
        //since both visited and key have the .first which is the city name we initialized above
        // we will use variable 'pair' to access city name on visited map and to access min value "key value/weight value"
        //if first element (city) in visited not visited and first element (city) key(weight) value smaller than min
        if (!visited.at(pair.first) && pair.second < min) {
            // << "\n\nvis: " << visited.at(pair.first) << endl;
            min = pair.second; //save weight of key in min var
            minKey = pair.first; //get city name
        }
    }

    return minKey;
}

pair<string, list<string>> graph::Prims(unordered_map<string, list<pair<string, int>>> mymap) {
    unordered_map<string, bool> visited;
    unordered_map<string, int> key;
    unordered_map<string, string> parent;
    int totalWeight = 0;
    //initialize visited map & key map
    for (const auto& pair : mymap) {
        string c1 = pair.first;
        key[c1] = INT_MAX;
        visited[c1] = false;
    }

    // Start with the first vertex
    string startVertex = mymap.begin()->first;
    //The weight of the first element always equals 0
    key[startVertex] = 0;
    //number of vertices
    for (int i = 0; i < mymap.size() - 1; ++i) {
        //we found the min key (weight) and visited(true)
        string u = findMinKey(key, visited);
        visited[u] = true;

        //iterating inside specific element in the graph
        //by another words iterating in the list of specific array index
        //accessing the list of u (neighbors)
        for (const auto& neighbor : mymap[u]) {
            // v is the name of the city connected to u city
            string v = neighbor.first;
            // v is the weight of the edge connecting u city to v city
            //it's weight in the map
            int weight = neighbor.second;
            //checking.. if that neighbor(v) not visited & neighbor(v)
            //checking.. if neighbor(v) weight smaller than it's value in key array
            if (!visited[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }

    pair<string, list<string>> MSTpair;
    for (const auto& pair : parent) {
        string line = pair.second + " - " + pair.first;
        MSTpair.second.push_back(line);
        totalWeight += key[pair.first];
    }
    MSTpair.first = to_string(totalWeight);

    return MSTpair;
}