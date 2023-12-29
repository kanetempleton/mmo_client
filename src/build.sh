#!/bin/bash

# Specify the SFML installation directory
SFML_DIR="/opt/homebrew/Cellar/sfml/2.6.1"

# Compile each .cpp file into corresponding .o object files
for file in *.cpp; do
    echo "Compiling $file..."
    g++ -std=c++11 -c "$file" -o "${file%.cpp}.o" -I"${SFML_DIR}/include" -I"${SFML_DIR}/include/SFML"
done

# Link all object files into an executable named my_program
g++ *.o -o mmo_client -L"${SFML_DIR}/lib" -lsfml-graphics -lsfml-window -lsfml-system

# Run the executable
if [ $? -eq 0 ]; then
    echo "Build successful. Running my_program..."
    ./mmo_client
else
    echo "Build failed."
fi
