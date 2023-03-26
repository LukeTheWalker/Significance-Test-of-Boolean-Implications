#include <algorithm>

#include "ShufflingStrategies.hpp"

using namespace std;

// first shuffling strategy
// every label can be assigned to any node
// constraints: number of high and low rests the same

void simple_strategy(uint32_t ** permutation, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) 
        shuffle(permutation[i], permutation[i] + n_nodes, gen);
}

// second shuffling strategy
// every label can be assigned to any node having the same level
// constraints: number of high and low rests the same and avoid creating new couples

void level_strategy(uint32_t ** permutation, vector<bool> levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
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
            if (levels[j]) {permutation [i][j] = high_nodes_copy.back(); high_nodes_copy.pop_back();}
            else {permutation [i][j] = low_nodes_copy.back(); low_nodes_copy.pop_back();}
        }
    }
}

// third shuffling strategy
// with a 0.5 probability every label can be assigned to another node having the same label
// constraints: number of high and low rests the same and avoid creating new couples

void flip_stategy(uint32_t ** permutation, vector<bool> levels, uint32_t n_nodes, uint32_t n_samples, std::mt19937 &gen) {
    for (uint32_t i = 0; i < n_samples; i++) {
        for (uint32_t j = 0; j < n_nodes; j++) {
            
        }
    }
}