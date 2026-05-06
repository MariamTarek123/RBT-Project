#include "RBNode.h"

// Constructs a new RBNode.
// Parameters:
//   key → the encoded cell integer (x * numCols + y)
//   x   → original row index of the matrix cell (default -1 for NIL sentinel)
//   y   → original column index of the matrix cell (default -1 for NIL sentinel)
//
// All new nodes start as RED. The RBT insert algorithm requires this:
// inserting RED preserves the black-height invariant automatically,
// and any resulting RED-RED violation is fixed by insertFix().
RBNode::RBNode(long long key, int x, int y) {
    this->key = key;   // store the encoded cell key
    this->x = x;     // store original row (for GUI display)
    this->y = y;     // store original column (for GUI display)
    color = RED;   // always start RED — insertFix will recolor as needed
    left = right = parent = nullptr; // all pointers start null;
    // the RBTree constructor will redirect
    // these to the NIL sentinel immediately
    // after creating the node
}
