#!/bin/bash

# This script builds and runs the Asteroids game for 5 seconds to test if it runs.
# It requires 'make', 'g++', and 'notcurses' library to be installed.

# Exit on error
set -e

echo "Building the project with make..."
# The 'all' target is the default, so 'make' is sufficient.
# make clean
make

echo "Running the game..."
# The C++ program is designed to run for 5 seconds and exit.
./asteroids "hello world"

echo "Test successful: Game ran and exited cleanly."
