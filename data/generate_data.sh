#!/bin/bash
osmium tags-filter "$1" "amenity=parking" -o InputCreation/parking.osm.pbf --overwrite
osmium export InputCreation/parking.osm.pbf -o InputCreation/parking.geojson --overwrite
osmium tags-filter "$1" "highway=*" -o InputCreation/roads.osm.pbf --overwrite
osmium export InputCreation/roads.osm.pbf --geometry-types=linestring -o InputCreation/roads.geojson --overwrite
cd python_datahandling
python3 generate_data.py
cd ..