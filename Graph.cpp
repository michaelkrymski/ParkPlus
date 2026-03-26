#include "Graph.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <utility>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
void Graph::insertEdge(int from, int to, float weight) {
    adjacencyList[from].push_back({to,weight});
    edges++;
}

bool Graph::isEdge(int from, int to) {
    for(std::pair <int, float> pair: adjacencyList[from]) 
        if (pair.first == to) return true;
    return false;
}
void Graph::makeGraph() {
    std::fstream node_file("nodes_test.csv");
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
    node_file.open("edges_test.csv");
    std::getline(node_file, cuzzo);
    while(std::getline(node_file,cuzzo, '\n')) {
        std::string from_id = cuzzo.substr(0, cuzzo.find(","));
        int firstComma = cuzzo.find(",");
        int secondComma = cuzzo.find(",", firstComma+1);
        std::string to_id = cuzzo.substr(firstComma+1, secondComma-firstComma-1);
        std::string distance_m = cuzzo.substr(secondComma+1);
        int from_id_int = std::stoi(from_id);
        int to_id_int = std::stod(to_id);
        float distance_m_float = std::stof(distance_m);
        insertEdge(from_id_int,to_id_int,distance_m_float);
    }
    node_file.close();
}


void Graph::printGraph() {

    for (auto iter = adjacencyList.begin(); iter != adjacencyList.end(); iter++) {
        std::cout << iter->first << " --> {";
        for(size_t i = 0; i < iter->second.size(); i ++ ) {
            std::cout << "( " << iter->second.at(i).first << ", " << iter->second.at(i).second << " )";
        }
        std::cout << "}" << std::endl;

    }
}


