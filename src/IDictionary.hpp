#pragma once

// ─── IDictionary ──────────────────────────────────────────────────────────────
//
// Minimalny interfejs wspólny dla wszystkich słowników (key → value).
// Wymagane operacje do benchmarku: insert, remove, clear.
//
struct IDictionary {
    virtual ~IDictionary() = default;
    virtual void insert(int key, int value) = 0;
    virtual void remove(int key)            = 0;
    virtual void clear()                    = 0;
};
