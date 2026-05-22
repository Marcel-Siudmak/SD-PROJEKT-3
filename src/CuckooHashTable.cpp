#include "CuckooHashTable.hpp"
#include <iostream>
#include <stdexcept>

// ─── Konstruktor / Destruktor ─────────────────────────────────────────────────

CuckooHashTable::CuckooHashTable(int capacity)
    : capacity_(capacity), count_(0), table1_(nullptr), table2_(nullptr)
{
    allocate_tables(capacity_);
}

CuckooHashTable::~CuckooHashTable() {
    free_tables();
}

// ─── Alokacja / dealokacja tablic ────────────────────────────────────────────

void CuckooHashTable::allocate_tables(int cap) {
    capacity_ = cap;
    table1_ = new Cell[cap];
    table2_ = new Cell[cap];
    // Cell() jest domyślnie konstruowany (occupied = false)
}

void CuckooHashTable::free_tables() {
    delete[] table1_;
    delete[] table2_;
    table1_ = nullptr;
    table2_ = nullptr;
}

// ─── Funkcje haszujące ────────────────────────────────────────────────────────
//
// Dobre h1 i h2 muszą być od siebie niezależne.
//   h1  –  klasyczny multiplicative hash (stała Knutha)
//   h2  –  FNV-inspired XOR-shift, inny multiplikator
//
// Oba zapewniają obsługę kluczy ujemnych przez rzutowanie na unsigned.

int CuckooHashTable::h1(int key) const {
    unsigned int k = static_cast<unsigned int>(key);
    k = (k ^ (k >> 16)) * 0x45d9f3bU;
    k = (k ^ (k >> 16)) * 0x45d9f3bU;
    k ^= (k >> 16);
    return static_cast<int>(k % static_cast<unsigned int>(capacity_));
}

int CuckooHashTable::h2(int key) const {
    unsigned int k = static_cast<unsigned int>(key);
    k = (k ^ (k >> 14)) * 0x9e3779b9U;
    k = (k ^ (k >> 14)) * 0x9e3779b9U;
    k ^= (k >> 14);
    return static_cast<int>(k % static_cast<unsigned int>(capacity_));
}

// ─── insert() ─────────────────────────────────────────────────────────────────
//
// Algorytm:
//   1. Sprawdź aktualizację (klucz już istnieje w table1 lub table2).
//   2. Sprawdź współczynnik zapełnienia → rehash prewencyjny.
//   3. Pętla wyrzuceń (max MAX_KICKS):
//      a) Wstaw do table1[h1(key)] – jeśli wolne, koniec.
//         Jeśli zajęte, zamień miejscami (wyrzuć starego lokatora).
//      b) Wstaw wyrzucony element do table2[h2(key)] – jeśli wolne, koniec.
//         Jeśli zajęte, zamień i kontynuuj pętlę z table1.
//   4. Jeśli pętla się wyczerpała → cykl → rehash i ponów insert.

void CuckooHashTable::insert(int key, int value) {
    // 1. Aktualizacja istniejącego klucza
    int pos1 = h1(key);
    if (table1_[pos1].occupied && table1_[pos1].key == key) {
        table1_[pos1].value = value;
        return;
    }
    int pos2 = h2(key);
    if (table2_[pos2].occupied && table2_[pos2].key == key) {
        table2_[pos2].value = value;
        return;
    }

    // 2. Prewencyjny rehash przy wysokim wypełnieniu
    if (count_ >= static_cast<int>(capacity_ * MAX_LOAD * 2)) {
        rehash();
        insert(key, value); // ponów po rehash
        return;
    }

    // 3. Pętla wyrzuceń Cuckoo
    int cur_key   = key;
    int cur_value = value;

    for (int kick = 0; kick < MAX_KICKS; ++kick) {
        // Próba w table1
        int p1 = h1(cur_key);
        if (!table1_[p1].occupied) {
            table1_[p1].key      = cur_key;
            table1_[p1].value    = cur_value;
            table1_[p1].occupied = true;
            ++count_;
            return;
        }
        // Wyrzuć lokatora z table1
        int evicted_key   = table1_[p1].key;
        int evicted_value = table1_[p1].value;
        table1_[p1].key      = cur_key;
        table1_[p1].value    = cur_value;
        // table1_[p1].occupied pozostaje true
        cur_key   = evicted_key;
        cur_value = evicted_value;

        // Próba w table2
        int p2 = h2(cur_key);
        if (!table2_[p2].occupied) {
            table2_[p2].key      = cur_key;
            table2_[p2].value    = cur_value;
            table2_[p2].occupied = true;
            ++count_;
            return;
        }
        // Wyrzuć lokatora z table2
        evicted_key   = table2_[p2].key;
        evicted_value = table2_[p2].value;
        table2_[p2].key      = cur_key;
        table2_[p2].value    = cur_value;
        // table2_[p2].occupied pozostaje true
        cur_key   = evicted_key;
        cur_value = evicted_value;
    }

    // 4. Cykl – rehash i ponów
    rehash();
    insert(cur_key, cur_value);
}

// ─── remove() ─────────────────────────────────────────────────────────────────

void CuckooHashTable::remove(int key) {
    int p1 = h1(key);
    if (table1_[p1].occupied && table1_[p1].key == key) {
        table1_[p1].occupied = false;
        --count_;
        return;
    }
    int p2 = h2(key);
    if (table2_[p2].occupied && table2_[p2].key == key) {
        table2_[p2].occupied = false;
        --count_;
        return;
    }
    // Klucz nie istnieje – cicha operacja
}

// ─── get() ────────────────────────────────────────────────────────────────────

int CuckooHashTable::get(int key) const {
    int p1 = h1(key);
    if (table1_[p1].occupied && table1_[p1].key == key)
        return table1_[p1].value;

    int p2 = h2(key);
    if (table2_[p2].occupied && table2_[p2].key == key)
        return table2_[p2].value;

    throw std::out_of_range("CuckooHashTable::get – klucz nie istnieje");
}

// ─── contains() ──────────────────────────────────────────────────────────────

bool CuckooHashTable::contains(int key) const {
    int p1 = h1(key);
    if (table1_[p1].occupied && table1_[p1].key == key)
        return true;

    int p2 = h2(key);
    return table2_[p2].occupied && table2_[p2].key == key;
}

// ─── size() ──────────────────────────────────────────────────────────────────

int CuckooHashTable::size() const {
    return count_;
}

// ─── rehash() ─────────────────────────────────────────────────────────────────
//
// Podwaja rozmiar tablic i reinseruje wszystkie istniejące elementy.
// Operacja jest O(n) amortyzowana.

void CuckooHashTable::rehash() {
    int   old_cap  = capacity_;
    Cell* old_t1   = table1_;
    Cell* old_t2   = table2_;

    // Alokuj nowe, puste tablice o podwojonej pojemności
    table1_ = nullptr;
    table2_ = nullptr;
    count_  = 0;
    allocate_tables(old_cap * 2);

    // Reinseruj wszystkie elementy ze starych tablic
    for (int i = 0; i < old_cap; ++i) {
        if (old_t1[i].occupied)
            insert(old_t1[i].key, old_t1[i].value);
        if (old_t2[i].occupied)
            insert(old_t2[i].key, old_t2[i].value);
    }

    delete[] old_t1;
    delete[] old_t2;
}

// ─── display() ───────────────────────────────────────────────────────────────

void CuckooHashTable::display() const {
    std::cout << "  Tablica 1 (h1):\n";
    for (int i = 0; i < capacity_; ++i) {
        if (table1_[i].occupied) {
            std::cout << "    [" << i << "] "
                      << table1_[i].key << " -> " << table1_[i].value << "\n";
        }
    }
    std::cout << "  Tablica 2 (h2):\n";
    for (int i = 0; i < capacity_; ++i) {
        if (table2_[i].occupied) {
            std::cout << "    [" << i << "] "
                      << table2_[i].key << " -> " << table2_[i].value << "\n";
        }
    }
}
