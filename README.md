# Terminal 3D Render Engine

A lightweight **3D render engine** that runs directly in the terminal.

> **Disclaimer:** The code is currently quite messy, and I plan to refactor it soon!


<figure>
  <img src="examples/rotating_cube.gif" alt="Rotating Cube Example">
  <figcaption><em>Rotating Cube Example</em></figcaption>
</figure>

---

<figure>
  <img src="examples/rotating_dual_polyhedron.gif" alt="Rotating Dual Polyhedron Example">
  <figcaption><em>Rotating Triangular Bipyramid Example</em></figcaption>
</figure>

---

## 🔧 Building & Running

This project uses **CMake** for cross-platform builds.

#### Build the project

```sh
cmake -B build -S .
```

#### Build the executable

```sh
cmake --build build
```

#### Build & Run

To build and run the project:

```sh
cmake --build build --target run
```

### 🐧 Linux

#### Build & Run

```sh
make -C build run
```

### 🪟 Windows

#### **Visual Studio**

[The first build command](#build-the-project) targets Visual Studio by default.
It generates a Visual Studio `.sln` project.

#### **MinGW (Alternative)**

To build with MinGW:

```powershell
cmake -B build -S . -G "MinGW Makefiles"
```

To build the executable:

```powershell
mingw32-make -C build
```

Run the engine:

```powershell
mingw32-make -C build run
```

Now you’re ready to experience 3D rendering in the terminal! 🚀

## Configuration

Configure the engine variables found in `/include/constants.h`:

```c
/*===============================================================================================*
 *
 * File: constants.h
 *
 * Description: Declares and defines different types of constants.
 *
 *===============================================================================================*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 64

#define PI 3.14159
#define FPS_LIMIT 0

#endif
```


