#include "Graph.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
int main() {
    Graph bro{};
    std::string edge_list = "edges_test.csv";    
    std::string node_list = "nodes_test.csv";
    bro.makeGraph(node_list,edge_list);    
    std::cout << (bro.isEdge(1016,1017)) << std::endl;
    bro.printGraph();
   return 0;
}
