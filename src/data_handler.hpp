#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <random>

#include "IDictionary.hpp"

// ─── data_set ─────────────────────────────────────────────────────────────────

class data_set {
public:
  explicit data_set(const std::string &dataset_name);

  std::string get_name() const;
  std::vector<int> get_points() const;
  std::vector<std::string> get_test_files() const;

  // Parsuje seed z nazwy pliku (np. "3748291234.txt" → 3748291234)
  unsigned int get_file_seed(const std::string &test_file) const;

  // Wczytuje pierwsze num_elements par (klucz, wartość) do słownika dict.
  // Format wiersza w pliku: "klucz,wartość"
  void load_to_dict(const std::string &test_file, int num_elements,
                    IDictionary &dict) const;

  // Wczytuje pierwsze num_elements par jako wektor – do użytku przez benchmark
  // przy wyborze klucza do remove().
  std::vector<std::pair<int,int>> load_pairs(const std::string &test_file,
                                             int num_elements) const;

private:
  std::string name_;
  std::vector<int> points_;
  std::vector<std::string> test_files_;
};

// ─── data_handler ─────────────────────────────────────────────────────────────

class data_handler {
public:
  // Generuje dataset: num_files plików, każdy z max(points) par (key,value).
  // Format wiersza: "klucz,wartość"
  static void generate_dataset(const std::string &dataset_name,
                               const std::vector<int> &points, int num_files,
                               unsigned int main_seed);

  static void delete_dataset(const std::string &dataset_name);

  static std::vector<std::string> list_datasets();
};
