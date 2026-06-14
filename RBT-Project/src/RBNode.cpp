#include "../include/RBNode.h"

RBNode::RBNode(long long key, int x, int y) {
    this->key = key;   
    this->x = x;     // row
    this->y = y;     // column 
    color = RED;   
    left = right = parent = nullptr;
