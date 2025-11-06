#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include "avl_tree.hh"


template <typename T>
void inorder_traversal(const AVLTree::AVLTree<T>& tree, std::vector<T>& result) {
    auto it = tree.lower_bound(std::numeric_limits<T>::lowest());
    for (; it != typename AVLTree::AVLTree<T>::Iterator(nullptr); ++it)
        result.push_back(*it);
}


TEST(AVLTreeInsert, SingleInsert) {
    AVLTree::AVLTree<int> tree;
    tree.insert(10);

    auto it = tree.lower_bound(10);
    ASSERT_NE(it, typename AVLTree::AVLTree<int>::Iterator(nullptr));
    EXPECT_EQ(*it, 10);
}

TEST(AVLTreeInsert, MultipleInserts) {
    AVLTree::AVLTree<int> tree;
    std::vector<int> keys = {10, 5, 15, 3, 7, 12, 18};
    for (int k : keys) tree.insert(k);

    std::vector<int> result;
    inorder_traversal(tree, result);
    std::sort(keys.begin(), keys.end());
    EXPECT_EQ(result, keys);
}

TEST(AVLTreeInsert, DuplicateInsert) {
    AVLTree::AVLTree<int> tree;
    tree.insert(10);

    testing::internal::CaptureStderr();
    tree.insert(10);
    std::string err = testing::internal::GetCapturedStderr();

    EXPECT_NE(err.find("duplicate key"), std::string::npos);
}

TEST(AVLTreeInsert, LLRotation) {
    AVLTree::AVLTree<int> tree;
    tree.insert(30);
    tree.insert(20);
    tree.insert(10); // LL case

    std::vector<int> result;
    inorder_traversal(tree, result);
    EXPECT_EQ(result, (std::vector<int>{10, 20, 30}));
}

TEST(AVLTreeInsert, RRRotation) {
    AVLTree::AVLTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30); // RR case

    std::vector<int> result;
    inorder_traversal(tree, result);
    EXPECT_EQ(result, (std::vector<int>{10, 20, 30}));
}

TEST(AVLTreeInsert, LRRotation) {
    AVLTree::AVLTree<int> tree;
    tree.insert(30);
    tree.insert(10);
    tree.insert(20); // LR case

    std::vector<int> result;
    inorder_traversal(tree, result);
    EXPECT_EQ(result, (std::vector<int>{10, 20, 30}));
}

TEST(AVLTreeInsert, RLRotation) {
    AVLTree::AVLTree<int> tree;
    tree.insert(10);
    tree.insert(30);
    tree.insert(20); // RL case

    std::vector<int> result;
    inorder_traversal(tree, result);
    EXPECT_EQ(result, (std::vector<int>{10, 20, 30}));
}

TEST(AVLTreeInsert, LargeInsertions) {
    AVLTree::AVLTree<int> tree;
    std::vector<int> keys(1000);
    std::iota(keys.begin(), keys.end(), 1);
    std::shuffle(keys.begin(), keys.end(), std::mt19937{42});

    for (int k : keys) tree.insert(k);

    std::vector<int> result;
    inorder_traversal(tree, result);

    std::vector<int> expected(1000);
    std::iota(expected.begin(), expected.end(), 1);
    EXPECT_EQ(result, expected);
}

TEST(AVLTreeInsert, DumpToPngDoesNotThrow) {
    AVLTree::AVLTree<int> tree;
    for (int k : {10, 5, 20, 15, 25}) tree.insert(k);

    EXPECT_NO_THROW({
        bool ok = tree.dump_to_png();
        EXPECT_TRUE(ok || !ok); // просто не должно падать
    });
}

