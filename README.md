# mrender

[![GitHub License](https://img.shields.io/github/license/pstron/mrender)](LICENSE)

**m**inimal, **m**odern C++ renderer

mrender is an educational ray tracing renderer built from scratch using modern C++23 features. It's designed to be minimal, understandable, and showcases contemporary C++ practices with modules.

The project is working in progress now.

## ✨ Features

- **Modern C++23**: Full embrace of C++ modules and latest standards
- **Minimal implementation**: Everything written from first principles
- **Dual output**: Support for PPM images and direct framebuffer display
- **Clean architecture**: Modular design with clear separation of concerns
- **Educational focus**: Code is intentionally simple and well-documented

## 📋 Requirements

### Toolchain
- **CMake 3.30+**
- **Ninja 1.12.0+**
- **Clang 18.0+** (with libc++)

### Optional Tools
- **ImageMagick**: For PPM to PNG conversion (`magick convert`)
- **Chafa**: Terminal image viewer (`chafa`)
- **GDB**: Debugging support

## 🚀 Quick Start

### Build and Run
```bash
# Install required packages
sudo pacman -S cmake ninja clang imagemagick chafa

# Clone the repository
git clone https://github.com/yourusername/mrender.git
cd mrender

# Build and run to generate image (work for PPM mode; magick, chafa needed)
./build.sh -S

# Build and run (work for FrameBuffer mode; sudo needed)
./ build.sh -Sf

# Build debug version
./build.sh -Sd

# Build debug and start debugger
./build.sh -Sdd

# Clean build artifacts
./build.sh -C
```

## 🔧 Build Details

### CMake Configuration
The project uses advanced CMake features for C++23 module support:
- Automatic module scanning (`CMAKE_CXX_SCAN_FOR_MODULES`)
- Precompiled standard library modules
- Clang with libc++ toolchain

### Build Script Features
The provided `build.sh` offers:
- Multiple build types (Release/Debug)
- Debugger integration (GDB)
- Project information queries
- Clean build automation

## 🎯 Design Principles

1. **Simplicity First**: Prioritize readability and learning over optimization
2. **Modern C++**: Full C++23 compliance with module system
3. **Zero Black Boxes**: Every component implemented from first principles
4. **Clear Abstractions**: Single responsibility per module

## 📖 Learning Objectives

This project demonstrates:
- Ray tracing fundamentals and mathematics
- Modern C++ module system in practice
- 3D graphics programming concepts
- Build system configuration for C++23
- Software design patterns in graphics

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

---

**Built for learning, designed for clarity, implemented with modern C++.**

*Experience ray tracing from first principles with the latest C++ features.*
