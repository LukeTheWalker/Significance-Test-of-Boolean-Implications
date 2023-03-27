#include <algorithm>
#include <iostream>

#include "ShufflingStrategies.hpp"

using namespace std;

// free shuffling strategy
// simple permutations of node, label and level permutate together
void free_shuffle_strategy(uint32_t ** permutations_labels, uint32_t ** permutations_levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) {
        shuffle(permutations_labels[i], permutations_labels[i] + n_nodes, gen);
        permutations_levels[i] = permutations_labels[i];
    }   
}

// label shuffling strategy
// every label can be assigned to any node

void shuffle_label_strategy(uint32_t ** permutations_labels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) 
        shuffle(permutations_labels[i], permutations_labels[i] + n_nodes, gen);
}

// same-level label shuffling strategy
// every label can be assigned to any node having the same level

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

// level shuffling strategy
// every level can be assigned to any node

void shuffle_level_strategy(uint32_t ** permutations_levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) 
        shuffle(permutations_levels[i], permutations_levels[i] + n_nodes, gen);
}

// flip level shuffling strategy
// every label appears twice in the permutation, with .5 probability the levels of the two labels are swapped

void flip_level_strategy(uint32_t ** permutations_levels, vector<uint32_t>& labels, vector<bool>& levels, uint32_t n_nodes, uint32_t n_expr, uint32_t n_samples, std::mt19937 &gen) {
    vector<uint32_t> labels_indexes[2] = {vector<uint32_t>(n_expr, -1), vector<uint32_t>(n_expr, -1)};
    // find the indexes of each label
    for (uint32_t i = 0; i < n_nodes; i++) labels_indexes[levels[i]][labels[i]] = i;
    // create the permutations
    for (uint32_t i = 0; i < n_samples; i++) {
        for (uint32_t j = 0; j < n_expr; j++) {
            if (labels_indexes[0][j] != -1)
            permutations_levels[i][labels_indexes[0][j]] = labels_indexes[0][j];
            if (labels_indexes[1][j] != -1)
            permutations_levels[i][labels_indexes[1][j]] = labels_indexes[1][j];
        }
        for (uint32_t j = 0; j < n_expr; j++) {
            if (labels_indexes[0][j] != -1 && labels_indexes[1][j] != -1 && gen() % 2) {
                permutations_levels[i][labels_indexes[0][j]] = labels_indexes[1][j];
                permutations_levels[i][labels_indexes[1][j]] = labels_indexes[0][j];
            }
        }
    }
}

// return true if the strategy is applied to the levels
void dispatch_strategy(uint32_t ** permutations_labels, uint32_t ** permutations_levels, vector<uint32_t>& labels, vector<bool>& levels, uint32_t n_nodes, uint32_t n_expr, uint32_t n_samples, std::mt19937 &gen, uint32_t strategy) {
    cerr << "strategy: " << strategy << endl;
    switch (strategy) {
        case FREE_STRATEGY:
            free_shuffle_strategy(permutations_labels, permutations_levels, n_nodes, n_samples, gen);
            break;

        case SHUFFLE_LABEL_STRATEGY:
            shuffle_label_strategy(permutations_labels, n_nodes, n_samples, gen);
            break;

        case EQUAL_LEVEL_STRATEGY:
            equal_level_strategy(permutations_labels, levels, n_nodes, n_samples, gen);
            break;

        case SHUFFLE_LEVEL_STRATEGY:
            shuffle_level_strategy(permutations_levels, n_nodes, n_samples, gen);
            break;

        case FLIP_LEVEL_STRATEGY:
            flip_level_strategy(permutations_levels, labels, levels, n_nodes, n_expr, n_samples, gen);
            break;

        default:
            cerr << "Error: invalid strategy" << endl;
            exit (EXIT_FAILURE);
    }
}

// return the number of strategies
int get_strategy_num (string strategy) {
    if (strategy == "free") return FREE_STRATEGY;
    if (strategy == "shuffle_label") return SHUFFLE_LABEL_STRATEGY;
    if (strategy == "equal") return EQUAL_LEVEL_STRATEGY;
    if (strategy == "shuffle_level") return SHUFFLE_LEVEL_STRATEGY;
    if (strategy == "flip") return FLIP_LEVEL_STRATEGY;
    else {
        cerr << "Error: invalid strategy" << endl;
        exit (EXIT_FAILURE);
    }
}