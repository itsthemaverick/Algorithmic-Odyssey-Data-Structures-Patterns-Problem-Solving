#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <queue>
#include <sstream>
#include <vector>

/**
 * @brief A production-grade Binary Search Tree (BST) implementation.
 * 
 * @tparam T The type of elements stored in the tree. Must support comparison operators.
 */
template <typename T>
class BST {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;

        explicit Node(const T& val) : value(val), left(nullptr), right(nullptr) {}
        explicit Node(T&& val) : value(std::move(val)), left(nullptr), right(nullptr) {}
    };

    Node* root;
    size_t tree_size;

    // Helper functions for recursive operations
    Node* insert_helper(Node* current, T value) {
        if (!current) {
            ++tree_size;
            return new Node(std::move(value));
        }

        if (value < current->value) {
            current->left = insert_helper(current->left, std::move(value));
        } else if (value > current->value) {
            current->right = insert_helper(current->right, std::move(value));
        }
        // Duplicate values are ignored in this standard BST implementation.
        return current;
    }

    Node* find_min(Node* current) const {
        while (current && current->left) {
            current = current->left;
        }
        return current;
    }

    Node* find_max(Node* current) const {
        while (current && current->right) {
            current = current->right;
        }
        return current;
    }

    Node* remove_helper(Node* current, const T& value, bool& removed) {
        if (!current) return nullptr;

        if (value < current->value) {
            current->left = remove_helper(current->left, value, removed);
        } else if (value > current->value) {
            current->right = remove_helper(current->right, value, removed);
        } else {
            // Node found
            removed = true;

            // Case 1: Leaf node or single child
            if (!current->left) {
                Node* temp = current->right;
                delete current;
                return temp;
            } else if (!current->right) {
                Node* temp = current->left;
                delete current;
                return temp;
            }

            // Case 2: Node with two children
            // Find in-order successor (smallest in the right subtree)
            Node* successor = find_min(current->right);
            current->value = successor->value;
            current->right = remove_helper(current->right, successor->value, removed);
        }
        return current;
    }

    void destroy_tree(Node* current) {
        if (current) {
            destroy_tree(current->left);
            destroy_tree(current->right);
            delete current;
        }
    }

    Node* copy_tree(const Node* other_node) {
        if (!other_node) return nullptr;
        Node* new_node = new Node(other_node->value);
        new_node->left = copy_tree(other_node->left);
        new_node->right = copy_tree(other_node->right);
        return new_node;
    }

    size_t calculate_height(const Node* current) const {
        if (!current) return 0;
        return 1 + std::max(calculate_height(current->left), calculate_height(current->right));
    }

    void in_order_traverse(const Node* current, const std::function<void(const T&)>& visitor) const {
        if (current) {
            in_order_traverse(current->left, visitor);
            visitor(current->value);
            in_order_traverse(current->right, visitor);
        }
    }

    void pre_order_traverse(const Node* current, const std::function<void(const T&)>& visitor) const {
        if (current) {
            visitor(current->value);
            pre_order_traverse(current->left, visitor);
            pre_order_traverse(current->right, visitor);
        }
    }

    void post_order_traverse(const Node* current, const std::function<void(const T&)>& visitor) const {
        if (current) {
            post_order_traverse(current->left, visitor);
            post_order_traverse(current->right, visitor);
            visitor(current->value);
        }
    }

public:
    // =========================================================================
    // Constructors, Destructor & Copy/Move Operations
    // =========================================================================

    BST() : root(nullptr), tree_size(0) {}

    BST(std::initializer_list<T> list) : root(nullptr), tree_size(0) {
        for (const auto& item : list) {
            insert(item);
        }
    }

    ~BST() {
        destroy_tree(root);
    }

    // Copy Constructor
    BST(const BST& other) : root(copy_tree(other.root)), tree_size(other.tree_size) {}

    // Move Constructor
    BST(BST&& other) noexcept : root(other.root), tree_size(other.tree_size) {
        other.root = nullptr;
        other.tree_size = 0;
    }

    // Copy Assignment Operator
    BST& operator=(const BST& other) {
        if (this != &other) {
            destroy_tree(root);
            root = copy_tree(other.root);
            tree_size = other.tree_size;
        }
        return *this;
    }

    // Move Assignment Operator
    BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            destroy_tree(root);
            root = other.root;
            tree_size = other.tree_size;

            other.root = nullptr;
            other.tree_size = 0;
        }
        return *this;
    }

    // =========================================================================
    // Capacity & Structural Queries
    // =========================================================================

    [[nodiscard]] size_t size() const noexcept { return tree_size; }
    [[nodiscard]] bool empty() const noexcept { return tree_size == 0; }
    [[nodiscard]] size_t height() const noexcept { return calculate_height(root); }

    void clear() noexcept {
        destroy_tree(root);
        root = nullptr;
        tree_size = 0;
    }

    // =========================================================================
    // Core Operations (Insert, Search, Remove, Min/Max)
    // =========================================================================

    void insert(const T& value) {
        root = insert_helper(root, value);
    }

    void insert(T&& value) {
        root = insert_helper(root, std::move(value));
    }

    [[nodiscard]] bool search(const T& target) const {
        Node* current = root;
        while (current) {
            if (target == current->value) return true;
            if (target < current->value) current = current->left;
            else current = current->right;
        }
        return false;
    }

    bool remove(const T& value) {
        bool removed = false;
        root = remove_helper(root, value, removed);
        if (removed) --tree_size;
        return removed;
    }

    const T& min() const {
        if (empty()) throw std::underflow_error("Tree is empty!");
        return find_min(root)->value;
    }

    const T& max() const {
        if (empty()) throw std::underflow_error("Tree is empty!");
        return find_max(root)->value;
    }

    // =========================================================================
    // Tree Traversals
    // =========================================================================

    /**
     * @brief In-Order Traversal (Left, Root, Right) -> Yields sorted order.
     */
    void in_order(const std::function<void(const T&)>& visitor) const {
        in_order_traverse(root, visitor);
    }

    /**
     * @brief Pre-Order Traversal (Root, Left, Right).
     */
    void pre_order(const std::function<void(const T&)>& visitor) const {
        pre_order_traverse(root, visitor);
    }

    /**
     * @brief Post-Order Traversal (Left, Right, Root).
     */
    void post_order(const std::function<void(const T&)>& visitor) const {
        post_order_traverse(root, visitor);
    }

    /**
     * @brief Level-Order Traversal (Breadth-First Search using std::queue).
     */
    void level_order(const std::function<void(const T&)>& visitor) const {
        if (!root) return;

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            visitor(current->value);

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

    // =========================================================================
    // Serialization & Inspection Utilities
    // =========================================================================

    /**
     * @brief Exports elements to a std::vector via in-order traversal.
     */
    std::vector<T> to_vector() const {
        std::vector<T> result;
        result.reserve(tree_size);
        in_order([&result](const T& val) {
            result.push_back(val);
        });
        return result;
    }

    /**
     * @brief String representation of the tree in sorted order.
     */
    std::string to_string() const {
        std::ostringstream ss;
        ss << "[";
        bool first = true;
        in_order([&ss, &first](const T& val) {
            if (!first) ss << ", ";
            ss << val;
            first = false;
        });
        ss << "]";
        return ss.str();
    }
};

// =========================================================================
// Demonstrative Main Execution
// =========================================================================

int main() {
    std::cout << "--- 1. Tree Initialization & Insertion ---\n";
    BST<int> tree = {50, 30, 70, 20, 40, 60, 80};
    std::cout << "Tree elements (In-Order / Sorted): " << tree.to_string() << "\n";
    std::cout << "Size: " << tree.size() << " | Height: " << tree.height() << "\n\n";

    std::cout << "--- 2. Min & Max Queries ---\n";
    std::cout << "Minimum value: " << tree.min() << "\n";
    std::cout << "Maximum value: " << tree.max() << "\n\n";

    std::cout << "--- 3. Search Operations ---\n";
    int search_target = 40;
    std::cout << "Searching for " << search_target << ": " 
              << (tree.search(search_target) ? "Found" : "Not Found") << "\n";

    search_target = 100;
    std::cout << "Searching for " << search_target << ": " 
              << (tree.search(search_target) ? "Found" : "Not Found") << "\n\n";

    std::cout << "--- 4. Traversals ---\n";
    std::cout << "Pre-Order:  ";
    tree.pre_order([](const int& val) { std::cout << val << " "; });
    std::cout << "\n";

    std::cout << "Post-Order: ";
    tree.post_order([](const int& val) { std::cout << val << " "; });
    std::cout << "\n";

    std::cout << "Level-Order (BFS): ";
    tree.level_order([](const int& val) { std::cout << val << " "; });
    std::cout << "\n\n";

    std::cout << "--- 5. Deletion ---\n";
    std::cout << "Removing node with 2 children (50)...\n";
    tree.remove(50);
    std::cout << "Tree after removal: " << tree.to_string() << "\n";
    std::cout << "New Root successor integrated. Size: " << tree.size() << "\n";

    return 0;
}
