#pragma once
#include <string>
#include <random>

inline std::string gen_id() {
    static std::mt19937_64 rng{std::random_device{}()};
    static const char* chars = "0123456789abcdef";
    std::uniform_int_distribution<int> dist(0,15);
    std::string s(16, '0');
    for (char& c : s) c = chars[dist(rng)];
    return s;
}
