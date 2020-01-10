#include <iostream>
#include <unordered_map>


namespace bst {

struct TreeNode {
    int key;
    int val;
    TreeNode* left, right;
    TreeNode(T k, U v): key(k), val(v), left(nullptr), right(nullptr) {};
};

class BST {
public:
    std::shared_ptr<TreeNode> search(int key) {
        return search_recursive(key, root_);
    }

    std::shared_ptr<TreeNode> search_recursive(int key, std::shared_ptrTreeNode> root) {
        if (root == nullptr || root->key == key) {
            return root;
        }
        if (key < root->key) {
            return search_recursive(key, root->left);
        }
        return search_recursive(key, root->right);
    }

    std::shared_ptr<TreeNode> search_iterative(int key, std::shared_ptr<TreeNode> root) {
        TreeNode* ptr = root;
        while (ptr != nullptr) {
            if (key == ptr->key) {
                return ptr;
            }
            if (key < ptr->key) {
                ptr = ptr->left;
            } else {
                ptr = ptr->right;
            }
        }
        return ptr;
    }

    void insert(int key, int value) {
        void insert_recursive(key, value, root_);
    }

    void insert_recursive(int key, int value, std::shared_ptr<TreeNode> root) {
        if (root == nullptr) {
            root = std::make_shared<TreeNode>(key, value);
        }
    }

    void insert_iterative(int key, int value) {}

    void insert(TreeNode* ptr) {
        if (root_ == nullptr) {
            root_ = std::move(ptr);
            return;
        }

        TreeNode* ptr2 = root_.get();
        while (true) {
            if (ptr->key <= ptr2->key) {
                // go left
            } else {
                // go right
            }
        }
    }

    void delete() {}

private:
    std::shared_ptr<TreeNode> root_ = nullptr;
};

template<class T>
class AVLTree {
    
};


};

