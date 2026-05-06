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
    RBNode* root;      // The topmost node of the tree (or NIL if empty)
    RBNode* NIL;       // Sentinel leaf node — all "null" child/parent pointers
    // point here instead of nullptr. This simplifies
    // rotations and fix-ups by removing nullptr checks.
    int treeSize;      // Number of real (non-NIL) nodes currently in the tree

    // ── Private helpers (called internally, not exposed to the rest of team) ──

    // leftRotate(x): pivots the subtree rooted at x to the left.
    // Used during insertFix and deleteFix to restructure the tree
    // without breaking the BST ordering property.
    void leftRotate(RBNode* x);

    // rightRotate(y): pivots the subtree rooted at y to the right.
    // Mirror operation of leftRotate.
    void rightRotate(RBNode* y);

    // insertFix(z): called after a BST insert to restore RBT invariants.
    // Handles 3 cases (and their left/right mirrors) by recoloring
    // and rotating until no RED-RED violation remains.
    void insertFix(RBNode* z);

    // deleteFix(x): called after a BST delete to restore RBT invariants.
    // Handles 4 cases (and their left/right mirrors) when a BLACK node
    // was removed, which may have broken the equal-black-height invariant.
    void deleteFix(RBNode* x);

    // minimum(node): returns the leftmost (smallest key) node in the
    // subtree rooted at 'node'. Used by remove() to find the in-order
    // successor when deleting a node with two children.
    RBNode* minimum(RBNode* node);

    // transplant(u, v): replaces the subtree rooted at u with the subtree
    // rooted at v. Used by remove() to splice out the deleted node cleanly
    // by rewiring parent pointers without touching the keys.
    void transplant(RBNode* u, RBNode* v);

public:
    // Constructor: initializes the NIL sentinel and an empty tree.
    RBTree();

    // ── FIX: Added destructor ────────────────────────────────────────
    // Without this, every RBNode allocated with 'new' is never freed,
    // causing a memory leak for the entire lifetime of the program.
    ~RBTree();
    // ─────────────────────────────────────────────────────────────────

    // insert(key, x, y): inserts a new node with the given encoded key
    // and original coordinates. If the key already exists (duplicate cell),
    // the insert is silently ignored — this is the core problem behavior.
    void insert(long long key, int x, int y);

    // remove(key): deletes the node with the given key if it exists.
    // Silently does nothing if the key is not found.
    void remove(long long key);

    // search(key): returns true if the key exists in the tree, false otherwise.
    // Marked const because it does not modify the tree in any way.
    bool search(long long key) const;  // ← FIX: added const

    // size(): returns the number of real nodes currently stored.
    // Marked const because it only reads treeSize.
    int size() const;                  // ← FIX: added const

    // ── FIX: Added getRoot() ─────────────────────────────────────────
    // Person 4 (GUI tree visualizer) needs access to the root node
    // to traverse the tree and draw it on screen. Without this method,
    // the internal tree structure is completely inaccessible from outside.
    RBNode* getRoot() const;

    // getNIL(): exposes the NIL sentinel so the GUI can check
    // whether a child pointer is a real node or a leaf boundary.
    RBNode* getNIL()  const;
    // ─────────────────────────────────────────────────────────────────
};
