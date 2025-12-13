#!/bin/bash
# Build script for Smart Parking Management System

echo "========================================"
echo "Building Smart Parking Management System"
echo "========================================"
echo ""

cd "/c/Users/Suborna Asha/OneDrive/Desktop/Smart Parking Management System"

echo "Cleaning previous build..."
make clean

echo ""
echo "Building project..."
make

echo ""
if [ -f "smart_parking.exe" ]; then
    echo "========================================"
    echo "Build SUCCESSFUL!"
    echo "========================================"
    echo ""
    echo "Executable: smart_parking.exe"
    echo ""
    echo "To run: ./smart_parking.exe"
    echo ""
else
    echo "========================================"
    echo "Build FAILED"
    echo "========================================"
    echo "Check errors above"
    echo ""
fi

read -p "Press Enter to exit..."

