#include <iostream>
#include "HashTableList.hpp"

int main() {
    std::cout << "=== SD-PROJEKT-3: Slownik oparty na Tablicy Mieszajacej ===\n\n";

    // Prosta demonstracja działania HashTableList
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

    ht.insert(1, 999); // aktualizacja wartości
    std::cout << "\nPo insert(1, 999) [aktualizacja]:\n";
    std::cout << "get(1) = " << ht.get(1) << "\n";

    ht.remove(17);
    std::cout << "\nPo remove(17):\n";
    ht.display();

    std::cout << "\nLiczba elementow: " << ht.size() << "\n";
    std::cout << "contains(5): "  << (ht.contains(5)  ? "tak" : "nie") << "\n";
    std::cout << "contains(17): " << (ht.contains(17) ? "tak" : "nie") << "\n";

    return 0;
}
