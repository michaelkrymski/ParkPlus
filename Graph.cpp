#include "Graph.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
void Graph::insertEdge(Node from, Node to, float weight) {
    adjacencyList[from].push_back({to,weight});
    adjacencyList[to].push_back({from, weight});
    edges++;
}
//
bool Graph::isEdge(int from, int to) {
    for(std::pair <Node, float> pair: adjacencyList[ node_vector.at(from) ] )
        if (pair.first == node_vector.at(to)) return true;
    return false;
}
void Graph::makeGraph(std::string node_list, std::string edge_list) {
    std::fstream node_file(node_list);
    if(!node_file.is_open()) {
        std::cerr << "failed to open" << std::endl;
        return;
    }
    std::string cuzzo;
    std::getline(node_file, cuzzo);
    while(std::getline(node_file,cuzzo, '\n')) {
        std::string id = cuzzo.substr(0, cuzzo.find(","));
        int firstComma = cuzzo.find(",");
        int secondComma = cuzzo.find(",", firstComma+1);
        std::string lat = cuzzo.substr(firstComma+1, secondComma-firstComma-1);
        std::string lawn = cuzzo.substr(secondComma+1);
        int id_int = std::stoi(id);
        double lat_double = std::stod(lat);
        double lawn_double = std::stod(lawn);
        Node temp{id_int,lat_double,lawn_double};
        // std::cout << lat<< " lat " << lat_float  << std::endlo
        addNode(temp);
    }

    node_file.close();  
    node_file.open(edge_list);
    std::getline(node_file, cuzzo);
    while(std::getline(node_file,cuzzo, '\n')) {
        std::string from_id = cuzzo.substr(0, cuzzo.find(","));
        int firstComma = cuzzo.find(",");
        int secondComma = cuzzo.find(",", firstComma+1);
        std::string to_id = cuzzo.substr(firstComma+1, secondComma-firstComma-1);
        std::string distance_m = cuzzo.substr(secondComma+1);
        int from_id_int = std::stoi(from_id);
        int to_id_int = std::stoi(to_id);
        float distance_m_float = std::stof(distance_m);
        insertEdge(node_vector.at(from_id_int),node_vector.at(to_id_int),distance_m_float);
    }
    node_file.close();
}


void Graph::printGraph() {

    for (auto iter = adjacencyList.begin(); iter != adjacencyList.end(); iter++) {
        std::cout << iter->first.id << " --> {";
        for(size_t i = 0; i < iter->second.size(); i ++ ) {
            std::cout << "( " << iter->second.at(i).first.id << ", " << iter->second.at(i).second << " )";
        }
        std::cout << "}" << std::endl;

    }
}


