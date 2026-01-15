#!/bin/bash

# Run script for Time to BPM Calculator
# This script runs the compiled application

set -e

# Check if the executable exists
if [ ! -f "build/bpmcalq" ]; then
    echo "Executable not found! Please run ./build.sh first."
    exit 1
fi

echo "Starting Time to BPM Calculator..."
echo "Keyboard shortcuts:"
echo "  F1: Focus Time Signature"
echo "  F2: Focus Number of Bars"
echo "  F3: Focus Time Entry"
echo "  F4: Focus Browse Button"
echo "  F5: Focus Calculate Button"
echo "  Enter: Calculate BPM"
echo "  Escape: Exit"
echo ""

# Run the application
./build/bpmcalq