/*
Student Name:
Student Number:
Project Number: 4
Compile Status: [SUCCESS/FAIL]
Running Status: [SUCCESS/FAIL]
Notes: Anything you want to say about your code that will be helpful in the grading process.
*/
#include <list>
#include <limits>
#include <set>
#include <queue>
#include "HelpStudents.h"

using namespace std;

const long long int INF = numeric_limits<long long int>::max();
list < pair<int, int> > *adjacencyList;//it holds adjacent vertices.
int nodeNumber;//total vertex number
int target;//peek vertice
int edgeNumber;//total edge number

HelpStudents::HelpStudents(int  N, int  M, int K, vector < pair< pair <int,int> , int > > ways) {
    // IMPLEMENT ME!
    nodeNumber = N;
    target = K;
    edgeNumber = M;
    adjacencyList = new list< pair<int, int> >[N+1];
    for(int i = 0; i < edgeNumber; i++){
        int a = ways[i].first.first;//finding and adding adjacent vertices to the list
        int b = ways[i].first.second;
        int dist = ways[i].second;
        adjacencyList[a].push_back(make_pair(b,dist));
        adjacencyList[b].push_back(make_pair(a,dist));
    }
}
//I use Dijkstra's Algorithm here.
//I also look to the slides in moodle and use some code provided there
long long int HelpStudents::firstStudent() {
    // IMPLEMENT ME!
    set< pair<long long int, long long int>> distSet;
    vector<long long int> dist(nodeNumber+1, INF);
    distSet.insert(make_pair(0, 1));
    dist[1] = 0;
    while (!distSet.empty()) {
        pair<long long int, long long int> tmp = *(distSet.begin());
        distSet.erase(distSet.begin());
        int u = tmp.second;
        list< pair<int, int> >::iterator i;
        for (i = adjacencyList[u].begin()++; i != adjacencyList[u].end(); ++i) {
            int v = (*i).first;
            long long int weight = (*i).second;
            if (dist[v] > dist[u] + weight) {
                if (dist[v] != INF){
                    distSet.erase(distSet.find(make_pair(dist[v], v)));
                }
                dist[v] = dist[u] + weight;
                distSet.insert(make_pair(dist[v], v));
            }
        }
    }
    return dist[target];
}
//I use Prim's Algorithm here
//I also use some code provided in the slides.
long long int HelpStudents::secondStudent() {
    // IMPLEMENT ME!
    long long int maxEdge = 0;//max edge weight in the path
    typedef pair<long long int, long long int> pairs;
    priority_queue< pairs, vector <pairs> , greater<pairs> > pq;
    long long int source = 1;
    vector<long long int> parent(nodeNumber + 1, -1);
    vector<long long int> key(nodeNumber + 1, INF);
    vector<bool> inMST(nodeNumber + 1, false);
    pq.push(make_pair(0,source));
    key[source] = 0;
    while(!pq.empty()){
        long long int shortEdge = pq.top().second;
        pq.pop();
        inMST[shortEdge] = true;
        list< pair<int, int> >::iterator i;
        for (i = adjacencyList[shortEdge].begin(); i != adjacencyList[shortEdge].end(); ++i){
            int tmpShortEdge = (*i).first;
            long long int weight = (*i).second;
            if (inMST[tmpShortEdge] == false && key[tmpShortEdge] > weight){
                key[tmpShortEdge] = weight;
                pq.push(make_pair(key[tmpShortEdge], tmpShortEdge));
                parent[tmpShortEdge] = shortEdge;
            }
        }
    }
    //traverse all path and finds the biggest edge weight
    while(parent[target] != -1){
        if(key[target] > maxEdge){
            maxEdge = key[target];
        }
        target = parent[target];
    }
    return maxEdge;
}
//I use Dijkstra's Algorithm here again by changing the weight to one for each edge
long long int HelpStudents::thirdStudent() {
    // IMPLEMENT ME!
    set< pair<long long int, long long int>> distSet;
    vector<long long int> dist(nodeNumber+1, INF);
    distSet.insert(make_pair(0, 1));
    dist[1] = 0;
    while (!distSet.empty()) {
        pair<long long int, long long int> tmp = *(distSet.begin());
        distSet.erase(distSet.begin());
        int u = tmp.second;
        list< pair<int, int> >::iterator i;
        for (i = adjacencyList[u].begin()++; i != adjacencyList[u].end(); ++i) {
            int v = (*i).first;
            long long int weight = 1;
            if (dist[v] > dist[u] + weight) {
                if (dist[v] != INF){
                    distSet.erase(distSet.find(make_pair(dist[v], v)));
                }
                dist[v] = dist[u] + weight;
                distSet.insert(make_pair(dist[v], v));
            }
        }
    }
    return dist[target];
}

long long int HelpStudents::fourthStudent() {
    // IMPLEMENT ME!
    long long int min = INF;
    int minAdjEdge = 1;
    int tmpMinAdjEdge = 1;
    long long int distance = 0;
    list<pair <int,int> >::iterator i;
    list<pair <int,int> >::iterator j;//new edge iterator(tmpMinAdjEdge)
    list<pair <int,int> >::iterator k;//old edge iterator(minAdjEdge)
    //this loop finds the minimum adjacent edge in the adjacent vertices list
    while(minAdjEdge != target && adjacencyList[minAdjEdge].size() != 0){
        min = INF;
        for (i = adjacencyList[minAdjEdge].begin(); i != adjacencyList[minAdjEdge].end(); ++i) {
            if ((*i).second < min) {
                min = (*i).second;
                tmpMinAdjEdge = (*i).first;
                j = i;
            }
            else if((*i).second == min){
                if((*i).first < tmpMinAdjEdge){
                    min = (*i).second;
                    tmpMinAdjEdge = (*i).first;
                    j =i;
                }
            }
        }
        //this loop finds the current edge iterator(ı make this because ı need the pair format of them)
        for (i = adjacencyList[tmpMinAdjEdge].begin(); i != adjacencyList[tmpMinAdjEdge].end(); ++i){
            if((*i).first == minAdjEdge){
                k = i;
            }
        }
        distance += min;//updates total distance
        adjacencyList[minAdjEdge].erase(j);//erase processed adjacent vertice from adjacency list
        adjacencyList[tmpMinAdjEdge].erase(k);//erase processed adjacent vertice from adjacency list
        minAdjEdge = tmpMinAdjEdge;
    }
    if(minAdjEdge == target){
        return distance;
    }
    //if student cant reach the summit it prints -1
    else {
        return -1;
    }
}

long long int HelpStudents::fifthStudent() {
    // IMPLEMENT ME!
}

// YOU CAN ADD YOUR HELPER FUNCTIONS