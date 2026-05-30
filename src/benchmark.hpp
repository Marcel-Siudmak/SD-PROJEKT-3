#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "data_handler.hpp"

using BenchmarkResults = std::map<std::string, std::map<int, double>>;

// ============================================================
//  Liczba kopii słownika tworzonych dla każdego seeda.
//  Większa wartość → dokładniejszy pomiar dla krótkich operacji.
// ============================================================
static constexpr int NUM_COPIES = 10;

// Zakres kluczy używanych przy generowaniu danych
static constexpr int KEY_MIN = 1;
static constexpr int KEY_MAX = 2'000'000;

// ─── result_exporter
// ──────────────────────────────────────────────────────────
//
// Zapis przyrostowy: jeden wiersz CSV natychmiast po zmierzeniu punktu.
// Format: operacja;n;czas_ns
//
class result_exporter {
public:
  static std::ofstream open_csv(const std::string &dataset_name,
                                const std::string &structure_name);

  static void append_row(std::ofstream &file, const std::string &operation,
                         int n, double time_ns);

  // Zachowana dla zgodności wstecznej
  static void export_to_csv(const BenchmarkResults &results,
                            const std::string &dataset_name,
                            const std::string &structure_name);
};

// ─── benchmark
// ────────────────────────────────────────────────────────────────

class benchmark {
public:
  explicit benchmark(const std::string &dataset_name);

  // ============================================================
  //  Główna metoda uruchamiająca testy dla jednej struktury.
  //
  //  DictType     – konkretny typ słownika (np. HashTableList)
  //  dict_factory – lambda () → DictType* tworząca pustą instancję
  //
  //  Mierzone operacje:
  //    insert(key, value) – wstawia losową, nową parę
  //    remove(key)        – usuwa losowo wybrany istniejący klucz
  //
  //  Procedura pomiaru (taka sama jak w poprzednim projekcie):
  //    Dla każdego seeda:
  //      1. Utwórz NUM_COPIES identycznych kopii słownika z n par.
  //      2. Uruchom zegarek.
  //      3. Wykonaj operację na każdej kopii.
  //      4. Zatrzymaj zegarek.
  //      5. Wynik dla seeda = czas_łączny / NUM_COPIES.
  //    Wynik końcowy = średnia ze wszystkich seedów.
  // ============================================================
  template <typename DictType>
  void run_structure_tests(const std::string &structure_name,
                           std::function<DictType *()> dict_factory) {

    std::ofstream csv =
        result_exporter::open_csv(dataset_.get_name(), structure_name);

    for (int n : dataset_.get_points()) {
      std::cout << "\n--- Testing " << structure_name << " for N=" << n
                << " ---\n";

      const auto &test_files = dataset_.get_test_files();
      const size_t num_seeds = test_files.size();

      std::vector<double> insert_times(num_seeds);
      std::vector<double> remove_times(num_seeds);

      for (size_t s = 0; s < num_seeds; ++s) {
        const std::string &file = test_files[s];
        const unsigned int seed = dataset_.get_file_seed(file);

        // --------------------------------------------------------
        // Wczytaj pary z pliku – potrzebne do:
        //   • załadowania kopii słownika
        //   • wylosowania klucza do remove()
        // --------------------------------------------------------
        auto pairs = dataset_.load_pairs(file, n); // vector<pair<int,int>>

        // --------------------------------------------------------
        // 1. insert()
        //
        //    Każda kopia słownika zawiera n par z pliku.
        //    Wstawiamy jeden dodatkowy element o kluczu wylosowanym
        //    deterministycznie z zakresu [KEY_MIN, KEY_MAX].
        //    Klucz jest inny dla każdej kopii, ale powtarzalny
        //    (RNG seed = file_seed).
        // --------------------------------------------------------
        {
          std::mt19937 rng(seed);
          std::uniform_int_distribution<int> key_dist(KEY_MIN, KEY_MAX);

          // Wylosuj NUM_COPIES kluczy do wstawienia (poza pomiarem)
          std::vector<int> ins_keys(NUM_COPIES);
          for (int &k : ins_keys)
            k = key_dist(rng);

          constexpr int INSERT_VALUE = 42;

          auto copies = make_copies<DictType>(dict_factory, pairs, NUM_COPIES);

          auto t0 = std::chrono::high_resolution_clock::now();
          for (size_t c = 0; c < copies.size(); ++c)
            copies[c]->insert(ins_keys[c], INSERT_VALUE);
          auto t1 = std::chrono::high_resolution_clock::now();

          insert_times[s] = ns(t0, t1) / NUM_COPIES;
          free_copies(copies);
        }

        // --------------------------------------------------------
        // 2. remove()
        //
        //    Każda kopia słownika zawiera n par z pliku.
        //    Usuwamy jeden istniejący klucz wylosowany deterministycznie
        //    ze zbioru kluczy załadowanych do słownika.
        //    RNG seed = file_seed ^ 0xDEADBEEFu (inny strumień niż insert).
        // --------------------------------------------------------
        {
          if (pairs.empty()) {
            remove_times[s] = 0.0;
          } else {
            std::mt19937 rng(seed ^ 0xDEADBEEFu);
            std::uniform_int_distribution<int> idx_dist(
                0, static_cast<int>(pairs.size()) - 1);

            // Wylosuj NUM_COPIES indeksów kluczy do usunięcia (poza pomiarem)
            std::vector<int> rem_keys(NUM_COPIES);
            for (int &k : rem_keys)
              k = pairs[idx_dist(rng)].first;

            auto copies =
                make_copies<DictType>(dict_factory, pairs, NUM_COPIES);

            auto t0 = std::chrono::high_resolution_clock::now();
            for (size_t c = 0; c < copies.size(); ++c)
              copies[c]->remove(rem_keys[c]);
            auto t1 = std::chrono::high_resolution_clock::now();

            remove_times[s] = ns(t0, t1) / NUM_COPIES;
            free_copies(copies);
          }
        }

      } // koniec pętli po seedach

      // --------------------------------------------------------
      // Uśrednij i zapisz do CSV
      // --------------------------------------------------------
      double avg_insert = average(insert_times);
      double avg_remove = average(remove_times);

      result_exporter::append_row(csv, "insert", n, avg_insert);
      result_exporter::append_row(csv, "remove", n, avg_remove);
      csv.flush();

      std::cout << "  insert avg = " << avg_insert << " ns\n";
      std::cout << "  remove avg = " << avg_remove << " ns\n";

    } // koniec pętli po punktach pomiarowych

    csv.close();
    std::cout << "\nWyniki zapisane do results/" << dataset_.get_name() << "/"
              << structure_name << ".csv\n";
  }

private:
  data_set dataset_;

  // ----------------------------------------------------------
  //  Tworzy `count` identycznych kopii słownika załadowanych
  //  z wektora par (klucz, wartość).
  // ----------------------------------------------------------
  template <typename DictType>
  std::vector<IDictionary *>
  make_copies(std::function<DictType *()> factory,
              const std::vector<std::pair<int, int>> &pairs, int count) {
    std::vector<IDictionary *> copies(count);
    for (int i = 0; i < count; ++i) {
      copies[i] = factory();
      copies[i]->clear();
      for (const auto &[k, v] : pairs)
        copies[i]->insert(k, v);
    }
    return copies;
  }

  static void free_copies(std::vector<IDictionary *> &copies) {
    for (auto *ptr : copies)
      delete ptr;
    copies.clear();
  }

  static double average(const std::vector<double> &v) {
    if (v.empty())
      return 0.0;
    double sum = 0.0;
    for (double x : v)
      sum += x;
    return sum / static_cast<double>(v.size());
  }

  static double ns(std::chrono::high_resolution_clock::time_point t0,
                   std::chrono::high_resolution_clock::time_point t1) {
    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
  }
};
