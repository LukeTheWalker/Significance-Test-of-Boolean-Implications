#ifndef SHUFFLINGSTRATEGIES_HPP
#define SHUFFLINGSTRATEGIES_HPP

#define SIMPLE_STRATEGY 0
#define LEVEL_STRATEGY 1
#define FLIP_STRATEGY 2

#include <random>
#include <vector>

void simple_strategy(uint32_t ** permutation, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen);
void level_strategy(uint32_t ** permutations_labels, std::vector<bool>& levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen);
bool dispatch_strategy(uint32_t ** permutations_labels, uint32_t ** permutations_levels, vector<uint32_t>& labels, vector<bool>& levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen, uint32_t strategy);

#endif // SHUFFLINGSTRATEGIES_HPP