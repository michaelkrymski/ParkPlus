/*
    ParkPlus - main.cpp
    Team: Michael Krymski, Gavin Calleja, Intevere

    This file contains the main functionality relating to searching and algorithms for finding the closest parking spots to the user's
    destination. Each teammate that has contributed to this file has their own section.

    Build:



*/

#include "Graph.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_set>

struct Result
{
    int node_id;
    float distance;
    double lat;
    double lon;
    std::string type;

    bool operator<(const Result &other) const
    {
        return distance < other.distance;
    }
};

/* Gabriel's Section: Djikstra's Algorithm Implementation

    Inputs: gavin's graph, the node closest to the user's destination,
    set of nodes specific to parking, user integer input for number of nodes to include in results.

    Outputs: vector<Result> sorted by distance using the operator< function.

*/

Graph &makeGraph(std::string node_list, std::string edge_list)
{
    Graph mainGraph{};
    mainGraph.makeGraph(node_list, edge_list);
    return mainGraph;
}

std::vector<Result>
dijkstra(Graph &graph, int userDestination,
         std::unordered_set<int> &parkingIds, int numResults)
{
}

/* Gavin's Section: A* Algorithm
    Inputs: gavin's graph, the node closest to the user's destination,
    set of nodes specific to parking, user integer input for number of nodes to include in results, input from python call of haversine heuristic distance.

    Outputs: vector<Result> sorted by distance using the operator< function.

*/

std::vector<Result> astar(Graph &graph, int userDestination, std::unordered_set<int> &parkingIds,
                          int numResults, double haversineDistance)
{
}
