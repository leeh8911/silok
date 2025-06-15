import os
import sys
import subprocess
from pathlib import Path
import argparse

def run_odb(input_header, odb_path="odb", db="sqlite", std="c++17"):
    cmd = [
        odb_path,
        "-d", db,
        "--std", std,
        "--generate-query",
        "--generate-schema",
        input_header
    ]
    print(f"[ODB] Running: {' '.join(cmd)}")
    subprocess.run(cmd, check=True)

def main():
    parser = argparse.ArgumentParser(description="Generate ODB code from C++ header.")
    parser.add_argument("header", help="Path to C++ header file (e.g., user.hpp)")
    parser.add_argument("--odb", default="odb", help="Path to ODB compiler (default: 'odb')")
    parser.add_argument("--db", default="sqlite", choices=["sqlite", "mysql", "pgsql"], help="Database backend")
    parser.add_argument("--std", default="c++20", help="C++ standard version")
    
    args = parser.parse_args()

    header_path = Path(args.header)
    if not header_path.exists():
        print(f"Error: header file '{header_path}' not found.")
        sys.exit(1)

    run_odb(str(header_path), args.odb, args.db, args.std)

if __name__ == "__main__":
    main()
