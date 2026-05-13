#include "../include/RBNode.h"

// Constructs a new RBNode.
// Parameters:
//   key → the encoded cell integer (x * numCols + y)
//   x   → original row index of the matrix cell (default -1 for NIL sentinel)
//   y   → original column index of the matrix cell (default -1 for NIL sentinel)
//
// All new nodes start as RED. The RBT insert algorithm requires this:
// any resulting RED-RED violation is fixed by insertFix().
RBNode::RBNode(long long key, int x, int y) {
    this->key = key;   // store the encoded cell key
    this->x = x;     // store original row
    this->y = y;     // store original column 
    color = RED;   // always start RED 
    left = right = parent = nullptr; // all pointers start null
}
