#pragma once
#include "RBTree.h"

class MatrixManager
{
private:
    //  Internal helpers
     // @brief Maps a 2D coordinate pair to a unique 64-bit key.
     //Formula:  key = (long long)x * TotalM + y
     /* Multiplying by TotalM(a runtime value) before adding y guarantees
      uniqueness for all valid (x, y) pairs and avoids 32-bit overflow
      when x or TotalM is large. */
      // x  Row index
      // y  Column index
      // long long  The unique key for this cell.

    long long toKey(int x, int y) const;

    // Returns true when (x, y) falls inside the matrix bounds.
    // x  Row index
    // y  Column index

    bool inBounds(int x, int y) const;

    //  Data members

    int    TotalN;     ///< Total number of rows
    int    TotalM;     ///< Total number of columns
    RBTree T;  ///< Underlying Red-Black Tree (stores only live cells)

public:
    //  Constructor
    // row × m - column matrix.
    // n > 0 && m > 0
    
    MatrixManager(int n, int m);

    //  Core operations
    // Toggles the cell at (x, y) between 0 and 1.
    // If the cell is currently 0, it becomes 1 (key inserted into tree).
    // If the cell is currently 1, it becomes 0 (key removed from tree).
    // Out-of-bounds coordinates are silently ignored.
    // x  Row index    (must satisfy 0 <= x < n)
    // y  Column index (must satisfy 0 <= y < m)
    
    void toggle(int x, int y);

     //  Returns the total number of cells currently set to 1.
     // Delegates directly to RBTree::size(), so the operation runs in O(1).
     // @return int  Count of active (live) cells.
     
    int getOnesCount() const;

     //  Helper / query
     // Checks whether the cell at (x, y) is currently 1.
     // x  Row index
     // y  Column index
     // @return true   if the cell is 1 (key present in tree)
     // @return false  if the cell is 0 OR the coordinates are out of bounds
    bool isCellOne(int x, int y) const;

    // Returns the number of rows in the matrix.
     
    int getRows() const;

    // Returns the number of columns in the matrix.
     
    int getCols() const;

    // Provides read-only access to the underlying RBTree.
    
    const RBTree& getTree() const;

};
