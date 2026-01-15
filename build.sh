#!/bin/bash

# Build script for Time to BPM Calculator
# This script creates a build directory and compiles the application

set -e

echo "Building Time to BPM Calculator..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Navigate to build directory
cd build

# Run CMake to configure the project
echo "Configuring with CMake..."
cmake ..

# Build the project
echo "Compiling..."
make -j$(nproc)

echo "Build complete! Executable is at: build/bpmcalq"
echo "Use ./run.sh to run the application."