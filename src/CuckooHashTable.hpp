#pragma once
#include <iostream>
#include "data_handler.hpp"  // IDictionary

// ─── CuckooHashTable ──────────────────────────────────────────────────────────
//
// Słownik oparty na Cuckoo hashing:
//   • Dwie niezależne tablice (table1, table2) o tym samym rozmiarze.
//   • Dwie funkcje haszujące: h1 i h2.
//   • insert() – cykliczne "wyrzucanie" elementów między tablicami.
//   • get() / remove() – O(1) w najgorszym przypadku (max 2 sprawdzenia).
//   • Rehash następuje gdy współczynnik wypełnienia > MAX_LOAD lub gdy
//     insert() wpadnie w cykl (MAX_KICKS wyrzuceń bez wolnego miejsca).
//
// Klucze i wartości są liczbami całkowitymi (int).
// Brak zewnętrznych zależności poza <iostream> i <stdexcept>.
// ─────────────────────────────────────────────────────────────────────────────

class CuckooHashTable : public IDictionary {
public:
    // Domyślny rozmiar jednej tablicy (łączna pojemność = 2 * capacity)
    static const int DEFAULT_CAPACITY = 16;

    // Limit wyrzuceń przed uznaniem cyklu i uruchomieniem rehash
    static const int MAX_KICKS = 128;

    // Próg zapełnienia jednej tablicy triggujący profilaktyczny rehash
    static constexpr double MAX_LOAD = 0.49;

    explicit CuckooHashTable(int capacity = DEFAULT_CAPACITY);
    ~CuckooHashTable() override;

    // Kopiowanie wyłączone (tablice dynamiczne – upraszcza kod)
    CuckooHashTable(const CuckooHashTable&)            = delete;
    CuckooHashTable& operator=(const CuckooHashTable&) = delete;

    // Wstawia parę (key, value).
    // Jeśli klucz już istnieje – aktualizuje wartość (bez duplikatów).
    void insert(int key, int value) override;

    // Usuwa parę o podanym kluczu. Nic nie robi gdy klucz nie istnieje.
    void remove(int key) override;

    // Resetuje słownik (usuwa wszystkie pary) – wymagane przez IDictionary.
    void clear() override;

    // Zwraca wartość dla klucza. Rzuca std::out_of_range gdy brak klucza.
    int get(int key) const;

    // Sprawdza czy klucz istnieje w słowniku – O(1) worst-case.
    bool contains(int key) const;

    // Wypisuje zawartość obu tablic (do debugowania / menu).
    void display() const;

    // Liczba przechowywanych par klucz-wartość.
    int size() const;

private:
    // ─── Wewnętrzna komórka tablicy ──────────────────────────────────────────
    struct Cell {
        int  key;
        int  value;
        bool occupied; // czy komórka jest zajęta

        Cell() : key(0), value(0), occupied(false) {}
    };

    int   capacity_;  // rozmiar każdej z dwóch tablic
    int   count_;     // liczba przechowywanych par
    Cell* table1_;    // pierwsza tablica
    Cell* table2_;    // druga tablica

    // ─── Funkcje haszujące ───────────────────────────────────────────────────
    // h1 i h2 muszą być od siebie niezależne, by minimalizować cykle.
    int h1(int key) const;
    int h2(int key) const;

    // ─── Rehash ──────────────────────────────────────────────────────────────
    // Podwaja rozmiar tablic i reinseruje wszystkie elementy.
    void rehash();

    // ─── Alokacja / dealokacja ───────────────────────────────────────────────
    void allocate_tables(int cap);
    void free_tables();
};
