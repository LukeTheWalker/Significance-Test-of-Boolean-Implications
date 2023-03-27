#ifndef SHUFFLINGSTRATEGIES_HPP
#define SHUFFLINGSTRATEGIES_HPP

#define FREE_STRATEGY 0
#define SHUFFLE_LABEL_STRATEGY 1
#define EQUAL_LEVEL_STRATEGY 2
#define SHUFFLE_LEVEL_STRATEGY 3
#define FLIP_LEVEL_STRATEGY 4

#include <random>
#include <vector>

void dispatch_strategy(uint32_t ** permutations_labels, uint32_t ** permutations_levels, std::vector<uint32_t>& labels, std::vector<bool>& levels, uint32_t n_nodes, uint32_t n_expr, uint32_t n_samples, std::mt19937 &gen, uint32_t strategy);
int get_strategy_num (std::string strategy);

#endif // SHUFFLINGSTRATEGIES_HPP