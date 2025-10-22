#pragma once

#include "avl_tree.hh"

namespace AVLTree {

template <typename KeyT>
std::size_t distance(typename AVLTree<KeyT>::Iterator first, typename AVLTree<KeyT>::Iterator last) {
    std::size_t count = 0;

    while (first != last) {
        ++first;
        ++count;
    }

    return count;
}

template <typename KeyT>
std::size_t range_query(const AVLTree<KeyT>& tree, const KeyT& min, const KeyT& max) {
    auto start_it = tree.lower_bound(min);
    auto end_it = tree.upper_bound(max);

    return distance<KeyT>(start_it, end_it);
}

}; // namespace AVLTree
