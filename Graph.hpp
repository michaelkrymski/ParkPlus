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
    Node() {id = -1; latitude = -1; longitude= -1;}
    bool operator==(const Node& other) const {
        return id == other.id;
    }
};  
namespace std {
    template <>
    struct hash<Node> {
        size_t operator()(const Node& n) const {
            return std::hash<int>()(n.id);
        }
    };
}

class Graph {//Adjacencylist implementation
    private:
        std::unordered_map< Node, std::vector<std::pair<Node, float>> > adjacencyList;
        // i.e street1 -> { <street2, 10> <street3,11> <wallstreet, 12> }
        std::vector<Node> node_vector;
        int edges;
        //graph data structure
    public:
        Graph() : adjacencyList(), node_vector(), edges(0) {}       
        int numVertices() {return adjacencyList.size();}// returns number of Vertices
        int numEdges() {return edges;} //returns number of Edges
        void addNode(Node node_) {

            int x = node_.id*2 + 100;
            if((size_t)node_.id >= node_vector.size()) node_vector.resize(x);
            node_vector.at(node_.id) = node_;
        }
        
        void insertEdge(Node from, Node to, float weight);
        bool isEdge(int from, int to);
        std::vector< std::pair<Node, float> > getAdjacent(Node from) { return adjacencyList[from];}
        void printGraph();
        void makeGraph(std::string node_list, std::string edge_list);
};


