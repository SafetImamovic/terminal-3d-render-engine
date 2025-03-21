# Terminal 3D Render Engine

A lightweight **3D render engine** that runs directly in the terminal.

## 🔧 Building

This project uses **CMake** for cross-platform builds.

### 🐧 Linux

```sh
cmake -B build -S .
cmake --build build
```

### 🪟 Windows

#### **Visual Studio (Default)**

```powershell
cmake -B build -S .
```
Generates a Visual Studio `.sln` project.

#### **MinGW (Alternative)**

```powershell
cmake -B build -S . -G "MinGW Makefiles"
mingw32-make -C build
```

Now you’re ready to run the engine! 🚀
