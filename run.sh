#!/bin/bash
# filepath: /home/gabechu/mdu/graphics/OpenGL/run.sh

set -e

# Compile the project
g++ -o graphics_app *.cpp -lGL -lGLU -lglut -lGLEW -lm -std=c++11

echo "Build complete. Running..."

# Run the compiled program
./graphics_app

echo "Application closed."