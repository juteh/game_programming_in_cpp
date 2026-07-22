# Game Programming in C++

My personal progress working through the book ["Game Programming in C++" by Sanjay Madhav](https://www.gameprogcpp.com/), chapter by chapter.


## Requirements

- **CMake**
- **Visual Studio 2022** (with the "Desktop development with C++" workload) for the MSVC compiler
- **Git**
- An IDE with CMake support (I use JetBrains Rider)

## Setup (Windows)

### 1. Install vcpkg

`vcpkg` is used to install SDL2. It is intentionally **not** part of this repo and needs to be cloned locally:

```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

### 2. Install SDL2

```powershell
.\vcpkg install sdl2:x64-windows
```

### 3. Configure the project

When configuring a chapter folder (e.g. `chapter_1`), CMake needs to be given the path to the vcpkg toolchain file:

```
-DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake
```

**In Rider:** Under *Settings → Build, Execution, Deployment → CMake*, in the "CMake options" field.

**Via command line:**
```powershell
cmake -S chapter_1 -B chapter_1/cmake-build-debug -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build chapter_1/cmake-build-debug
```

## Chapter Overview

### Chapter 1 Pinball
Use SDL as a framework for implementing a 2D pinball game with a simple game loop using `ProcessInput`, `UpdateGame`, and `GenerateOutput`.

