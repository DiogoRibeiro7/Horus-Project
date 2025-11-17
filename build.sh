#!/bin/bash
# Horus Project - Build Script
# Author: João Oliveira (@joaoliveira6704)

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Print colored message
print_msg() {
    color=$1
    shift
    echo -e "${color}$@${NC}"
}

print_msg $BLUE "═══════════════════════════════════════════"
print_msg $BLUE "    Horus Project Build Script"
print_msg $BLUE "═══════════════════════════════════════════"
echo

# Parse arguments
BUILD_TYPE="Release"
CLEAN=false
VERBOSE=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Options:"
            echo "  -d, --debug     Build in Debug mode (default: Release)"
            echo "  -c, --clean     Clean build directory before building"
            echo "  -v, --verbose   Verbose output"
            echo "  -h, --help      Show this help message"
            exit 0
            ;;
        *)
            print_msg $RED "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Display configuration
print_msg $GREEN "Build Configuration:"
echo "  Build Type: $BUILD_TYPE"
echo "  Clean Build: $CLEAN"
echo "  Verbose: $VERBOSE"
echo

# Clean if requested
if [ "$CLEAN" = true ]; then
    print_msg $YELLOW "Cleaning build directory..."
    rm -rf build
    echo
fi

# Create build directory
print_msg $YELLOW "Creating build directory..."
mkdir -p build
cd build

# Check for Qt
print_msg $YELLOW "Checking for Qt..."
if command -v qmake6 &> /dev/null; then
    print_msg $GREEN "✓ Qt 6 found"
elif command -v qmake &> /dev/null; then
    print_msg $GREEN "✓ Qt found"
else
    print_msg $RED "✗ Qt not found!"
    print_msg $RED "Please install Qt 6.x or Qt 5.12+"
    exit 1
fi

# Configure with CMake
print_msg $YELLOW "Configuring with CMake..."
CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
if [ "$VERBOSE" = true ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_VERBOSE_MAKEFILE=ON"
fi

if ! cmake .. $CMAKE_ARGS; then
    print_msg $RED "✗ CMake configuration failed!"
    exit 1
fi
print_msg $GREEN "✓ CMake configuration successful"
echo

# Build
print_msg $YELLOW "Building Horus Project..."
MAKE_ARGS=""
if [ "$VERBOSE" = false ]; then
    MAKE_ARGS="--silent"
fi

# Determine number of cores for parallel build
if [[ "$OSTYPE" == "darwin"* ]]; then
    CORES=$(sysctl -n hw.ncpu)
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    CORES=$(nproc)
else
    CORES=2
fi

print_msg $BLUE "Building with $CORES parallel jobs..."

if ! make -j$CORES $MAKE_ARGS; then
    print_msg $RED "✗ Build failed!"
    exit 1
fi
print_msg $GREEN "✓ Build successful!"
echo

# Check if executable exists
if [[ "$OSTYPE" == "darwin"* ]]; then
    EXECUTABLE="Horus-Project.app/Contents/MacOS/Horus-Project"
else
    EXECUTABLE="Horus-Project"
fi

if [ -f "$EXECUTABLE" ]; then
    print_msg $GREEN "✓ Executable created: $EXECUTABLE"

    # Print file size
    SIZE=$(du -h "$EXECUTABLE" | cut -f1)
    print_msg $BLUE "  Size: $SIZE"
    echo

    # Print instructions
    print_msg $GREEN "═══════════════════════════════════════════"
    print_msg $GREEN "  Build Complete!"
    print_msg $GREEN "═══════════════════════════════════════════"
    echo
    print_msg $YELLOW "To run the application:"
    if [[ "$OSTYPE" == "darwin"* ]]; then
        echo "  open build/Horus-Project.app"
    else
        echo "  ./build/Horus-Project"
    fi
    echo
else
    print_msg $RED "✗ Executable not found!"
    exit 1
fi
