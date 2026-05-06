#include "RBTree.h"
#include <iostream>
#include <functional>

// ─────────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────────
RBTree::RBTree() {
    // Create the NIL sentinel node.
    // NIL acts as the universal "empty" placeholder — every leaf pointer
    // and the root's parent pointer point here instead of nullptr.
    // This removes the need for nullptr checks inside rotations and fix-ups.
    NIL = new RBNode(0);   // key=0, x=-1, y=-1 (not a real cell)
    NIL->color = BLACK;           // NIL is always BLACK (RBT invariant #3)
    NIL->left = NIL;             // NIL's children point back to itself
    NIL->right = NIL;             // so traversal loops terminate naturally

    // ── FIX: NIL->parent must also point to NIL ──────────────────────
    // Without this, insertFix() can access NIL->parent (e.g. when climbing
    // up via z = z->parent->parent) and read an uninitialized garbage pointer,
    // causing undefined behavior or a crash.
    NIL->parent = NIL;
    // ─────────────────────────────────────────────────────────────────

    root = NIL;             // empty tree: root is NIL
    treeSize = 0;               // no nodes yet
}

// ─────────────────────────────────────────────
//  Destructor
//  Recursively frees all nodes using a post-order
//  traversal (children before parent), then frees NIL.
// ─────────────────────────────────────────────
RBTree::~RBTree() {
    // Internal lambda to recursively delete all real nodes.
    // Must be post-order: free left subtree, then right, then the node itself.
    std::function<void(RBNode*)> freeAll = [&](RBNode* node) {
        if (node == NIL) return;    // hit a leaf boundary — stop
        freeAll(node->left);        // free everything in the left subtree
        freeAll(node->right);       // free everything in the right subtree
        delete node;                // now safe to free this node
        };
    freeAll(root);  // start from the root and recurse down
    delete NIL;     // finally free the sentinel itself
}

// ─────────────────────────────────────────────
//  Left Rotate
//
//  Pivots the subtree at x to the left:
//
//       x                y
//      / \              / \
//     A   y    →       x   C
//        / \          / \
//       B   C        A   B
//
//  The BST ordering (A < x < B < y < C) is preserved.
//  Called from insertFix and deleteFix.
// ─────────────────────────────────────────────
void RBTree::leftRotate(RBNode* x) {
    RBNode* y = x->right;       // y is x's right child — it will move up

    x->right = y->left;         // y's left subtree (B) becomes x's new right subtree
    // because B's keys are between x and y

    if (y->left != NIL)         // if B is a real node (not a leaf)
        y->left->parent = x;    //   update B's parent to x (it's moving under x)

    y->parent = x->parent;      // y inherits x's old parent position

    if (x->parent == NIL)       // if x was the root
        root = y;               //   y becomes the new root
    else if (x == x->parent->left)   // if x was its parent's left child
        x->parent->left = y;         //   replace x with y on the left
    else
        x->parent->right = y;   // otherwise replace x with y on the right

    y->left = x;              // x becomes y's new left child
    x->parent = y;              // x's parent is now y
}

// ─────────────────────────────────────────────
//  Right Rotate
//  Mirror image of leftRotate — pivots the subtree at y upward to the right.
//
//       y                x
//      / \              / \
//     x   C    →       A   y
//    / \                  / \
//   A   B                B   C
// ─────────────────────────────────────────────
void RBTree::rightRotate(RBNode* y) {
    RBNode* x = y->left;        // x is y's left child — it will move up

    y->left = x->right;         // x's right subtree (B) becomes y's new left subtree
    // because B's keys are between x and y

    if (x->right != NIL)        // if B is a real node
        x->right->parent = y;   //   update B's parent to y

    x->parent = y->parent;      // x inherits y's old parent position

    if (y->parent == NIL)       // if y was the root
        root = x;               //   x becomes the new root
    else if (y == y->parent->right)  // if y was its parent's right child
        y->parent->right = x;        //   replace y with x on the right
    else
        y->parent->left = x;    // otherwise replace y with x on the left

    x->right = y;              // y becomes x's new right child
    y->parent = x;              // y's parent is now x
}

// ─────────────────────────────────────────────
//  Insert
//  Adds a new (key, x, y) cell to the tree.
//  If the key already exists, silently returns — this is the mechanism
//  that ensures each (x,y) cell is only toggled once (first occurrence only).
// ─────────────────────────────────────────────
void RBTree::insert(long long key, int x, int y) {
    // Standard BST traversal to find the correct insertion position.
    RBNode* parent = NIL;      // will track the parent of the insertion point
    RBNode* current = root;     // start searching from the root

    while (current != NIL) {            // walk down until we hit a NIL leaf
        parent = current;               // remember the last real node we visited
        if (key < current->key)
            current = current->left;    // go left — key is smaller
        else if (key > current->key)
            current = current->right;   // go right — key is larger
        else
            return;                     // exact match found → duplicate, skip it
    }

    // Create the new node — starts RED (required by RBT insert algorithm)
    RBNode* z = new RBNode(key, x, y); // allocate and store key + coordinates
    z->left = NIL;                    // new node's children are NIL leaves
    z->right = NIL;
    z->parent = parent;                 // attach to the parent we found above
    // Note: z->color is already RED from the constructor — no need to set it again

    // Wire the new node into the tree
    if (parent == NIL)              // tree was empty — new node becomes root
        root = z;
    else if (key < parent->key)     // new node is smaller — goes left
        parent->left = z;
    else                            // new node is larger — goes right
        parent->right = z;

    insertFix(z);   // restore RBT invariants (may recolor and rotate)
    treeSize++;     // one more real node in the tree
}

// ─────────────────────────────────────────────
//  Insert Fix-Up
//  Restores the Red-Black invariants after a BST insert.
//  A new RED node may create a RED-RED parent-child violation.
//  This is resolved through one of three cases:
//
//  Case 1: Uncle is RED → recolor parent, uncle, and grandparent. Move z up.
//  Case 2: Uncle is BLACK, z is an "inner" child → rotate z's parent to convert to Case 3.
//  Case 3: Uncle is BLACK, z is an "outer" child → rotate grandparent and recolor.
//
//  Each case has a left and right mirror depending on whether z's parent
//  is a left or right child of the grandparent.
// ─────────────────────────────────────────────
void RBTree::insertFix(RBNode* z) {
    // Keep fixing as long as z's parent is RED (violation exists)
    while (z->parent->color == RED) {

        if (z->parent == z->parent->parent->left) {
            // z's parent is the LEFT child of the grandparent
            RBNode* uncle = z->parent->parent->right;  // uncle is on the right

            if (uncle->color == RED) {
                // ── Case 1: Uncle is RED ──────────────────────────────
                // Both parent and uncle are RED under a BLACK grandparent.
                // Recolor them BLACK and push the RED up to grandparent,
                // then continue fixing from the grandparent.
                z->parent->color = BLACK;  // parent goes BLACK
                uncle->color = BLACK;  // uncle goes BLACK
                z->parent->parent->color = RED;    // grandparent goes RED
                z = z->parent->parent;              // move z up to grandparent
                // (grandparent might now violate with ITS parent)
            }
            else {
                // Uncle is BLACK — need rotations
                if (z == z->parent->right) {
                    // ── Case 2: z is an inner (right) child ──────────
                    // The violation is "zig-zag" shaped — rotate parent
                    // to convert this into the straight Case 3 shape.
                    z = z->parent;      // move z up (it will drop after rotation)
                    leftRotate(z);      // rotate left around old parent
                }
                // ── Case 3: z is an outer (left) child ───────────────
                // The violation is "zig-zig" shaped — one rotation fixes it.
                z->parent->color = BLACK;  // parent goes BLACK (becomes new subtree root)
                z->parent->parent->color = RED;    // grandparent goes RED (drops down)
                rightRotate(z->parent->parent);     // rotate grandparent right
                // After this rotation, the subtree is balanced and correctly colored
            }
        }
        else {
            // z's parent is the RIGHT child of the grandparent (mirror cases)
            RBNode* uncle = z->parent->parent->left;   // uncle is on the left

            if (uncle->color == RED) {
                // ── Case 1 (mirror): Uncle is RED ────────────────────
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    // ── Case 2 (mirror): z is an inner (left) child ──
                    z = z->parent;
                    rightRotate(z);
                }
                // ── Case 3 (mirror): z is an outer (right) child ─────
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    // Invariant #2: root must always be BLACK.
    // If we bubbled RED all the way to the root in Case 1, fix it here.
    root->color = BLACK;
}

// ─────────────────────────────────────────────
//  Search
//  Returns true if 'key' exists in the tree, false otherwise.
//  Standard BST search: go left if key is smaller, right if larger.
//  Marked const — this method never modifies the tree.
// ─────────────────────────────────────────────
bool RBTree::search(long long key) const {
    RBNode* current = root;         // start at the root

    while (current != NIL) {        // loop until we hit a NIL leaf (not found)
        if (key == current->key)
            return true;            // exact match — key exists in the tree
        else if (key < current->key)
            current = current->left;    // key is smaller — go left
        else
            current = current->right;   // key is larger — go right
    }
    return false;                   // fell off the tree — key not found
}

// ─────────────────────────────────────────────
//  Minimum
//  Returns the node with the smallest key in the subtree rooted at 'node'.
//  In a BST, the minimum is always the leftmost node.
//  Used by remove() to find a deleted node's in-order successor.
// ─────────────────────────────────────────────
RBNode* RBTree::minimum(RBNode* node) {
    while (node->left != NIL)   // keep going left as long as there's a left child
        node = node->left;
    return node;                // the leftmost node is the minimum
}

// ─────────────────────────────────────────────
//  Transplant
//  Replaces the subtree rooted at u with the subtree rooted at v.
//  This rewires only the parent pointer — it does NOT update v's children.
//  Used by remove() to splice out a node without restructuring the whole tree.
// ─────────────────────────────────────────────
void RBTree::transplant(RBNode* u, RBNode* v) {
    if (u->parent == NIL)           // u was the root
        root = v;                   //   v becomes the new root
    else if (u == u->parent->left)  // u was its parent's left child
        u->parent->left = v;        //   replace u with v on the left
    else
        u->parent->right = v;       // replace u with v on the right

    v->parent = u->parent;          // v now points up to u's old parent
    // (safe even when v is NIL — NIL->parent gets updated,
    //  which is why NIL->parent = NIL in the constructor matters)
}

// ─────────────────────────────────────────────
//  Remove
//  Deletes the node with the given key from the tree.
//  Handles three cases based on how many children the target node has:
//    Case A: No left child  → replace with right child
//    Case B: No right child → replace with left child
//    Case C: Two children   → replace with in-order successor (minimum of right subtree)
// ─────────────────────────────────────────────
void RBTree::remove(long long key) {
    // First, find the node to delete (standard BST search)
    RBNode* z = root;
    while (z != NIL && z->key != key) {
        if (key < z->key)
            z = z->left;
        else
            z = z->right;
    }

    if (z == NIL) return;           // key not found — nothing to delete

    RBNode* y = z;                          // y tracks the node that will be physically removed
    Color yOriginalColor = y->color;        // remember y's original color — if y was BLACK,
    // removing it breaks the equal-black-height invariant
    // and we'll need to call deleteFix
    RBNode* x;                              // x will replace y in the tree structure

    if (z->left == NIL) {
        // ── Case A: z has no left child ─────────────────────────────
        // Simply promote z's right child (could be NIL) into z's spot
        x = z->right;
        transplant(z, z->right);

    }
    else if (z->right == NIL) {
        // ── Case B: z has no right child ────────────────────────────
        // Simply promote z's left child into z's spot
        x = z->left;
        transplant(z, z->left);

    }
    else {
        // ── Case C: z has two children ──────────────────────────────
        // Find the in-order successor: the smallest node in z's right subtree.
        // This successor y will take z's place (it has z's value for BST ordering).
        y = minimum(z->right);
        yOriginalColor = y->color;      // recheck y's color — it's now the successor, not z
        x = y->right;                   // x is what replaces y after y moves up

        if (y->parent == z) {
            // y is z's direct right child — x stays in place
            x->parent = y;              // keep x's parent pointer pointing to y
            // (handles the case where x is NIL)
        }
        else {
            // y is deeper — splice y out of its current position first
            transplant(y, y->right);    // replace y with y's right child
            y->right = z->right;        // give y its new right subtree (z's old right)
            y->right->parent = y;       // update that subtree's parent pointer
        }

        // Now put y in z's place
        transplant(z, y);               // replace z with y
        y->left = z->left;              // give y z's left subtree
        y->left->parent = y;            // update left subtree's parent
        y->color = z->color;            // y takes z's color to preserve black-heights
    }

    // If the physically removed node (y) was BLACK, we may have lost
    // black-height balance — fix it starting from x (what took y's place)
    if (yOriginalColor == BLACK)
        deleteFix(x);

    delete z;       // free the memory of the removed node
    treeSize--;     // one fewer node in the tree
}

// ─────────────────────────────────────────────
//  Delete Fix-Up
//  Restores RBT invariants after removing a BLACK node.
//  x is treated as having an "extra" BLACK — we push this extra
//  BLACK up the tree through 4 cases until it lands on a RED node
//  (which we recolor BLACK) or reaches the root (which we just color BLACK).
//
//  Case 1: Sibling w is RED → rotate to convert to Case 2/3/4
//  Case 2: Sibling w is BLACK with two BLACK children → recolor w RED, move up
//  Case 3: Sibling w is BLACK, near child RED, far child BLACK → rotate + recolor → Case 4
//  Case 4: Sibling w is BLACK, far child RED → rotate + recolor (terminates)
// ─────────────────────────────────────────────
void RBTree::deleteFix(RBNode* x) {
    // Keep fixing until x is the root or x has absorbed the extra BLACK
    while (x != root && x->color == BLACK) {

        if (x == x->parent->left) {
            // x is a LEFT child — sibling w is on the right
            RBNode* w = x->parent->right;   // w is x's sibling

            if (w->color == RED) {
                // ── Case 1: Sibling w is RED ──────────────────────────
                // Rotate to convert: after this, x's new sibling will be BLACK
                w->color = BLACK;      // w goes BLACK
                x->parent->color = RED;        // parent goes RED
                leftRotate(x->parent);          // rotate parent left
                w = x->parent->right;           // update w to x's new sibling
                // Now falls through to Case 2, 3, or 4
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                // ── Case 2: Both of w's children are BLACK ────────────
                // We can recolor w RED and push the extra BLACK up to parent
                w->color = RED;         // w absorbs x's extra BLACK, becomes RED
                x = x->parent;          // move the "extra BLACK" problem one level up
            }
            else {
                if (w->right->color == BLACK) {
                    // ── Case 3: w's far child (right) is BLACK, near child (left) is RED
                    // Rotate w right to convert to Case 4
                    w->left->color = BLACK;    // w's left child goes BLACK
                    w->color = RED;      // w goes RED
                    rightRotate(w);             // rotate w right
                    w = x->parent->right;       // update w
                }
                // ── Case 4: w's far child (right) is RED ─────────────
                // One left rotation on parent completely resolves the violation
                w->color = x->parent->color; // w takes parent's color
                x->parent->color = BLACK;            // parent goes BLACK
                w->right->color = BLACK;            // w's far child goes BLACK
                leftRotate(x->parent);                // rotate parent left
                x = root;                             // done — exit the loop
            }
        }
        else {
            // x is a RIGHT child — sibling w is on the left (mirror cases)
            RBNode* w = x->parent->left;

            if (w->color == RED) {
                // ── Case 1 (mirror) ───────────────────────────────────
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                // ── Case 2 (mirror) ───────────────────────────────────
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    // ── Case 3 (mirror) ───────────────────────────────
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                // ── Case 4 (mirror) ───────────────────────────────────
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    // x has absorbed its extra BLACK — color it BLACK to finalize
    // (also handles the case where x reached the root with an extra BLACK)
    x->color = BLACK;
}

// ─────────────────────────────────────────────
//  Size — returns the number of real nodes in the tree
// ─────────────────────────────────────────────
int RBTree::size() const {
    return treeSize;    // maintained by insert() and remove()
}

// ─────────────────────────────────────────────
//  getRoot — exposes the root for the GUI tree visualizer (Person 4)
// ─────────────────────────────────────────────
RBNode* RBTree::getRoot() const {
    return root;        // Person 4 calls this to start their tree traversal
}

// ─────────────────────────────────────────────
//  getNIL — exposes the NIL sentinel for the GUI
//  Person 4 needs this to check: if (node == tree.getNIL()) → it's a leaf, stop drawing
// ─────────────────────────────────────────────
RBNode* RBTree::getNIL() const {
    return NIL;
}
