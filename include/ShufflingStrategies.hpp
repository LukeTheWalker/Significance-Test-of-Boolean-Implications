#ifndef SHUFFLINGSTRATEGIES_HPP
#define SHUFFLINGSTRATEGIES_HPP

#define SIMPLE_STRATEGY 0
#define EQUAL_LEVEL_STRATEGY 1
#define SHUFFLE_LEVEL_STRATEGY 2
#define FLIP_LEVEL_STRATEGY 3

#include <random>
#include <vector>

bool dispatch_strategy(uint32_t ** permutations_labels, uint32_t ** permutations_levels, std::vector<uint32_t>& labels, std::vector<bool>& levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen, uint32_t strategy);

#endif // SHUFFLINGSTRATEGIES_HPP