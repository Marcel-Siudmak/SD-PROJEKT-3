#include <iostream>
#include "HashTableList.hpp"
#include "CuckooHashTable.hpp"

// ─── Testy HashTableList ──────────────────────────────────────────────────────
static void test_hash_table_list() {
    std::cout << "=== HashTableList (Kubełki + Lista) ===\n\n";
    HashTableList ht;

    ht.insert(1,  100);
    ht.insert(17, 200); // 17 % 16 == 1 → kolizja z kluczem 1
    ht.insert(5,  50);
    ht.insert(-3, 30);  // klucz ujemny

    std::cout << "Po insert(1,100), insert(17,200), insert(5,50), insert(-3,30):\n";
    ht.display();

    std::cout << "\nget(1)  = " << ht.get(1)  << "\n";
    std::cout << "get(17) = " << ht.get(17) << "\n";
    std::cout << "get(-3) = " << ht.get(-3) << "\n";

    ht.insert(1, 999);
    std::cout << "\nPo insert(1, 999) [aktualizacja]: get(1) = " << ht.get(1) << "\n";

    ht.remove(17);
    std::cout << "\nPo remove(17):\n";
    ht.display();

    std::cout << "Liczba elementow: " << ht.size() << "\n";
    std::cout << "contains(5):  " << (ht.contains(5)  ? "tak" : "nie") << "\n";
    std::cout << "contains(17): " << (ht.contains(17) ? "tak" : "nie") << "\n";
}

// ─── Testy CuckooHashTable ────────────────────────────────────────────────────
static void test_cuckoo() {
    std::cout << "\n=== CuckooHashTable (Cuckoo Hashing) ===\n\n";
    CuckooHashTable ct;

    // Podstawowe wstawienia
    ct.insert(1,  100);
    ct.insert(2,  200);
    ct.insert(3,  300);
    ct.insert(-7, 77);   // klucz ujemny
    ct.insert(0,  0);    // klucz zero

    std::cout << "Po insert(1,100), (2,200), (3,300), (-7,77), (0,0):\n";
    ct.display();

    std::cout << "\nget(1)  = " << ct.get(1)  << "\n";
    std::cout << "get(2)  = " << ct.get(2)  << "\n";
    std::cout << "get(-7) = " << ct.get(-7) << "\n";
    std::cout << "get(0)  = " << ct.get(0)  << "\n";

    // Aktualizacja
    ct.insert(2, 999);
    std::cout << "\nPo insert(2, 999) [aktualizacja]: get(2) = " << ct.get(2) << "\n";

    // Usuwanie
    ct.remove(3);
    std::cout << "\nPo remove(3):\n";
    ct.display();
    std::cout << "contains(3):  " << (ct.contains(3)  ? "tak" : "nie") << "\n";
    std::cout << "contains(-7): " << (ct.contains(-7) ? "tak" : "nie") << "\n";

    // Stres-test: dużo elementów (sprawdza rehash)
    std::cout << "\nStres-test: wstawianie 200 elementow...\n";
    CuckooHashTable ct2;
    for (int i = 0; i < 200; ++i)
        ct2.insert(i * 7 + 3, i * 13); // rozróżnialne klucze

    bool all_ok = true;
    for (int i = 0; i < 200; ++i) {
        if (!ct2.contains(i * 7 + 3)) { all_ok = false; break; }
        if (ct2.get(i * 7 + 3) != i * 13) { all_ok = false; break; }
    }
    std::cout << "Wynik: " << (all_ok ? "POPRAWNY" : "BLAD") << "\n";
    std::cout << "Liczba elementow: " << ct2.size() << "\n";

    // Usuniecie wszystkich
    for (int i = 0; i < 200; ++i)
        ct2.remove(i * 7 + 3);
    std::cout << "Po usunieciu wszystkich, size = " << ct2.size() << "\n";
}

// ─── main ─────────────────────────────────────────────────────────────────────
int main() {
    std::cout << "=== SD-PROJEKT-3: Slownik oparty na Tablicy Mieszajacej ===\n\n";
    test_hash_table_list();
    test_cuckoo();
    return 0;
}
