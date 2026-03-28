from flask import Flask, request, jsonify, send_from_directory
import subprocess
import requests
import csv
import numpy as np
from scipy.spatial import KDTree
import os

# Setup flask environment with name.
app = Flask(__name__)

# Load KDTree to get node closest to target destination. 
print("Loading all nodes to KDTree...")
nodeCoords = []
with open("../data/nodes.csv", encoding="utf-8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        nodeCoords.append((float(row["lat"]), float(row["lon"])))
tree = KDTree(np.array(nodeCoords))
print("Done!")

# Address input to coordinates function.
def getCoords(address):
    nom = requests.get("https://nominatim.openstreetmap.org/search", params={
        "q": address, "format": "json", "limit": 1
    }, headers={"User-Agent": "ParkPlus/1.0"})

    data = nom.json()
    print(f"Coordinates for {address}: {data[0]['lat']}, {data[0]['lon']}")
    return float(data[0]["lat"]), float(data[0]["lon"])

# Get results from cpp program.
def getResults(stdout):
    DJResults = []
    AStarResults = []

    temp = None
    for line in stdout.strip().split('\n'):
        if line[0] == "D":
            current = DJResults
        elif line[0] == "A":
            current = AStarResults
        else:
            parts = line.split(',')
            current.append({"id": parts[0], "distance": parts[1], "lat": parts[2], "lon": parts[3], "type": parts[4]})

    return DJResults, AStarResults

# Host webapp.

# Root Directory hosts index.html (/ defaults to index.html)
@app.route("/")
def index():
    return send_from_directory("../website", "index.html")

# Load css/js using a catch all (also does explicit /index.html lookup)
@app.route("/<path:filename>")
def static_files(filename):
    return send_from_directory("../website", filename)

# Search URL POST methods for data retrieval.
@app.route("/search", methods=["POST"])
def search():
    body = request.json # Get data from browser.

    address = body.get("address", "").strip() # Default no address -> fail.
    numResults = int(body.get("numResults", 10)) # Default 10 results.

    if not address:
        return jsonify({"error": "No address provided."}), 400 # Bad Request
    
    lat, lon = getCoords(address)
    if lat is None:
        return jsonify({"error": "Invalid or private address."}), 400 # Bad Request
    
    temp, index = tree.query([lat, lon])
    nearestRoadID = int(index)

    #debug
    print("Binary exists:", os.path.exists("../parkplus.exe"))
    print("Calling:", ["../parkplus.exe", str(nearestRoadID), str(numResults), str(lat), str(lon)])

    # Subprocess to interact with cpp program.
    result = subprocess.run(
        [ #Call parkplus with 4 input params.
            "../parkplus.exe",
            str(nearestRoadID),
            str(numResults),
            str(lat),
            str(lon)
        ],
        capture_output=True, #Grab stdout
        text=True #get stdout as string
    )

    print("Return code:", result.returncode)

    if result.returncode != 0:
        print("STDERR:", result.stderr)
        print("STDOUT:", result.stdout)
        return jsonify({"error": result.stderr}), 500

    if result.returncode != 0:
        return jsonify({"error": result.stderr}), 500 # Pull error from call.
    
    dj, astar = getResults(result.stdout)

    return jsonify(
        {
            "address":  address,
            "dijkstra": dj,
            "astar":    astar
        }
    )

# Safeguard running from imports from other files like flask API
if __name__ == "__main__":
    app.run()