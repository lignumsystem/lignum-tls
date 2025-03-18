#!/usr/bin/env python3
# Usage: python3 fanni2lignum.py <input_file_or_folder> [/path/to/output_folder] [limit]

##------------------------------------------------
# Name: fanni2lignum.py
# Desc: Extracts leaf information from
#       fanni .obj files and converts to .csv
#       files for QSMReader.
# Author: H. O'Sullivan
# Date: Mar 2025
#
# Inputs:
#   input_folder : path/to/obj_folder (or file.obj)
#   output_folder: (Optional) path/to/output_folder
#   limit        : (Optional) int limit the no.
#                  files to be processed (-5)
# Outputs:
#   LIG_F*.csv     : .csv file(s) with leaf 
#                  information 
##------------------------------------------------

import math
import csv
import os
import glob
from tqdm import tqdm
import argparse

def parse_obj_into_leaves(filename):
    """
    Parse .obj and get leaf geometry.
    
    New leaves start after the string 'usemtl'.
    Each leaf is a dictionary with:
      - material
      - list of faces
      - the start index of leaf vertices 
      - the end index of leaf vertices

    Args:
        filename (str):  Path to .obj file to convert.

    Returns:
        tuple: Containing:
            - vertices (list): List of vertex coords (x, y, z).
            - leaves (list): List of leaf dictionaries
    """
    vertices = []  
    leaves = []    # additional leaf info
    current_leaf = None

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            # Start new leaf on 'usemtl' line
            if line.startswith('usemtl'):
                # Finish current leaf with its end index
                if current_leaf is not None:
                    current_leaf['end_index'] = len(vertices)
                    leaves.append(current_leaf)
                parts = line.split()
                material = parts[1] if len(parts) > 1 else ""
                current_leaf = {
                    'material': material,
                    'faces': [],
                    'start_index': len(vertices),
                    'end_index': None 
                }
                continue

            # Parse vertices
            if line.startswith('v '):
                parts = line.split()
                try:
                    x, y, z = map(float, parts[1:4])
                    vertices.append((x, y, z))
                except ValueError:
                    print("Error parsing vertex:", line)
                continue

            # Parse faces
            if line.startswith('f '):
                parts = line.split()
                face_indices = []
                for token in parts[1:]:
                    try:
                        idx = int(token)
                        abs_index = len(vertices) + idx
                        face_indices.append(abs_index)
                    except ValueError:
                        print("Error parsing face token:", token)
                if current_leaf is not None:
                    current_leaf['faces'].append(face_indices)
                else:
                    pass
                continue

        # Finish final leaf
        if current_leaf is not None:
            current_leaf['end_index'] = len(vertices)
            leaves.append(current_leaf)
            
        return vertices, leaves

def get_leaf_polygon(vertices, leaf):
    """
    Determine ordered polygon for a given leaf. The first leaf vertex
    is the one with the smallest angle (i.e. petiole). 
    1. Extract vertices from global vertex list 2. Calculate centroid 
    (only x and y coordinates). 3. Sort vertices by the angle around
    the centroid.

    Args:
        vertices (list): List of vertices (x, y, z).
        leaf (dict): Dictionary with 'faces' key. Each face is
            a list of vertex indices referencing the vertices list.

    Returns:
        leaf_vertices_sorted (list): 
            Ordered leaf vertices (3-tuples of floats).
    """
    vertex_indices = set()
    for face in leaf['faces']:
        for idx in face:
            vertex_indices.add(idx)

    # Get the vertex coords
    leaf_vertices = [vertices[i] for i in vertex_indices]

    # Compute centroid with x and y coords
    cx = sum(v[0] for v in leaf_vertices) / len(leaf_vertices)
    cy = sum(v[1] for v in leaf_vertices) / len(leaf_vertices)

    # Sort by angle (anti-clockwise)
    def angle(v):
        return math.atan2(v[1] - cy, v[0] - cx)
    leaf_vertices_sorted = sorted(leaf_vertices, key=angle)
    return leaf_vertices_sorted

def export_leaves_to_csv(vertices, leaves, csv_filename):
    """
    Each leaf is one row in a csv
    Each row contains:
      leaf_index, material, coordinates for each vertex i.e.:
      x1, y1, z1, x2, y2, z2, ...
    """
    # Get polygon for each leaf and max no. of vertices
    leaf_polygons = []
    max_vertices = 0
    for leaf in leaves:
        polygon = get_leaf_polygon(vertices, leaf)
        leaf_polygons.append(polygon)
        if len(polygon) > max_vertices:
            max_vertices = len(polygon)

    # Build header (leaf_index, x1,y1,z1, x2,y2,z2, etc..)
    header = ['leaf_index']
    for i in range(1, max_vertices + 1):
        header.extend([f'x{i}', f'y{i}', f'z{i}'])

    # Write csv
    with open(csv_filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(header)
        for i, (leaf, polygon) in enumerate(zip(leaves, leaf_polygons), start=1):
            row = [i]
            # Add x,y,z coords for each vertex
            for v in polygon:
                row.extend(v)  # tuple (x, y, z)
            # Pad if necessary
            missing = max_vertices - len(polygon)
            row.extend([''] * (missing * 3))
            writer.writerow(row)

# --- Main script ---
def main():

    # Set up cmd line arguments
    parser = argparse.ArgumentParser(
        description="Convert leaf .obj files to .csv for QSMReader."
    )
    parser.add_argument("input", help="Input .obj file or folder containing .obj files")
    parser.add_argument(
        "--output",
        "-o",
        default="../foliage_results",
        help="Output folder for .csv files (default: ../foliage_results)"
    )
    parser.add_argument(
        "--limit",
        "-l",
        type=int,
        default=None,
        help="Limit the number of files to be processed"
    )
    args = parser.parse_args()

    input_path = args.input
    output_path = args.output
    file_limit = args.limit

    # Make sure output folder exists
    if not os.path.exists(output_path):
        os.makedirs(output_path)

    # Get .obj files from the input path
    if os.path.isdir(input_path):
        obj_files = glob.glob(os.path.join(input_path, "*.obj"))
        if not obj_files:
            print(f"No .obj files found in {input_path}")
            return
    elif os.path.isfile(input_path):
        if not input_path.endswith(".obj"):
            print("Input file is not an .obj file.")
            return
        obj_files = [input_path]
    else:
        print("Invalid input! Please provide an .obj file or folder of .obj files.")
        return

    # Limit the no. of files with the file limit flag
    if file_limit is not None:
        obj_files = obj_files[:abs(file_limit)]

    print(f"Converting {len(obj_files)} .obj file{'s' if len(obj_files) != 1 else ''}...")

    # Set up progress bar, process files and export csv
    for obj_filename in tqdm(obj_files, desc="Processing files"):
        file_name_full = os.path.basename(obj_filename)
        file_name, _ = os.path.splitext(file_name_full)
        csv_filename = os.path.join(output_path, f"LIG_F_{file_name}.csv")

        vertices, leaves = parse_obj_into_leaves(obj_filename)
        export_leaves_to_csv(vertices, leaves, csv_filename)

    print("Done!")

if __name__ == "__main__":
    main()
