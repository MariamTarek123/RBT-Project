#include "RBTree.h"

// ─── Constructor ────────────────────────────────────────────────────────────

RBTree::RBTree() {
    NIL = new RBNode(0);  // Sentinel — represents all leaf boundaries
    NIL->color = BLACK;
    NIL->left = NIL;
    NIL->right = NIL;
    NIL->parent = NIL;
    root = NIL;
    treeSize = 0;
}

// ─── Destructor ─────────────────────────────────────────────────────────────

void RBTree::freeNodes(RBNode* node) {
    if (node == NIL) return;
    freeNodes(node->left);
    freeNodes(node->right);
    delete node;
}

RBTree::~RBTree() {
    freeNodes(root);
    delete NIL;
}

// ─── Rotations ──────────────────────────────────────────────────────────────

/*
 * leftRotate(x): pivots subtree at x to the left.
 *
 *      x                y
 *     / \              / \
 *    A   y    →       x   C
 *       / \          / \
 *      B   C        A   B
 */
void RBTree::leftRotate(RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)          root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else                           x->parent->right = y;

    y->left = x;
    x->parent = y;
}

/*
 * rightRotate(y): mirror of leftRotate.
 *
 *      y                x
 *     / \              / \
 *    x   C    →       A   y
 *   / \                  / \
 *  A   B                B   C
 */
void RBTree::rightRotate(RBNode* y) {
    RBNode* x = y->left;
    y->left = x->right;

    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NIL)           root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else                            y->parent->left = x;

    x->right = y;
    y->parent = x;
}

// ─── Insert ─────────────────────────────────────────────────────────────────

void RBTree::insert(long long key, int x, int y) {
    RBNode* parent = NIL;
    RBNode* current = root;

    // Standard BST descent
    while (current != NIL) {
        parent = current;
        if (key < current->key) current = current->left;
        else if (key > current->key) current = current->right;
        else return;  // Duplicate — ignore
    }

    RBNode* z = new RBNode(key, x, y);
    z->left = NIL;
    z->right = NIL;
    z->parent = parent;

    if (parent == NIL)      root = z;
    else if (key < parent->key)  parent->left = z;
    else                         parent->right = z;

    insertFix(z);
    treeSize++;
}

/*
 * insertFix: restores RBT invariants after insert.
 *
 * Case 1 — Uncle is RED:        recolor parent, uncle, grandparent; move z up.
 * Case 2 — Uncle BLACK, inner:  rotate parent to convert to Case 3.
 * Case 3 — Uncle BLACK, outer:  rotate grandparent, recolor.
 */
void RBTree::insertFix(RBNode* z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode* uncle = z->parent->parent->right;
            if (uncle->color == RED) {
                // Case 1
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    // Case 2 → convert to Case 3
                    z = z->parent;
                    leftRotate(z);
                }
                // Case 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        }
        else {
            // Mirror cases
            RBNode* uncle = z->parent->parent->left;
            if (uncle->color == RED) {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;  // Invariant: root is always BLACK
}

// ─── Search ─────────────────────────────────────────────────────────────────

bool RBTree::search(long long key) const {
    RBNode* current = root;
    while (current != NIL) {
        if (key == current->key) return true;
        else if (key < current->key) current = current->left;
        else                          current = current->right;
    }
    return false;
}

// ─── Remove helpers ─────────────────────────────────────────────────────────

RBNode* RBTree::minimum(RBNode* node) {
    while (node->left != NIL)
        node = node->left;
    return node;
}

// transplant(u, v): replaces the subtree at u with v.
void RBTree::transplant(RBNode* u, RBNode* v) {
    if (u->parent == NIL)         root = v;
    else if (u == u->parent->left)     u->parent->left = v;
    else                               u->parent->right = v;
    v->parent = u->parent;
}

// ─── Remove ─────────────────────────────────────────────────────────────────

/*
 * remove: deletes the node with the given key.
 *
 * Case A — No left child:   promote right child.
 * Case B — No right child:  promote left child.
 * Case C — Two children:    replace with in-order successor (minimum of right subtree).
 */
void RBTree::remove(long long key) {
    RBNode* z = root;
    while (z != NIL && z->key != key) {
        if (key < z->key) z = z->left;
        else              z = z->right;
    }
    if (z == NIL) return;  // Not found

    RBNode* y = z;
    Color   yOriginalColor = y->color;
    RBNode* x;

    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (yOriginalColor == BLACK)
        deleteFix(x);

    delete z;
    treeSize--;
}

/*
 * deleteFix: restores RBT invariants after removing a BLACK node.
 * x carries an "extra BLACK" that must be resolved.
 *
 * Case 1 — Sibling RED:                    rotate to reach Case 2/3/4.
 * Case 2 — Sibling BLACK, both kids BLACK: recolor sibling, push problem up.
 * Case 3 — Sibling BLACK, far kid BLACK:   rotate sibling, convert to Case 4.
 * Case 4 — Sibling BLACK, far kid RED:     rotate parent, recolor (terminates).
 */
void RBTree::deleteFix(RBNode* x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;
            if (w->color == RED) {
                // Case 1
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                // Case 2
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    // Case 3
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                // Case 4
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        else {
            // Mirror cases
            RBNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

// ─── Accessors ──────────────────────────────────────────────────────────────

int     RBTree::size()    const { return treeSize; }
RBNode* RBTree::getRoot() const { return root; }
RBNode* RBTree::getNIL()  const { return NIL; }
