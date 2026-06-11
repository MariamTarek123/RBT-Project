#include "MatrixManager.h"

MatrixManager::MatrixManager(int n, int m)
    : TotalN(n), TotalM(m), T()
{
    // tree_ is default-constructed by RBTree::RBTree().
    // TotalN and TotalM are stored for bounds-checking and key generation.
}


long long MatrixManager::toKey(int x, int y) const
{
    // Cast x to long long *before* the multiplication so that the
    // intermediate product never wraps around in 32-bit arithmetic.
    //
    //   key = x * TotalM + y
    //
    // Because 0 <= y < TotalM, no two distinct (x, y) pairs can share
    // the same key, making the mapping a true bijection over the
    // valid coordinate space.
    return static_cast<long long>(x) * TotalM + y;
}

bool MatrixManager::inBounds(int x, int y) const
{
    return (x >= 0 && x < TotalN) && (y >= 0 && y < TotalM);
}

void MatrixManager::toggle(int x, int y)
{
  

    // 1. Validate coordinates — silently ignore invalid input.
    if (!inBounds(x, y))
    {
        return;
    }

    //  2.Compute the unique 1D key for this cell.
    long long key = toKey(x, y);

    //  3.Search the tree to decide which direction to toggle.
    if (T.search(key))
    {
        // Cell is currently 1 → remove the key to set it back to 0.
        T.remove(key);
    }
    else
    {
        // Cell is currently 0 → insert the key to set it to 1.
        // We pass the original (x, y) coordinates so tree
        // node can store positional metadata if needed.
        T.insert(key, x, y);
    }
}

int MatrixManager::getOnesCount() const
{
    // RBTree::size() maintains a running counter, so this is O(1).
    return T.size();
}


bool MatrixManager::isCellOne(int x, int y) const
{
    // Out-of-bounds cells are treated as permanently 0.
    if (!inBounds(x, y))
    {
        return false;
    }

    return T.search(toKey(x, y));
}

int MatrixManager::getRows() const
{
    return TotalN;
}

int MatrixManager::getCols() const
{
    return TotalM;
}

const RBTree& MatrixManager::getTree() const
{
    return T;
}