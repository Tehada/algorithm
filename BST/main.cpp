#include <vector>

struct node() {
    int key;
    *node left;
    *node right;
    *node p;
}

struct tree() {
    std::vector<node> nodes;
    *node root;
}

node Tree_search(node x, int k) {
    if (x == nullptr || k == x.key) {
        return x;
    }
    if (k < x.key) {
        return Tree_search(x.left, k);
    } else {
        return Tree_search(x.right, k);
    }
}

node Iterative_tree_search(node x, int k) {
    while (x != nullptr && k != x.key) {
        if (k < x.key) {
            x = x.left;
        } else {
            x = x.right;
        }
    }
    return x;
}

node Tree_minimum(node x) {
    while (x.left != nullptr) {
        x = x.left;
    }
    return x;
}

node Recursive_tree_minimun(node x) {
    if (x.left == nullptr) {
        return x;
    }
    return Recursive_tree_minimun(x.left);
}

node Recursive_tree_maximum(node x) {
    if (x.right == nullptr) {
        return x;
    }
    return Recursive_tree_maximum(x.right);
}

node Tree_maximum(node x) {
    while (x.right != nullptr) {
        x = x.right;
    }
    return x;
}

node Tree_successor(node x) {
    if (x.right != nullptr) {
        return Tree_minimum(x.right);
    }
    y = x.p;
    while (y != nullptr && x == y.right) {
        x = y;
        y = y.p;
    }
    return y;
}

node Tree_predecessor(node x) {
    if (x.left != nullptr) {
        return Tree_maximum(x.left);
    }
    y = x.p;
    while (y != nullptr && x == y.left) {
        x = y;
        y = y.p;
    }
    return y;
}

void Inorder_tree_walk(node x) {
    if (x != nullptr) {
        Inorder_tree_walk(x.left);
        std::cout << x.key << "\n";
        Inorder_tree_walk(x.right);
    }
}

void Tree_insert(tree T, node z) {
    y = nullptr;
    x = T.root;
    while (x != nullptr) {
        y = x;
        if (z.key < x.key) {
            x = x.left;
        } else {
            x = x.right;
        }
    }
    z.p = y;
    if (y == nullptr) {
        T.root = z;
    } else if (z.key < y.key) {
        y.left = z;
    } else {
        y.right = z;
    }
}

void Transplant(tree T, node u, node v) {
    if (u.p == nullptr) {
        T.root = v;
    } else if (u == u.p.left) {
        u.p.left = v;
    } else {
        u.p.right = v;
    }
    if (v != nullptr) {
        v.p = u.p;
    }
}

void Tree_delete(tree T, node z) {
    if (z.left == nullptr) {
        Transplant(T, z, z.right);
    } else if (z.right == nullptr) {
        Transplant(T, z, z.left);
    } else {
        y = Tree_minimum(z.right);
        if (y.p != z) {
            Transplant(T, y, y.right);
            y.right = z.right;
            y.right.p = y;
        }
        Transplant(T, z, y);
        y.left = z.left;
        y.left.p = y;
    }
}