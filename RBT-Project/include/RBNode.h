#pragma once

// Color enum represents the two possible colors of a Red-Black Tree node.
// Every node must be either RED or BLACK — this is a core RBT invariant.
enum Color { RED, BLACK };

class RBNode {
public:
    long long key;    // The encoded cell value: key = x * numCols + y
    // Using long long to safely handle large matrix sizes
    // without integer overflow (e.g., n=100000, m=100000)

// ── FIX: Added x and y fields ───────────────────────────────────────
// The GUI visualizer (Person 4) needs to display the original (x, y)
// coordinates on each node, not just the encoded key.
// Without these, P4 would have to reverse-engineer x and y from the key,
// which requires knowing 'm' (number of columns) at display time.
    int x, y;         // Original matrix coordinates this node represents
    // ────────────────────────────────────────────────────────────────────

    Color color;      // Current color of this node (RED or BLACK)
    // New nodes always start as RED (set in constructor)
    // and may be recolored during insertFix / deleteFix

    RBNode* left;     // Pointer to left child (smaller key)
    // Points to NIL sentinel when no left child exists
    RBNode* right;    // Pointer to right child (larger key)
    // Points to NIL sentinel when no right child exists
    RBNode* parent;   // Pointer to this node's parent
    // Points to NIL sentinel when this node is the root

// Constructor: initializes a new node with the given key.
// x and y default to -1 (used when creating the NIL sentinel node,
// which doesn't represent a real matrix cell).
    RBNode(long long key, int x = -1, int y = -1);
};
