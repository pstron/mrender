#!/bin/bash

set -e

BUILD_DIR="build"
EXECUTABLE="main"

show_help() {
    cat << EOF
Usage: $0 [OPTION]

Build Operations:
  -Sr         Build release
  -Sd         Build debug
  -Sdd        Build debug and start debugger (gdb)
  -S          Build and run to generate image (work for PPM mode; magick, chafa needed)
  -Sf         Build and run (work for FrameBuffer mode; sudo needed)

Runtime Operations:
  -D          Start debugger (gdb) with existing binary

Query Operations:
  -Q          Show build info

Maintenance Operations:
  -C          Clean build artifacts
  
EOF
}

query_info() {
    echo "==> Project Information"
    if [ -d "$BUILD_DIR" ]; then
        echo "Build directory: $BUILD_DIR"
        if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
            BUILD_TYPE=$(grep CMAKE_BUILD_TYPE:STRING "$BUILD_DIR/CMakeCache.txt" | cut -d'=' -f2)
            echo "Build type: ${BUILD_TYPE:-Unknown}"
            COMPILER=$(grep CMAKE_CXX_COMPILER:FILEPATH "$BUILD_DIR/CMakeCache.txt" | cut -d'=' -f2)
            echo "Compiler: ${COMPILER:-Unknown}"
        fi
        if [ -f "$BUILD_DIR/$EXECUTABLE" ]; then
            SIZE=$(du -h "$BUILD_DIR/$EXECUTABLE" | cut -f1)
            echo "Binary size: $SIZE"
            echo "Binary: $BUILD_DIR/$EXECUTABLE"
        else
            echo "Binary: Not built"
        fi
    else
        echo "Not configured yet"
    fi
}

clean_build() {
    echo "==> Cleaning build artifacts"
    rm -rf "$BUILD_DIR" compile_commands.json
    echo "==> Clean complete"
}

build_project() {
    local build_type=$1
    echo "==> Building ($build_type)"
    
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    cmake -GNinja \
        -DCMAKE_BUILD_TYPE="$build_type" \
        -DCMAKE_CXX_COMPILER=clang++ \
        ..
    
    ninja
    cd ..
    echo "==> Build complete"
}

run_binary() {
    if [ ! -f "$BUILD_DIR/$EXECUTABLE" ]; then
        echo "error: binary not found. Build first with -Sr or -Sd"
        exit 1
    fi
    echo "==> Running $EXECUTABLE"
    "./$BUILD_DIR/$EXECUTABLE"
}

run_binary_for_fb() {
    if [ ! -f "$BUILD_DIR/$EXECUTABLE" ]; then
        echo "error: binary not found. Build first with -Sr or -Sd"
        exit 1
    fi
    sudo "./$BUILD_DIR/$EXECUTABLE"
}

run_and_generate() {
    if [ ! -f "$BUILD_DIR/$EXECUTABLE" ]; then
        echo "error: binary not found. Build first with -Sr or -Sd"
        exit 1
    fi
    "./$BUILD_DIR/$EXECUTABLE" > out.ppm
    # magick out.ppm -filter point -scale 4000% out.png
    magick out.ppm out.png
    chafa out.png
}

debug_binary() {
    if [ ! -f "$BUILD_DIR/$EXECUTABLE" ]; then
        echo "error: binary not found. Build debug version with -Sd first"
        exit 1
    fi
    echo "==> Starting debugger"
    gdb "./$BUILD_DIR/$EXECUTABLE"
}

if [ $# -eq 0 ]; then
    show_help
    exit 0
fi

case "$1" in
    -Sr)
        build_project "Release"
        ;;
    -Sd)
        build_project "Debug"
        ;;
    -Sdd)
        build_project "Debug"
        debug_binary
        ;;
    -S)
        build_project "Release"
        run_and_generate
        ;;
    -Sf)
        build_project "Release"
        run_binary_for_fb
        ;;
    -D)
        debug_binary
        ;;
    -Q)
        query_info
        ;;
    -C)
        clean_build
        ;;
    -h|--help)
        show_help
        ;;
    *)
        echo "error: unknown option '$1'"
        echo "Try '$0 --help' for more information"
        exit 1
        ;;
esac
