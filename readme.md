# Miniprojekt 3: Słownik oparty na Tablicy Mieszającej

Projekt realizowany w ramach przedmiotu Struktury Danych.

---

1. **HashTableList** – Kubełki oparte na liście wiązanej.
2. **HashTableBST** – Kubełki oparte na zwykłym drzewe przeszukiwań binarnych (BST).
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
│   ├── BSTree.hpp                  # Klasa zwykłego drzewa BST (nagłówek)
│   ├── BSTree.cpp                  # Klasa zwykłego drzewa BST (implementacja)
│   │
│   ├── AVLTree.hpp                 # Klasa zbalansowanego drzewa AVL (nagłówek)
│   ├── AVLTree.cpp                 # Klasa zbalansowanego drzewa AVL (implementacja)
│   │
│   ├── HashTableList.hpp           # Tablica mieszająca + lista (nagłówek)
│   ├── HashTableList.cpp           # Tablica mieszająca + lista (implementacja)
│   │
│   ├── HashTableBST.hpp            # Tablica mieszająca + BST (nagłówek)
│   ├── HashTableBST.cpp            # Tablica mieszająca + BST (implementacja)
│   │
│   ├── HashTableAVL.hpp            # Tablica mieszająca + AVL (nagłówek)
│   ├── HashTableAVL.cpp            # Tablica mieszająca + AVL (implementacja)
│   │
│   └── main.cpp                    # Generowanie danych, pomiary chrono, menu
│
└── CMakeLists.txt                  # Plik konfiguracyjny do budowania projektu