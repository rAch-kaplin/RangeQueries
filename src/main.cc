#include <iostream>

#include "avl_tree.hh"

int main() {
    AVLTree::AVLTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(60);
    tree.insert(70);

    if (!tree.dump_to_png()) {
        std::cerr << "Failed to generate PNG dump!\n";
        return 1;
    }

    return 0;
}
