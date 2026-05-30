#include "HashTableList.hpp"
#include <iostream>
#include <stdexcept>

// ─── Konstruktor / Destruktor ─────────────────────────────────────────────────

HashTableList::HashTableList(int cap) : capacity(cap) {
    buckets = new singly_linked_list<KeyValue>*[capacity];
    for (int i = 0; i < capacity; i++) {
        buckets[i] = new singly_linked_list<KeyValue>();
    }
}

HashTableList::~HashTableList() {
    for (int i = 0; i < capacity; i++) {
        delete buckets[i];
    }
    delete[] buckets;
}

// ─── Funkcja mieszająca ───────────────────────────────────────────────────────

// Prosta funkcja mieszająca z obsługą kluczy ujemnych
int HashTableList::hash(int key) const {
    return ((key % capacity) + capacity) % capacity;
}

// ─── insert() ─────────────────────────────────────────────────────────────────

void HashTableList::insert(int key, int value) {
    int idx = hash(key);
    singly_linked_list<KeyValue>* bucket = buckets[idx];

    // Iterujemy po kubełku: jeśli klucz już istnieje – aktualizujemy wartość
    singly_node<KeyValue>* node = bucket->get_head();
    while (node != nullptr) {
        if (node->data.key == key) {
            node->data.value = value; // aktualizacja in-place
            return;
        }
        node = node->next;
    }

    // Klucz nie istnieje – dodajemy nową parę na przód kubełka (O(1))
    bucket->push_front({key, value});
}

// ─── remove() ─────────────────────────────────────────────────────────────────

void HashTableList::remove(int key) {
    int idx = hash(key);
    singly_linked_list<KeyValue>* bucket = buckets[idx];

    // Szukamy indeksu (pozycji) węzła z podanym kluczem
    singly_node<KeyValue>* node = bucket->get_head();
    int pos = 0;
    while (node != nullptr) {
        if (node->data.key == key) {
            bucket->remove(pos); // usuwamy węzeł na pozycji pos
            return;
        }
        node = node->next;
        pos++;
    }
    // Klucz nie istnieje – cicha operacja (zgodnie z kontraktem)
}

// ─── clear() ─────────────────────────────────────────────────────────────────────────────

void HashTableList::clear() {
    for (int i = 0; i < capacity; i++) {
        // Usunięcie wszystkich elementów z kubekła przez kolejne pop_front
        while (buckets[i]->get_size() > 0)
            buckets[i]->pop_front();
    }
}

// ─── get() ────────────────────────────────────────────────────────────────────

int HashTableList::get(int key) const {
    int idx = hash(key);
    singly_node<KeyValue>* node = buckets[idx]->get_head();
    while (node != nullptr) {
        if (node->data.key == key)
            return node->data.value;
        node = node->next;
    }
    throw std::out_of_range("HashTableList::get – klucz nie istnieje");
}

// ─── contains() ──────────────────────────────────────────────────────────────

bool HashTableList::contains(int key) const {
    int idx = hash(key);
    singly_node<KeyValue>* node = buckets[idx]->get_head();
    while (node != nullptr) {
        if (node->data.key == key)
            return true;
        node = node->next;
    }
    return false;
}

// ─── size() ──────────────────────────────────────────────────────────────────

int HashTableList::size() const {
    int total = 0;
    for (int i = 0; i < capacity; i++) {
        total += buckets[i]->get_size();
    }
    return total;
}

// ─── display() ───────────────────────────────────────────────────────────────

void HashTableList::display() const {
    for (int i = 0; i < capacity; i++) {
        if (buckets[i]->get_size() == 0)
            continue; // pomiń puste kubełki
        std::cout << "  [" << i << "] ";
        singly_node<KeyValue>* node = buckets[i]->get_head();
        while (node != nullptr) {
            std::cout << "(" << node->data.key << "->" << node->data.value << ")";
            if (node->next != nullptr)
                std::cout << " -> ";
            node = node->next;
        }
        std::cout << "\n";
    }
}
