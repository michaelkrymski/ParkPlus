#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
struct Node
{
    int id;
    double latitude;
    double longitude;
    Node(int id_, double latitude_, double longitude_)
        : id(id_), latitude(latitude_), longitude(longitude_) {}
    Node()
    {
        id = -1;
        latitude = -1;
        longitude = -1;
    }
    bool operator==(const Node &other) const
    {
        return id == other.id;
    }
};
namespace std
{
    template <>
    struct hash<Node>
    {
        size_t operator()(const Node &n) const
        {
            return std::hash<int>()(n.id);
        }
    };
}

class Graph
{ // Adjacencylist implementation
private:
    std::unordered_map<Node, std::vector<std::pair<Node, float>>> adjacencyList;
    // i.e street1 -> { <street2, 10> <street3,11> <wallstreet, 12> }
    std::vector<Node> node_vector;
    int edges;
    // graph data structure
public:
    Graph() : adjacencyList(), node_vector(), edges(0) {}
    int numVertices() { return adjacencyList.size(); } // returns number of Vertices
    int numEdges() { return edges; }                   // returns number of Edges
    void addNode(Node node_)
    {

        int x = node_.id * 2 + 100;
        if ((size_t)node_.id >= node_vector.size())
            node_vector.resize(x);
        node_vector.at(node_.id) = node_;
    }

    void insertEdge(Node from, Node to, float weight)
    {
        adjacencyList[from].push_back({to, weight});
        adjacencyList[to].push_back({from, weight});
        edges++;
    }

    bool isEdge(int from, int to)
    {
        for (std::pair<Node, float> pair : adjacencyList[node_vector.at(from)])
            if (pair.first == node_vector.at(to))
                return true;
        return false;
    }

    std::vector<std::pair<Node, float>> getAdjacent(Node from) { return adjacencyList[from]; }

    void printGraph()
    {
        for (auto iter = adjacencyList.begin(); iter != adjacencyList.end(); iter++)
        {
            std::cout << iter->first.id << " --> {";
            for (size_t i = 0; i < iter->second.size(); i++)
            {
                std::cout << "( " << iter->second.at(i).first.id << ", " << iter->second.at(i).second << " )";
            }
            std::cout << "}" << std::endl;
        }
    }

    void makeGraph(std::string node_list, std::string edge_list)
    {
        std::fstream node_file(node_list);
        if (!node_file.is_open())
        {
            std::cerr << "failed to open" << std::endl;
            return;
        }
        std::string cuzzo;
        std::getline(node_file, cuzzo);
        while (std::getline(node_file, cuzzo, '\n'))
        {
            std::string id = cuzzo.substr(0, cuzzo.find(","));
            int firstComma = cuzzo.find(",");
            int secondComma = cuzzo.find(",", firstComma + 1);
            std::string lat = cuzzo.substr(firstComma + 1, secondComma - firstComma - 1);
            std::string lawn = cuzzo.substr(secondComma + 1);
            int id_int = std::stoi(id);
            double lat_double = std::stod(lat);
            double lawn_double = std::stod(lawn);
            Node temp{id_int, lat_double, lawn_double};
            // std::cout << lat<< " lat " << lat_float  << std::endl;
            addNode(temp);
        }

        node_file.close();
        node_file.open(edge_list);
        std::getline(node_file, cuzzo);
        while (std::getline(node_file, cuzzo, '\n'))
        {
            std::string from_id = cuzzo.substr(0, cuzzo.find(","));
            int firstComma = cuzzo.find(",");
            int secondComma = cuzzo.find(",", firstComma + 1);
            std::string to_id = cuzzo.substr(firstComma + 1, secondComma - firstComma - 1);
            std::string distance_m = cuzzo.substr(secondComma + 1);
            int from_id_int = std::stoi(from_id);
            int to_id_int = std::stoi(to_id);
            float distance_m_float = std::stof(distance_m);
            insertEdge(node_vector.at(from_id_int), node_vector.at(to_id_int), distance_m_float);
        }
        node_file.close();
    }
};
