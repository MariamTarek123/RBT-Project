#pragma once
#include "RBNode.h"

// ─────────────────────────────────────────────────────────────────────
//  RBTree — Red-Black Tree
//
//  A self-balancing Binary Search Tree that guarantees O(log n) for
//  insert, delete, and search by enforcing these invariants after
//  every operation:
//    1. Every node is RED or BLACK.
//    2. The root is always BLACK.
//    3. Every NIL leaf is BLACK.
//    4. A RED node's children are always BLACK (no two reds in a row).
//    5. Every path from any node to its NIL descendants has the same
//       number of BLACK nodes (equal "black-height").
//
//  In this project, the key stored in each node is:
//      key = x * numCols + y
//  which uniquely identifies a (row, col) cell in the matrix.
// ─────────────────────────────────────────────────────────────────────
class RBTree {
private:
    RBNode* root;     
    RBNode* NIL;       
    
    int treeSize;      
    void leftRotate(RBNode* x);
    void rightRotate(RBNode* y);
    void insertFix(RBNode* z);
    void deleteFix(RBNode* x);
    RBNode* minimum(RBNode* node);
    void transplant(RBNode* u, RBNode* v);
    void freeNodes(RBNode* node);

public:
    RBTree();
    ~RBTree();
    void insert(long long key, int x, int y);
    void remove(long long key);
    bool search(long long key) const;  
    int size() const;                 
    RBNode* getRoot() const;
    RBNode* getNIL()  const;
};
