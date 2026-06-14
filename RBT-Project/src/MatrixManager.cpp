#include "MatrixManager.h"

MatrixManager::MatrixManager(int n, int m)
    : TotalN(n), TotalM(m), T()
{}


long long MatrixManager::toKey(int x, int y) const
{
   
    return static_cast<long long>(x) * TotalM + y;
}

bool MatrixManager::inBounds(int x, int y) const
{
    return (x >= 0 && x < TotalN) && (y >= 0 && y < TotalM);
}

void MatrixManager::toggle(int x, int y)
{
  

    if (!inBounds(x, y))
    {
        return;
    }

    long long key = toKey(x, y);

    if (T.search(key))
    {
        T.remove(key);
    }
    else
    {
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