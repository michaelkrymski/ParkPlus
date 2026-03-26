#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>
struct Node {
    int id;
    double latitude;
    double longitude;
    Node(int id_, double latitude_, double longitude_) 
        : id(id_), latitude(latitude_), longitude(longitude_) {}    
    
};  


class Graph {//Adjacencylist implementation
    private:
        std::unordered_map< int, std::vector<std::pair<int, float>> > adjacencyList;
        // i.e street1 -> { <street2, 10> <street3,11> <wallstreet, 12> }
        std::vector<Node> node_list;
        int edges;
        //graph data structure
    public:
        Graph() : adjacencyList(), node_list(), edges(0) {}       
        int numVertices() {return adjacencyList.size();}// returns number of Vertices
        int numEdges() {return edges;} //returns number of Edges
        void addNode(Node node_) {node_list.push_back(node_);}
        void insertEdge(int from, int to, float weight);
        bool isEdge(int from, int to);
        int getWeight(int from, int to);
        std::vector< std::pair<int, float> > getAdjacent(int from) { return adjacencyList[from];}
        void printGraph();
        void makeGraph();
};


