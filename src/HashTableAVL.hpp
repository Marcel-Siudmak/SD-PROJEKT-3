#pragma once

#include "data_handler.hpp" // IDictionary

// Forward declarations
template <typename T>
class AVLTree;

template <typename T>
class AVLHashTable;

// AVL Node
template <typename T>
struct AVLNode {
    int _key;
    T _value;
    AVLNode* left;
    AVLNode* right;
    int _height;

    AVLNode(int k, T v) : _key(k), _value(v), left(nullptr), right(nullptr), _height(1) {}
    ~AVLNode() {
        delete left;
        delete right;
    }
friend class AVLTree<T>;
};

template <typename T>
class AVLTree : public IDictionary {
private:
    AVLNode<T>* root;
    int node_count;

    int height(AVLNode<T>* node) const;
    int getBalance(AVLNode<T>* node) const;
    AVLNode<T>* rightRotate(AVLNode<T>* y);
    AVLNode<T>* leftRotate(AVLNode<T>* x);
    AVLNode<T>* insertNode(AVLNode<T>* node, int key, T value);
    AVLNode<T>* removeNode(AVLNode<T>* node, int key);
    AVLNode<T>* minValueNode(AVLNode<T>* node) const;
    void clearNode(AVLNode<T>* node);

public:
    AVLTree() : root(nullptr), node_count(0) {}
    ~AVLTree() override { delete root; }

    void insert(int key, T value) override;
    void remove(int key) override;
    void clear() override;

    // Dodatkowe metody do testowania i debugowania
    T get(int key) const;
    bool contains(int key) const;
    void display() const;
    int size() const { return node_count; }
friend class AVLHashTable<T>;
};

//-----------------------------------------------------------------
template <typename T>
class AVLHashTable : public IDictionary {
private:
    int _capacity;
    int _size;
    AVLTree<T>* buckets;
    float _load_threshold;

    void rehash();
    int hash(int key) const { return key * 0x9e3779b9 % _capacity; } // Simple fibonacci hash

public:
    AVLHashTable(int capacity = 16, float threshold = 5.0f) : _capacity(capacity), _size(0), _load_threshold(threshold) {
        buckets = new AVLTree<T>[_capacity];
    }

    ~AVLHashTable() {
        delete[] buckets;
    }

    void insert(int key, T value) override;
    void remove(int key) override;
    void clear() override;

    bool contains(int key) const;
    void display() const;
};