#ifndef SHUFFLINGSTRATEGIES_HPP
#define SHUFFLINGSTRATEGIES_HPP

#include <random>
#include <vector>

void simple_strategy(uint32_t ** permutation, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen);
void level_strategy(uint32_t ** permutation, std::vector<bool> levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen);

#endif // SHUFFLINGSTRATEGIES_HPP