# Miniprojekt 3: Słownik oparty na Tablicy Mieszającej

Projekt realizowany w ramach przedmiotu Struktury Danych.

---

1. **HashTableList** – Kubełki oparte na liście wiązanej.
2. **CuckooHashTable** – Cuckoo hashing
3. **HashTableAVL** – Kubełki oparte na zbalansowanym drzewie AVL.

**Badane operacje:** `insert()` oraz `remove()`.
**Typ danych:** Zarówno klucze, jak i wartości są liczbami całkowitymi (`int`).

---

## 📁 Struktura Plików w Projekcie

```text
SłownikTablicaMieszajaca/
├── 📁 src/                         # Wszystkie pliki kodu źródłowego razem
│   ├── IDictionary.hpp             # Wspólny interfejs dla słowników (klasa abstrakcyjna)
│   │
│   ├── LinkedList.hpp              # Klasa listy wiązanej (nagłówek)
│   ├── LinkedList.cpp              # Klasa listy wiązanej (implementacja)
│   │
│   ├── AVLTree.hpp                 # Klasa zbalansowanego drzewa AVL (nagłówek)
│   ├── AVLTree.cpp                 # Klasa zbalansowanego drzewa AVL (implementacja)
│   │
│   ├── HashTableList.hpp           # Tablica mieszająca + lista (nagłówek)
│   ├── HashTableList.cpp           # Tablica mieszająca + lista (implementacja)
│   │
│   ├── CuckooHashTable.hpp         # Tablica mieszająca typu Cuckoo (nagłówek)
│   ├── CuckooHashTable.cpp         # Tablica mieszająca typu Cuckoo (implementacja)
│   │
│   ├── HashTableAVL.hpp            # Tablica mieszająca + AVL (nagłówek)
│   ├── HashTableAVL.cpp            # Tablica mieszająca + AVL (implementacja)
│   │
│   └── main.cpp                    # 
│
└── CMakeLists.txt                  # Plik konfiguracyjny do budowania projektu