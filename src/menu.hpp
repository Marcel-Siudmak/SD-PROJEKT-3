#pragma once
#include "HashTableAVL.hpp"
#include "CuckooHashTable.hpp"
#include "HashTableList.hpp"
#include "benchmark.hpp"
#include "data_handler.hpp"
#include <iostream>
#include <string>
#include <vector>

// ─── menu
// ─────────────────────────────────────────────────────────────────────
//
// Interaktywne menu do testowania słowników opartych na tablicach mieszających.
// Obsługuje:
//   • Generowanie / usuwanie datasetów
//   • Konfigurację parametrów benchmarku
//   • Uruchamianie benchmarków dla obu struktur
//
class menu {
private:
  std::string _dataset_name;
  std::vector<int> _points;
  int _num_files;
  unsigned int _main_seed;

public:
  menu()
      : _dataset_name("dict_benchmark"),
        _points({1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000, 256000,
                 512000}),
        _num_files(50), _main_seed(42) {}

  void display_menu() {
    std::cout << "\n=== SD-PROJEKT-3: Benchmarking Slownnikow ===\n";
    std::cout << "Aktualny dataset: " << _dataset_name << "\n";
    std::cout << "1. Generuj / odbuduj dataset\n";
    std::cout << "2. Zmien nazwe datasetu\n";
    std::cout << "3. Parametry benchmarku\n";
    std::cout << "4. Uruchom benchmark (oba slowniki)\n";
    std::cout << "5. Uruchom benchmark (tylko HashTableList)\n";
    std::cout << "6. Uruchom benchmark (tylko CuckooHashTable)\n";
    std::cout << "7. Uruchom benchmark (tylko AVLHashTable)\n";
    std::cout << "8. Wyjdz\n";
    std::cout << "Wybor: ";
  }

  void generate_dataset() {
    std::cout << "Generowanie datasetu '" << _dataset_name << "' ("
              << _num_files << " plikow, seed=" << _main_seed << ")...\n";
    data_handler::delete_dataset(_dataset_name);
    data_handler::generate_dataset(_dataset_name, _points, _num_files,
                                   _main_seed);
    std::cout << "Gotowe.\n";
  }

  void change_dataset() {
    std::cout << "Podaj nowa nazwe datasetu: ";
    std::cin >> _dataset_name;
  }

  void parameters() {
    std::cout << "Liczba plikow testowych [" << _num_files << "]: ";
    std::cin >> _num_files;

    std::cout << "Seed glowny [" << _main_seed << "]: ";
    std::cin >> _main_seed;

    std::cout
        << "Punkty pomiarowe (oddzielone przecinkiem, np. 1000,2000,4000): ";
    std::string input;
    std::cin >> input;

    _points.clear();
    size_t pos = 0;
    while ((pos = input.find(',')) != std::string::npos) {
      _points.push_back(std::stoi(input.substr(0, pos)));
      input.erase(0, pos + 1);
    }
    if (!input.empty())
      _points.push_back(std::stoi(input));

    std::cout << "Zapisano " << _points.size() << " punktow.\n";
  }

  void run_benchmarks_all() {
    run_hash_table_list();
    run_cuckoo();
    run_avl();
  }

  void run_hash_table_list() {
    std::cout << "\nUruchamianie benchmarku: HashTableList...\n";
    benchmark bench(_dataset_name);
    bench.run_structure_tests<HashTableList>(
        "HashTableList", []() { return new HashTableList(); });
  }

  void run_cuckoo() {
    std::cout << "\nUruchamianie benchmarku: CuckooHashTable...\n";
    benchmark bench(_dataset_name);
    bench.run_structure_tests<CuckooHashTable>(
        "CuckooHashTable", []() { return new CuckooHashTable(); });
  }

  void run_avl() {
    std::cout << "\nUruchamianie benchmarku: AVLHashTable...\n";
    benchmark bench(_dataset_name);
    bench.run_structure_tests<AVLHashTable<int>>(
        "AVLHashTable", []() { return new AVLHashTable<int>(); });
  }

  void run() {
    std::cout << "Witaj w narzedziu do benchmarkowania slownikow!\n";
    while (true) {
      display_menu();
      int choice;
      std::cin >> choice;

      switch (choice) {
      case 1:
        generate_dataset();
        break;
      case 2:
        change_dataset();
        break;
      case 3:
        parameters();
        break;
      case 4:
        run_benchmarks_all();
        break;
      case 5:
        run_hash_table_list();
        break;
      case 6:
        run_cuckoo();
        break;
      case 7:
        run_avl();
        break;
      case 8:
        std::cout << "Koniec.\n";
        return;
      default:
        std::cout << "Nieprawidlowy wybor.\n";
      }
    }
  }
};
