/*
    ParkPlus - main.cpp
    Team: Michael Krymski, Gavin Calleja, Gabriel Colen

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
#include <unordered_map>
#include <queue>

// Fallback PI definition for some compilers.
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Result struct to hold a singular result output of the algorithms.
struct Result
{
    int nodeID;
    float distance;
    double lat;
    double lon;
    std::string type;

    bool operator<(const Result &other) const
    {
        return distance < other.distance;
    }

    Result(int nodeID_, float distance_, double lat_, double lon_, std::string type_)
        : nodeID(nodeID_), distance(distance_), lat(lat_), lon(lon_), type(type_) {}
};

// Haversine formula to calculate direct distance between two points on the Earth's surface.
double haversine(double lat1, double lon1, double lat2, double lon2)
{
    const double R = 6371000.0;
    double dlat = (lat2 - lat1) * M_PI / 180.0;
    double dlon = (lon2 - lon1) * M_PI / 180.0;
    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
               sin(dlon / 2) * sin(dlon / 2);
    return R * 2 * atan2(sqrt(a), sqrt(1 - a));
}

// Load necessary data like parking node IDs and Meta.
// Input Notes: path is the only input, parkingIds and parkingMeta are passed in by reference for the Python Flask implementation.
void loadParkingData(const std::string &path, std::unordered_set<int> &parkingIds, std::unordered_map<int, std::string> &parkingMeta)
{
    std::ifstream parkingFile(path);
    if (!parkingFile.is_open())
    {
        std::cerr << "Failed to open parking data file." << std::endl;
        return;
    }

    std::string line;
    std::getline(parkingFile, line); // Skip header with column names.

    while (std::getline(parkingFile, line))
    {
        // Input format: id,lat,lon,name,type

        size_t commaPos = line.find(',');
        int id = std::stoi(line.substr(0, commaPos));

        size_t secondCommaPos = line.find(',', commaPos + 1);
        double lat = std::stod(line.substr(commaPos + 1, secondCommaPos - commaPos - 1));

        size_t thirdCommaPos = line.find(',', secondCommaPos + 1);
        double lon = std::stod(line.substr(secondCommaPos + 1, thirdCommaPos - secondCommaPos - 1));

        size_t fourthCommaPos = line.find(',', thirdCommaPos + 1);
        std::string name = line.substr(thirdCommaPos + 1, fourthCommaPos - thirdCommaPos - 1);

        size_t fifthCommaPos = line.find(',', fourthCommaPos + 1);
        std::string type = line.substr(fourthCommaPos + 1, fifthCommaPos - fourthCommaPos - 1);

        parkingIds.insert(id);
        parkingMeta[id] = name + " (" + type + ")";
    }
    parkingFile.close();
}

/* Gabriel's Section: Djikstra's Algorithm Implementation

    Inputs: gavin's graph, the node closest to the user's destination,
    set of nodes specific to parking, user integer input for number of nodes to include in results.

    Outputs: vector<Result> sorted by distance using the operator< function.

*/

std::vector<Result>
dijkstra(Graph &graph, int userDestination, std::unordered_set<int> &parkingIds, int numResults)
{
    std::vector<Result> results;

    std::priority_queue<
        std::pair<float, int>,
        std::vector<std::pair<float, int>>,
        std::greater<std::pair<float, int>>> pq;

    std::unordered_map<int, float> dist;

    std::unordered_set<int> addedToResults; //duplicates

    dist[userDestination] = 0.0f;
    pq.push({0.0f, userDestination});

    while (!pq.empty() && (int)results.size() < numResults)
    {
        float currentDist = pq.top().first;
        int currentID = pq.top().second;
        pq.pop();

        if (dist.find(currentID) != dist.end() && currentDist > dist[currentID])
            continue;

        Node currentNode = graph.getNode(currentID);

        if (parkingIds.find(currentID) != parkingIds.end() &&
            addedToResults.find(currentID) == addedToResults.end())
        {
            results.push_back(Result(
                currentID,
                currentDist,
                currentNode.latitude,
                currentNode.longitude,
                "Parking"));

            addedToResults.insert(currentID);

            if ((int)results.size() == numResults)
                break;
        }

        std::vector<std::pair<Node, float>> neighbors = graph.getAdjacent(currentNode);
        for (const auto &neighborPair : neighbors)
        {
            Node neighbor = neighborPair.first;
            float weight = neighborPair.second;
            float newDist = currentDist + weight;

            if (dist.find(neighbor.id) == dist.end() || newDist < dist[neighbor.id])
            {
                dist[neighbor.id] = newDist;
                pq.push({newDist, neighbor.id});
            }
        }
    }

    return results;
}

/* Gavin's Section: A* Algorithm
    Inputs: gavin's graph, the node closest to the user's destination, 
    set of nodes specific to parking, user integer input for number of nodes to include in results, 
    input of destination coordinates for per-node calculation.

    Outputs: vector<Result> sorted by distance using the operator< function.

*/

std::vector<Result> astar(Graph &graph, int userDestination, std::unordered_set<int> &parkingIds,
                          int numResults, double destLat, double destLon)
{
    //TODO
    std::vector<Result> results;
    return results;
}

int main(int argc, char *argv[])
{
    // Argument check.
    if(argc != 5)
    {
        std::cerr << "Needs 4 arguments. <originID> <numResults> <destinationLatitude> <destinationLongitude>" << std::endl;
        return 1;
    }

    int originID = std::stoi(argv[1]);
    int numResults = std::stoi(argv[2]);
    double destLat = std::stod(argv[3]);
    double destLon = std::stod(argv[4]);

    Graph graph{};
    graph.makeGraph("../data/nodes.csv", "../data/edges.csv");

    std::unordered_set<int> parkingIDs;
    std::unordered_map<int, std::string> parkingMeta;
    loadParkingData("../data/parking_nodes.csv", parkingIDs, parkingMeta);

    std::vector<Result> dijkstraResults = dijkstra(graph, originID, parkingIDs, numResults);
    std::vector<Result> astarResults = astar(graph, originID, parkingIDs, numResults, destLat, destLon);

    std::cout << "DJ" << std::endl;

    // node, dist, lat, lon, type

    for(const Result &res : dijkstraResults)
    {
        std::cout << res.nodeID << "," << res.distance << "," << res.lat << "," << res.lon << "," << res.type << std::endl;
    }
    std::cout << "A*" << std::endl;
    for(const Result &res : astarResults)
    {
        std::cout << res.nodeID << "," << res.distance << "," << res.lat << "," << res.lon << "," << res.type << std::endl;
    }

    return 0;
}
