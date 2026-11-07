// input.hpp - defensive console input helpers.
// Plain `cin >> int` loops forever on non-numeric input and on EOF; these don't.
#pragma once

#include <iostream>
#include <limits>
#include <optional>
#include <string>

// Reads an int in [lo, hi]. Re-prompts on bad input; returns nullopt on EOF.
inline std::optional<int> read_int(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::cout << prompt;
        int value;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (value >= lo && value <= hi) return value;
            std::cout << "Please enter a number between " << lo << " and " << hi << ".\n";
            continue;
        }
        if (std::cin.eof()) return std::nullopt;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "That is not a number. Try again.\n";
    }
}
