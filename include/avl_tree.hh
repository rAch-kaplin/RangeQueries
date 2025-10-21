#pragma once

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

namespace AVLTree {

enum class Error {
    OK = 0x000000,
    NODE_NULLPTR = 0x000001,
    REPEAT_ELEM = 0x000002,
};

template <typename KeyT>
class AVLTree {
  private:
    struct Node {
        std::unique_ptr<Node> left_ = nullptr;
        std::unique_ptr<Node> right_ = nullptr;
        Node *parent_ = nullptr;

        int64_t height_;
        KeyT key_;

        explicit Node(const KeyT& key) : key_(key), height_(1) {}
        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };

    std::unique_ptr<Node> root_ = nullptr;

    int64_t get_height(const std::unique_ptr<Node>& node) const { return node ? node->height_ : 0; }

    int64_t get_balance_factor(const std::unique_ptr<Node>& node) const {
        return node ? get_height(node->left_) - get_height(node->right_) : -1;
    }

    Error update_height(std::unique_ptr<Node>& node) {
        assert(node);
        if (node == nullptr) return Error::NODE_NULLPTR;

        node->height_ = 1 + std::max(get_height(node->left_), get_height(node->right_));

        return Error::OK;
    }

    Error right_rotate(std::unique_ptr<Node>& node) {
        assert(node);

        std::unique_ptr<Node> new_root = std::move(node->left_);
        std::unique_ptr<Node> temp = std::move(new_root->right_);

        new_root->right_ = std::move(node);
        new_root->right_->left_ = std::move(temp);

        if (new_root->right_->left_) {
            new_root->right_->left_->parent_ = new_root->right_.get();
        }
        new_root->right_->parent_ = new_root.get();

        update_height(new_root->right_);
        update_height(new_root);

        node = std::move(new_root);

        return Error::OK;
    }

    Error left_rotate(std::unique_ptr<Node>& node) {
        assert(node);

        std::unique_ptr<Node> new_root = std::move(node->right_);
        std::unique_ptr<Node> temp = std::move(new_root->left_);

        new_root->left_ = std::move(node);
        new_root->left_->right_ = std::move(temp);

        if (new_root->left_->right_) {
            new_root->left_->right_->parent_ = new_root->left_.get();
        }
        new_root->left_->parent_ = new_root.get();

        update_height(new_root->left_);
        update_height(new_root);

        node = std::move(new_root);

        return Error::OK;
    }

    Error insert_node(std::unique_ptr<Node>& root, const KeyT& key) {
        if (root == nullptr) {
            root = std::make_unique<Node>(key);
            return Error::OK;
        }

        if (key < root->key_) {
            auto err = insert_node(root->left_, key);
            if (err != Error::OK) return err;
            root_->left_->parent_ = root_.get();
        } else if (key > root->key_) {
            auto err = insert_node(root->right_, key);
            if (err != Error::OK) return err;
            root_->right_->parent_ = root_.get();
        } else {
            return Error::REPEAT_ELEM;
        }

        update_height(root);
        int64_t balance_factor = get_balance_factor(root);

        if (balance_factor > 1) {
            if (key < root->left_->key_) {
                // LL case
                right_rotate(root);
            } else {
                // LR case
                left_rotate(root->left_);
                right_rotate(root);
            }
        } else if (balance_factor < -1) {
            if (key > root->right_->key_) {
                // RR case
                left_rotate(root);
            } else {
                // RL case
                right_rotate(root->right_);
                left_rotate(root);
            }
        }

        return Error::OK;
    }

    void generate_graph(std::ostream& out, const std::unique_ptr<Node>& node) const {
        if (!node) return;
        std::string node_ptr = "node_" + std::to_string(reinterpret_cast<uintptr_t>(node.get()));
        out << "\t" << node_ptr << " [shape=plaintext, style=filled, color=\"#fcf0d2\", label=<\n"
            << "\t\t<table BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"6\" "
               "BGCOLOR=\"#bfb58f\" COLOR=\"#4d3d03\">\n"
            << "\t\t\t<tr><td align=\"center\" colspan=\"2\"><FONT COLOR=\"#3a3a3a\"><b> " << node_ptr << "</b></FONT></td></tr>\n"
            << "\t\t\t<tr><td align=\"center\" colspan=\"2\"><FONT COLOR=\"#4d3d03\">Key: <b>" << node->key_ << "</b></FONT></td></tr>\n"
            << "\t\t\t<tr><td align=\"center\" colspan=\"2\"><FONT COLOR=\"#4d3d03\">Height: <b>" << node->height_ << "</b></FONT></td></tr>\n"
            << "\t\t\t<tr>\n"
            << "\t\t\t\t<td WIDTH=\"150\" PORT=\"parent\" align=\"center\"><FONT COLOR=\"#00008b\"><b>Parent: "
            << (node->parent_ ? "node_" + std::to_string(reinterpret_cast<uintptr_t>(node->parent_)) : std::string("nullptr"))
            << "</b></FONT></td>\n"
            << "\t\t\t\t<td WIDTH=\"150\" PORT=\"left\" align=\"center\"><FONT COLOR=\"#006400\"><b>Left: "
            << (node->left_ ? "node_" + std::to_string(reinterpret_cast<uintptr_t>(node->left_.get())) : "nullptr") << "</b></FONT></td>\n"
            << "\t\t\t\t<td WIDTH=\"150\" PORT=\"right\" align=\"center\"><FONT COLOR=\"#8b0000\"><b>Right: "
            << (node->right_ ? "node_" + std::to_string(reinterpret_cast<uintptr_t>(node->right_.get())) : "nullptr") << "</b></FONT></td>\n"
            << "\t\t\t</tr>\n"
            << "\t\t</table>\n"
            << "\t>];\n";

        if (node->left_) {
            generate_graph(out, node->left_);
            std::string left_id = "node_" + std::to_string(reinterpret_cast<uintptr_t>(node->left_.get()));
            out << "\t" << node_ptr << ":left -> " << left_id << " [color=\"#006400\", style=bold];\n";
        }
        if (node->right_) {
            generate_graph(out, node->right_);
            std::string right_id = "node_" + std::to_string(reinterpret_cast<uintptr_t>(node->right_.get()));
            out << "\t" << node_ptr << ":right -> " << right_id << " [color=\"#8b0000\", style=bold];\n";
        }
    }

    bool dump_to_dot(const std::string& filename) const {
        namespace fs = std::filesystem;
        fs::create_directories("graph");

        std::ofstream file("graph/" + filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open " << filename << " for writing\n";
            return false;
        }

        file << "digraph G {\n"
             << "\trankdir = HR;\n"
             << "\tbgcolor=\"#fcf0d2\";\n";

        generate_graph(file, root_);

        file << "}\n";
        return true;
    }

    std::size_t range_query_internal(const std::unique_ptr<Node>& node, const KeyT& min, const KeyT& max) const {
        if (node == nullptr) return 0;

        if (node->key_ <= min) {
            return range_query_internal(node->right_, min, max);
        } else if (node->key_ > max) {
            return range_query_internal(node->left_, min, max);
        }

        return 1 + range_query_internal(node->left_, min, max) + range_query_internal(node->right_, min, max);
    }

   public:
    AVLTree() = default;
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;
    AVLTree(const AVLTree&&) noexcept = default;
    AVLTree& operator=(const AVLTree&&) noexcept = default;
    ~AVLTree() = default;

    class Iterator {
        const Node* node_;

      public:
        explicit Iterator(const Node* node) : node_(node) {};

        const KeyT& operator*() const {
            assert(node_);
            return node_->key_;
        }

        const KeyT* operator->() const {
            assert(node_);
            return &(node_->key_);
        }

        bool operator==(const Iterator& other) const {
            return node_ == other.node_;
        }

        bool operator!=(const Iterator& other) const {
            return node_ != other.node_;
        }

        Iterator& operator++() {
            if (node_->right_ != nullptr) {
                node_ = node_->right_.get();

                while (node_->left_ != nullptr) {
                    node_ = node_->left_.get();
                }
            } else {

            }
        }
    };

    void insert(KeyT key) {
        auto err = insert_node(root_, key);
        if (err != Error::OK) return;
        return;
    }

    std::size_t range_query(const KeyT& min, const KeyT& max) const {
        if (min > max) {
            return 0;
        }

        return range_query_internal(root_, min, max);
    }

    bool dump_to_png() const {
        static int dump_counter = 0;
        const std::string dot_file = "graph/dump.dot";
        const std::string png_file = "graph/img/dump_" + std::to_string(dump_counter++) + ".png";

        std::filesystem::create_directories("graph/img");

        if (!dump_to_dot("dump.dot")) {
            return false;
        }

        std::string command = "dot -Tpng " + dot_file + " -o " + png_file;
        int result = std::system(command.c_str());

        if (result != 0) {
            std::cerr << "Error: 'dot' command failed. Is Graphviz installed?\n";
            return false;
        }

        std::cout << "Tree dumped to " << png_file << "\n";
        return true;
    }
};

};  // namespace AVLTree
