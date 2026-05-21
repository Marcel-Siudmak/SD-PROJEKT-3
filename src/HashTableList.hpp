#pragma once
#include <iostream>
#include "singly_linked_list.hpp"

// Para klucz-wartość przechowywana w kubełkach
struct KeyValue {
    int key;
    int value;

    // Wymagane do singly_linked_list::find() - porównuje po kluczu
    bool operator==(const KeyValue& other) const { return key == other.key; }

    // Wymagane przez singly_linked_list<KeyValue>::display()
    friend std::ostream& operator<<(std::ostream& os, const KeyValue& kv) {
        return os << "(" << kv.key << "->" << kv.value << ")";
    }
};

// Tablica mieszająca z kubełkami opartymi na liście wiązanej.
// Rozwiązuje kolizje metodą łańcuchową (separate chaining).
class HashTableList {
private:
    static const int DEFAULT_CAPACITY = 16;

    int capacity;                         // Liczba kubełków
    singly_linked_list<KeyValue>** buckets; // Tablica wskaźników na listy

    // Funkcja mieszająca – mapuje klucz na indeks kubełka
    int hash(int key) const;

public:
    explicit HashTableList(int capacity = DEFAULT_CAPACITY);
    ~HashTableList();

    // Wstawia parę (key, value). Jeśli klucz już istnieje – aktualizuje wartość.
    void insert(int key, int value);

    // Usuwa parę o podanym kluczu. Nic nie robi, jeśli klucz nie istnieje.
    void remove(int key);

    // Zwraca wartość dla klucza. Rzuca std::out_of_range jeśli klucza nie ma.
    int get(int key) const;

    // Sprawdza czy klucz istnieje w słowniku
    bool contains(int key) const;

    // Wypisuje zawartość tablicy (do debugowania / menu)
    void display() const;

    // Zwraca liczbę przechowywanych par
    int size() const;
};
