#include <algorithm>
#include <iostream>

#include "ShufflingStrategies.hpp"

using namespace std;

// first shuffling strategy
// every label can be assigned to any node
// constraints: number of high and low rests the same

void simple_strategy(uint32_t ** permutations_labels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) 
        shuffle(permutations_labels[i], permutations_labels[i] + n_nodes, gen);
}

// second shuffling strategy
// every label can be assigned to any node having the same level
// constraints: number of high and low rests the same and avoid creating new couples

void equal_level_strategy(uint32_t ** permutations_labels, vector<bool>& levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    vector<uint32_t> high_nodes;
    vector<uint32_t> low_nodes;
    for (uint32_t i = 0; i < n_nodes; i++) {
        if (levels[i]) high_nodes.push_back(i);
        else low_nodes.push_back(i);
    }
    for (uint32_t i = 0; i < n_samples; i++) {
        vector<uint32_t> high_nodes_copy = high_nodes;
        vector<uint32_t> low_nodes_copy = low_nodes;
        shuffle(high_nodes_copy.begin(), high_nodes_copy.end(), gen);
        shuffle(low_nodes_copy.begin(), low_nodes_copy.end(), gen);
        for (uint32_t j = 0; j < n_nodes; j++) {
            if (levels[j]) {permutations_labels [i][j] = high_nodes_copy.back(); high_nodes_copy.pop_back();}
            else {permutations_labels [i][j] = low_nodes_copy.back(); low_nodes_copy.pop_back();}
        }
    }
}

// third shuffling strategy
// shuffle level strategy
// constraints: number of high and low rests the same and avoid creating new couples

void shuffle_level_strategy(uint32_t ** permutations_levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) 
        shuffle(permutations_levels[i], permutations_levels[i] + n_nodes, gen);
}

// fourth shuffling strategy
// every label appears twice in the permutation, with .5 probability the levels of the tow labels are swapped
// constraints: number of high and low rests the same and avoid creating new couples
void flip_level_strategy(uint32_t ** permutations_labels, vector<bool>& levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) {
        for (uint32_t j = 0; j < n_nodes; j++) {
            if (gen() % 2) {
                if (levels[j]) levels[j] = false;
                else levels[j] = true;
            }
        }
    }
}

// return true if the strategy is applied to the levels
bool dispatch_strategy(uint32_t ** permutations_labels, uint32_t ** permutations_levels, vector<uint32_t>& labels, vector<bool>& levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen, uint32_t strategy) {
    switch (strategy) {
        case SIMPLE_STRATEGY:
            simple_strategy(permutations_labels, n_nodes, n_samples, gen);
            return false;
        case EQUAL_LEVEL_STRATEGY:
            equal_level_strategy(permutations_labels, levels, n_nodes, n_samples, gen);
            return false;
        case SHUFFLE_LEVEL_STRATEGY:
            shuffle_level_strategy(permutations_levels, n_nodes, n_samples, gen);
            return true;
        case FLIP_LEVEL_STRATEGY:
            flip_level_strategy(permutations_labels, levels, n_nodes, n_samples, gen);
            return true;
        default:
            cerr << "Error: invalid strategy" << endl;
            exit (EXIT_FAILURE);
    }
}

// return the number of strategies
int get_strategy_num (string strategy) {
    if (strategy == "simple") return SIMPLE_STRATEGY;
    else if (strategy == "equal") return EQUAL_LEVEL_STRATEGY;
    else if (strategy == "shuffle") return SHUFFLE_LEVEL_STRATEGY;
    else if (strategy == "flip") return FLIP_LEVEL_STRATEGY;
    else {
        cerr << "Error: invalid strategy" << endl;
        exit (EXIT_FAILURE);
    }
}