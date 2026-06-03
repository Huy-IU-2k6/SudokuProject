
# 🧩 Sudoku AI Platform

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![SFML](https://img.shields.io/badge/SFML-3.1.0-green.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-red.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20macOS-lightgrey.svg)

A high-performance, cross-platform Sudoku application built with **C++17** and **SFML 3.1.0**. This project is not just a digital board game; it is a sophisticated **Constraint Satisfaction Problem (CSP)** engine featuring an AI Generator and an AI Solver.

## 📸 Screenshots
*(Drag and drop your gameplay screenshots here)*

---

## ✨ Key Features
* **Advanced AI Solver:** Utilizes *Randomized Backtracking*, *Minimum Remaining Values (MRV) Heuristics*, and *Bitwise Constraint Propagation* to solve complex puzzles in under 2 milliseconds.
* **Smart Generator:** Generates unique, solvable boards across 6 difficulty levels using *Arc Consistency 3 (AC-3)* and *Forward Checking*.
* **Modern GUI:** Built with SFML and a State Machine pattern. Features include Undo/Redo mechanisms, real-time mistake tracking, Hint system, and animations.
* **Zero-Friction Build System:** Uses CMake `FetchContent`. **No manual SFML installation is required!**
* **CI/CD Integrated:** Automated GitHub Actions pipeline builds release installers (`.exe`, `.dmg`) for Windows and macOS automatically.

---

## 🎮 How to Play & Controls
* **Mouse:** Click on any empty cell to select it. Click on the UI buttons to access menus, change difficulty, or request hints.
* **Keyboard (Numbers 1-9):** Input a number into the selected cell.
* **Backspace / Delete:** Clear the number from the selected cell.
* **Real-time Validation:** If you enter a number that violates Sudoku rules, it will be highlighted in red automatically.

---

## 🚀 How to Build and Run Locally

This project uses modern CMake practices. It will **automatically download and compile SFML 3.1.0** from the official GitHub repository during the build process. You do not need to install SFML manually on your machine.

### 📋 Prerequisites
Ensure your system has the following tools installed and added to your system `PATH`:
1. **[CMake](https://cmake.org/download/)** (v3.16 or higher)
2. **[Git](https://git-scm.com/downloads)** (Crucial: CMake needs Git to fetch the SFML dependency)
3. **A C++17 compatible compiler:**
   - **Windows:** MinGW-w64 (GCC) or MSVC (Visual Studio 2022)
   - **macOS:** Apple Clang (Xcode Command Line Tools)

### 🛠️ Build Instructions

**1. Clone the repository:**
```bash
git clone [https://github.com/Huy-IU-2k6/SudokuProject.git](https://github.com/Huy-IU-2k6/SudokuProject.git)
cd SudokuProject

```

**2. Configure the project using CMake:**

*For Windows (using MinGW):*

```bash
cmake -S . -B build -G "MinGW Makefiles" --fresh

```

*For macOS or Windows (using MSVC):*

```bash
cmake -S . -B build --fresh

```

**3. Build the executable:**

```bash
cmake --build build --config Release

```

*(Note: The initial build will take a few minutes as CMake will download the SFML source code and compile it locally).*

### 🎯 Running the Game

Once the build is complete, the executable and the required `assets/` folder will be placed in the `build` directory.

* **On Windows:**
Navigate to the `build` folder and double-click `SudokuProject.exe`, or run:
```bash
.\build\SudokuProject.exe

```


* **On macOS:**
Navigate to the `build` folder and open the App Bundle:
```bash
open build/SudokuProject.app

```



---

## 📦 Generating Installers (Optional)

If you want to generate a standalone setup wizard (Installer) to distribute the game to users without requiring them to build from source, you can use **CPack**.

* **For Windows:** Requires [NSIS](https://nsis.sourceforge.io/Download) installed.
* **For macOS:** No extra tools required (generates a `.dmg` drag-and-drop installer).

To generate the installer, run the following command after successfully building the project:

```bash
cd build
cpack -C Release

```

Check the `build` directory for your `SudokuProject-1.0.0-win64.exe` or `SudokuProject-1.0.0-Darwin.dmg` file!

---

## 🧠 Architecture & Algorithms Highlight

* **State Management:** Implemented a robust State Machine (`StateStack`) to seamlessly transition between Main Menu, Difficulty Selection, Playing State, and Game Over screens.
* **Time Optimization:** Constant time $O(1)$ constraint checking using 16-bit integers (`uint16_t`) and Bitwise operations (AND, OR, NOT).
* **Graph Algorithms:** Constraint graph propagation (AC-3) dramatically reduces the branching factor in the DFS tree to ensure near-instant solving time.
* **Cross-Platform Assets:** Uses CMake custom commands to automatically copy the `assets/` directory (fonts, images) to the build output, ensuring paths never break on different OS.

## 📄 License

This project was created for educational purposes. All visual assets and fonts belong to their respective creators.

```

```