#include "HashTableAVL.hpp"
#include <functional>

template <typename T>
int AVLTree<T>::height(AVLNode<T>* node) const {
    return node ? node->_height : 0;
}

template <typename T>
int AVLTree<T>::getBalance(AVLNode<T>* node) const {
    return node ? height(node->left) - height(node->right) : 0;
}
/* murarz-malarz-tynkarz-akrobata by tak ladnie nie narysowal
      y
    x/  \T2
 T0/ \T1

*/
template <typename T>
AVLNode<T>* AVLTree<T>::rightRotate(AVLNode<T>* y) {
    AVLNode<T>* x = y->left;
    AVLNode<T>* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->_height = 1 + std::max(height(y->left), height(y->right));
    x->_height = 1 + std::max(height(x->left), height(x->right));

    // Return new root
    return x;
}


template <typename T>
AVLNode<T>* AVLTree<T>::leftRotate(AVLNode<T>* x) {
    AVLNode<T>* y = x->right;
    AVLNode<T>* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->_height = 1 + std::max(height(x->left), height(x->right));
    y->_height = 1 + std::max(height(y->left), height(y->right));

    // Return new root
    return y;
}

template <typename T>
AVLNode<T>* AVLTree<T>::insertNode(AVLNode<T>* node, int key, T value) {
    // 1. Perform the normal BST insert
    if (!node) {
        node_count++;
        return new AVLNode<T>(key, value);
    }

    if (key < node->_key)
        node->left = insertNode(node->left, key, value);
    else if (key > node->_key)
        node->right = insertNode(node->right, key, value);
    else {
        // If the key already exists, update the value
        node->_value = value;
        return node;
    }

    // 2. Update height of this ancestor node
    node->_height = 1 + std::max(height(node->left), height(node->right));

    // 3. Get the balance factor to check whether this ancestor node became unbalanced
    int balance = getBalance(node);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && key < node->left->_key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->_key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->_key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->_key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the unchanged ancestor node pointer
    return node;
}

template <typename T>
AVLNode<T>* AVLTree<T>::removeNode(AVLNode<T>* node, int key) {
    // STEP 1: PERFORM STANDARD BST DELETE
    if (!node)
        return node;

    if (key < node->_key)
        node->left = removeNode(node->left, key);
    else if (key > node->_key)
        node->right = removeNode(node->right, key);
    else {
        // node with only one child or no child
        if (!node->left || !node->right) {
            AVLNode<T>* temp = node->left ? node->left : node->right;

            // No child case
            if (!temp) {
                temp = node;
                node = nullptr;
            } else // One child case
                *node = *temp; // Copy the contents of the non-empty child

            delete temp;
            node_count--;
        } else {
            // node with two children: Get the inorder successor (smallest in the right subtree)
            AVLNode<T>* temp = minValueNode(node->right);

            // Copy the inorder successor's content to this node
            node->_key = temp->_key;
            node->_value = temp->_value;

            // Delete the inorder successor
            node->right = removeNode(node->right, temp->_key);
        }
    }

    // If the tree had only one node then return
    if (!node)
        return node;

    // STEP 2: UPDATE HEIGHT OF THIS ANCESTOR NODE
    node->_height = 1 + std::max(height(node->left), height(node->right));

    // STEP 3: GET THE BALANCE FACTOR OF THIS ANCESTOR NODE TO
    // CHECK WHETHER THIS ANCESTOR NODE BECAME UNBALANCED
    int balance = getBalance(node);

    // If this ancestor node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);
    
    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    // Return the (potentially) updated node pointer
    return node;
}

template <typename T>
AVLNode<T>* AVLTree<T>::minValueNode(AVLNode<T>* node) const {
    AVLNode<T>* current = node;

    // Loop down to find the leftmost leaf
    while (current->left)
        current = current->left;

    return current;
}

template <typename T>
void AVLTree<T>::clearNode(AVLNode<T>* node) {
    if (node) {
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }
}

template <typename T>
void AVLTree<T>::insert(int key, T value) {
    root = insertNode(root, key, value);
}

template <typename T>
void AVLTree<T>::remove(int key) {
    root = removeNode(root, key);
}

template <typename T>
void AVLTree<T>::clear() {
    clearNode(root);
    root = nullptr;
    node_count = 0;
}

template <typename T>
T AVLTree<T>::get(int key) const {
    AVLNode<T>* current = root;
    while (current) {
        if (key < current->_key)
            current = current->left;
        else if (key > current->_key)
            current = current->right;
        else
            return current->_value; // Key found
    }
    throw std::out_of_range("AVLTree::get - klucz nie istnieje");
}

template <typename T>
bool AVLTree<T>::contains(int key) const {
    AVLNode<T>* current = root;
    while (current) {
        if (key < current->_key)
            current = current->left;
        else if (key > current->_key)
            current = current->right;
        else
            return true; // Key found
    }
    return false; // Key not found
}

template <typename T>
void AVLTree<T>::display() const {
    // Simple in-order traversal to display the tree
    std::function<void(AVLNode<T> *) > inOrder = [&](AVLNode<T>* node) {
        if (node) {
            inOrder(node->left);
            std::cout << "(" << node->_key << ": " << node->_value << ") ";
            inOrder(node->right);
        }
    };
    inOrder(root);
    std::cout << std::endl;
}


//-------------------------------------------------

template <typename T>
void AVLHashTable<T>::insert(int key, T value) {
    if (static_cast<float>(_size + 1) / _capacity > _load_threshold) {
        rehash();
    }
    buckets[hash(key)].insert(key, value);
    _size++;
}

template <typename T>
void AVLHashTable<T>::remove(int key) {
    if (contains(key)) {
        buckets[hash(key)].remove(key);
        _size--;
    }
}

template <typename T>
void AVLHashTable<T>::clear() {
    for (int i = 0; i < _capacity; i++) {
        buckets[i].clear();
    }
    _size = 0;
}

template <typename T>
bool AVLHashTable<T>::contains(int key) const {
    return buckets[hash(key)].contains(key);
}

template <typename T>
void AVLHashTable<T>::display() const {
    for (int i = 0; i < _capacity; i++) {
        std::cout << "Bucket " << i << ": "<<std::endl;
        buckets[i].display();
    }
    std::cout<<std::endl;
}

template <typename T>
void AVLHashTable<T>::rehash() {
    int old_capacity = _capacity;
    _capacity *= static_cast<int>(_load_threshold);
    AVLTree<T>* new_buckets = new AVLTree<T>[_capacity];

    for (int i = 0; i < old_capacity; i++) {
        // We need to rehash all elements from the old bucket
        std::function<void(AVLNode<T> *) > rehashNodes = [&](AVLNode<T>* node) {
            if (node) {
                rehashNodes(node->left);
                rehashNodes(node->right);
                new_buckets[hash(node->_key)].insert(node->_key, node->_value);
            }
        };
        rehashNodes(buckets[i].root);
    }

    delete[] buckets;
    buckets = new_buckets;
}

//Explicit template instantiation for int values
template class AVLTree<int>;
template class AVLHashTable<int>;