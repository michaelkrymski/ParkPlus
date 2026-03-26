#include "Graph.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
int main() {
    Graph bro{};
    bro.makeGraph();    
    std::cout << (bro.isEdge(1016,1017)) << std::endl;
    bro.printGraph();
   return 0;
}
