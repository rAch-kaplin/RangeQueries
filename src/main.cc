#include <iostream>

#include "avl_tree.hh"

int main() {
    AVLTree::AVLTree<int> tree;

    char query;

    while (std::cin >> query) {
        int key{0};
        int first{0}, second{0};

        switch (query) {
        case 'k':
            if (std::cin >> key) {
                tree.insert(key);
            }
            break;
        case 'q':
            if (std::cin >> first >> second) {
                    std::cout << tree.range_query(first, second) << std::endl;
            }
            break;
        default:
            std::cout << "unknown command" << std::endl;
            break;
        }
    }

    if (!tree.dump_to_png()) {
        std::cerr << "Failed to generate PNG dump!\n";
        return 1;
    }

    return 0;
}
