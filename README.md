# ParkPlus — Data Pipeline

## Data Source

All map data comes from [Geofabrik](https://download.geofabrik.de/north-america/us/florida.html), which packages [OpenStreetMap](https://www.openstreetmap.org/) data by region. Download the Florida extract:

```
florida-latest.osm.pbf
```

Place it anywhere accessible — you will pass its path as an argument to the generation scripts.

---

## Requirements

**osmium-tool**
- Mac: `brew install osmium-tool`
- Windows: Download from https://osmcode.org/osmium-tool/

**Python dependencies**
```bash
pip install -r dependencies.txt
```

---

## Generating the Data

Run the appropriate script from the **project root** (`ParkPlus/`), passing the path to your `.pbf` file as the argument.

**Mac/Linux:**
```bash
chmod +x generate_data.sh
./generate_data.sh path/to/florida-latest.osm.pbf
```

**Windows:**
```cmd
generate_data.bat path\to\florida-latest.osm.pbf
```

This runs the full pipeline end to end and produces all required output files.

---

## What the Scripts Do

### Step 1 — Filter parking features
```bash
osmium tags-filter florida.osm.pbf "amenity=parking" -o data/InputCreation/parking.osm.pbf
```
Extracts only features tagged `amenity=parking` from the full OSM extract.

### Step 2 — Export parking to GeoJSON
```bash
osmium export data/InputCreation/parking.osm.pbf -o data/InputCreation/parking.geojson
```
Converts the filtered parking data to GeoJSON for Python to read.

### Step 3 — Filter road network
```bash
osmium tags-filter florida.osm.pbf "highway=*" -o data/InputCreation/roads.osm.pbf
```
Extracts all road features from the full OSM extract.

### Step 4 — Export roads to GeoJSON
```bash
osmium export data/InputCreation/roads.osm.pbf --geometry-types=linestring -o data/InputCreation/roads.geojson
```
Converts road data to GeoJSON LineStrings for Python to read.

### Step 5 — Run Python pipeline
```bash
cd python_datahandling
python3 generate_data.py
```

## Output Files

All final output files are written to `data/`:

| File | Description | Used by |
|---|---|---|
| `nodes.csv` | All graph nodes with `node_id`, `lat`, `lon` | C++ `makeGraph`, Flask KDTree |
| `edges.csv` | All edges with `from_node`, `to_node`, `distance_m` | C++ `makeGraph` |
| `parking_nodes.csv` | Parking-only nodes with `node_id`, `lat`, `lon`, `name`, `type` | C++ `loadParkingData` |
| `data/InputCreation/parking_lots.csv` | Intermediate parking features from OSM | Python pipeline only |

---

## Project Structure

```
ParkPlus/
├── generate_data.bat          # Windows pipeline runner
├── generate_data.sh           # Mac/Linux pipeline runner
├── parkplus                   # Compiled C++ binary
├── data/
│   ├── nodes.csv
│   ├── edges.csv
│   ├── parking_nodes.csv
│   └── InputCreation/         # Intermediate osmium outputs
├── cpp_processing/
│   ├── Graph.hpp
│   └── main.cpp
├── python_datahandling/
│   ├── generate_data.py
│   └── dependencies.txt       #Backup specific to data generation.
├── python_frontend/
│   └── app.py
|   └── dependencies.txt       #Backup specific to frontend.
└── website/
    ├── index.html
    └── main.css
```

---

## Building the C++ Binary

**Mac/Linux:**
```bash
cd cpp_processing
g++ -std=c++17 main.cpp -o ../parkplus
chmod +x ../parkplus
```

**Windows (MSYS2/MinGW):**
```bash
cd cpp_processing
g++ -std=c++17 -static main.cpp -o ../parkplus.exe
```

---

## Running the Web App

```bash
cd python_frontend
python3 app.py
```

Open `http://127.0.0.1:5000` in your browser.