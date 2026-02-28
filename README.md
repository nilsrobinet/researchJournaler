# resJourn - Qt Application

A simple Qt6 application displaying a "Hello World" window.

## Prerequisites

- Qt6 development libraries
- CMake 3.21 or higher
- C++17 compatible compiler

### Installation on Ubuntu/Debian

```bash
sudo apt-get install qt6-base-dev cmake build-essential
```

### Installation on macOS

```bash
brew install qt cmake
```

### Installation on Windows

Download Qt6 from [qt.io](https://www.qt.io/download) or use vcpkg:
```bash
vcpkg install qt6:x64-windows
```

## Building

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the application
cmake --build .
```

## Running

```bash
./resJourn
```

Or from the build directory:
```bash
cd build
./resJourn
```
