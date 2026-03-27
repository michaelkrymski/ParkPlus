import json
import csv
import math
from collections import defaultdict
from scipy.spatial import KDTree
import numpy as np

nodeDict = {}
nodeCoords = []

def haversine(lat1, lon1, lat2, lon2):
    R = 6371000
    dlat = math.radians(lat2 - lat1)
    dlon = math.radians(lon2 - lon1)
    a = math.sin(dlat/2)**2 + math.cos(math.radians(lat1)) * \
        math.cos(math.radians(lat2)) * math.sin(dlon/2)**2
    return R * 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

def nodeCheck(lat, lon):
    key = (round(lat, 5), round(lon, 5))
    if key not in nodeDict:
        nodeDict[key] = len(nodeCoords)
        nodeCoords.append(key)
    return nodeDict[key]

edges = []

with open("../data/InputCreation/roads.geojson", encoding="utf-8") as roadsFile:
    roads = json.load(roadsFile)

for i, feature in enumerate(roads["features"]):
    if i % 50000 == 0:
        print(f"Progress: {len(nodeDict)} roads processed.")
    geometry = feature["geometry"]
    if geometry["type"] != "LineString":
        continue
    coords = geometry["coordinates"]
    for j in range(len(coords) - 1):
        lon1, lat1 = coords[j]
        lon2, lat2 = coords[j+1]
        a = nodeCheck(lat1, lon1)
        b = nodeCheck(lat2, lon2)
        dist = haversine(lat1, lon1, lat2, lon2)
        edges.append((a, b, round(dist, 2)))

# Build KDTree for nearest neighbor search
kdtree = KDTree(np.array(nodeCoords))
parkingNodeIDs = []
parkingMeta = []

with open("../data/InputCreation/parking.geojson", encoding="utf-8") as parkingFile:
    reader = csv.DictReader(parkingFile)
    for row in reader:
        lat_lon = row["lat, long"].split(",")
        lat, lon = float(lat_lon[0]), float(lat_lon[1])
        if row["access"] in ("private", "no", "employees", "staff"):
            continue
        
        nodeID = nodeCheck(lat, lon)
        parkingNodeIDs.append(nodeID)

        d, index = kdtree.query([lat, lon])
        distance = haversine(lat, lon, *nodeCoords[index])
        edges.append((nodeID, index, round(distance, 2)))

        parkingMeta.append(
            {
                "node_id": nodeID,
                "lat": lat,
                "lon": lon,
                "name": row["name"],
                "type": row["parking"],
            }
        )

# Output all data to csvs.

with open("../data/nodes.csv", "w", newline="", encoding="utf-8") as nodesFile:
    writer = csv.writer(nodesFile)
    writer.writerow(["node_id", "lat", "lon"])
    for nodeID, (lat, lon) in enumerate(nodeCoords):
        writer.writerow([nodeID, lat, lon])

with open("../data/edges.csv", "w", newline="", encoding="utf-8") as edgesFile:
    writer = csv.writer(edgesFile)
    writer.writerow(["from_node", "to_node", "distance"])
    for fromNode, toNode, distance in edges:
        writer.writerow([fromNode, toNode, distance])

with open("../data/parking_nodes.csv", "w", newline="", encoding="utf-8") as parkingFile:
    writer = csv.writer(parkingFile)
    writer.writerow(["node_id", "lat", "lon", "name", "type"])
    for meta in parkingMeta:
        writer.writerow([meta["node_id"], meta["lat"], meta["lon"], meta["name"], meta["type"]])

# Stats
print(f"Total nodes: {len(nodeCoords)}")
print(f"Total edges: {len(edges)}")
print(f"Total parking nodes: {len(parkingNodeIDs)}")