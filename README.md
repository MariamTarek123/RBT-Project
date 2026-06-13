# Sparse Matrix Red-Black Tree Visualizer

A premium, interactive desktop GUI application built with C++ and Qt Widgets (supporting Qt 5 and Qt 6). It visualizes a sparse matrix backed by a self-balancing Red-Black Tree data structure.

The interface consists of a dynamic grid on the left and a live-updating binary tree visualizer on the right.

---

## Getting Started

### Prerequisites
*   A C++17 compliant compiler (GCC, Clang, or MSVC)
*   **Qt 5** or **Qt 6** installed
*   **CMake** (minimum version 3.16)
*   **Qt Creator** (recommended IDE)

### Build Instructions
*   **Build**: open CMakeLists.txt in Qt Creator, configure, and build.
    *(Alternatively, you can run `cmake -B build` followed by `cmake --build build` from a terminal in this directory)*

### Run Instructions
*   **Run**: click Create after entering rows and cols, click cells to toggle, watch Ones count update, and view the RBTree on the right.

---

## Features and Usage Guide

1.  **Configure Matrix Dimensions**: 
    Use the `Rows` and `Cols` inputs at the top to set the grid dimensions. Click **Create** to (re)initialize a fresh empty matrix.
2.  **Toggle Cells**: 
    Clicking any cell in the left grid toggles it:
    *   **0 (Inactive)**: Rendered as a clean white cell.
    *   **1 (Active)**: Rendered as a sea green cell (`#2E8B57`).
3.  **Real-Time Statistics**: 
    The **Ones Count** label updates immediately, displaying the number of elements with a value of `1`. The bottom status bar shows the coordinates of the last clicked cell and its state.
4.  **Red-Black Tree Visualization**: 
    The right pane draws the Red-Black Tree representation of the matrix keys:
    *   Red nodes are colored `#D9534F`.
    *   Black nodes are colored `#222222`.
    *   Node labels show the original `x,y` coordinates.
    *   Lines connect parent and child nodes showing structural relationships.
5.  **Interactive Navigation & Zoom**:
    *   Zoom in or out on the tree using the **+** and **-** buttons at the top right, or by using your **mouse scroll wheel** over the tree pane. Click **Reset** to return to the default zoom.
    *   Click and drag on the tree canvas to pan around if the tree is large.
6.  **Clear Matrix**: 
    Clicking the **Clear** button resets all cells in the matrix back to `0` and clears the tree.

---

## Code Architecture

*   **`CMakeLists.txt`**: Standard CMake configuration file that dynamically links the Qt library and builds the project executable.
*   **`main.cpp`**: Application entry point that boots `QApplication` and the main window.
*   **`MainWindow`**: Core UI class managing layout splitting, control event signals, status bar feedback, and reinitialization of the matrix.
*   **`GridWidget`**: Custom-drawn, ultra-fast grid component that avoids widget overhead. Renders 200x200 grids efficiently.
*   **`TreeView`**: `QGraphicsView` subclass that displays the tree structure. It implements an **in-order layout** guaranteeing nodes and lines never overlap.
