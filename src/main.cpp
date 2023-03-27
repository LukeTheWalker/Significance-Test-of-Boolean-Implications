#define TEST 0

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <chrono>
#include <omp.h>
#include <string.h>


#include "FileManager.hpp"
#include "InputParser.hpp"
#include "ShufflingStrategies.hpp"

using namespace std;

void parse_arguments(int argc, char * argv[], string & implication_file, string& significance_file, uint32_t & n_samples, double & p_value_threshold, int& strategy_n){
    InputParser pars(argc, argv);
    string strategy = "free";

    if (pars.cmdOptionExists("-h") || !pars.cmdOptionExists("-i")) {cerr << "Usage: " << argv[0] << " -i <expression_file> -s <statistic_threshold> -p <p-value_threshold> -o <implication_file>" << endl; exit(0);}
    if (pars.cmdOptionExists("-i")) implication_file = pars.getCmdOption("-i");  else cerr << "Warning: no expression file specified, using default: " << implication_file << endl;
    if (pars.cmdOptionExists("-ns")) n_samples = stoi(pars.getCmdOption("-ns"));  else cerr << "Warning: no number of samples specified, using default: " << n_samples << endl;
    if (pars.cmdOptionExists("-p")) p_value_threshold = stod(pars.getCmdOption("-p"));  else cerr << "Warning: no p-value threshold specified, using default: " << p_value_threshold << endl;
    if (pars.cmdOptionExists("-o")) significance_file = pars.getCmdOption("-o");  else cerr << "Warning: no significance file specified, using default: " << significance_file << endl;
    if (pars.cmdOptionExists("-s")) strategy = pars.getCmdOption("-s");  else cerr << "Warning: no shuffling strategy specified, using default: " << strategy << endl;    
    cerr << "Strategy: " << strategy << endl;
    strategy_n = get_strategy_num(strategy);
}

int main (int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << "-i <expr_file>" << endl;
        return 1;
    }

    string implication_file = "input/implications_big_luca.txt";
    string significance_file = "out/significance.txt";
    uint32_t n_samples = 1000;
    double p_value_threshold = 0.05;
    int strategy = FREE_STRATEGY;
    parse_arguments(argc, argv, implication_file, significance_file, n_samples, p_value_threshold, strategy);

    FileManager fm;
    fm.readFile(implication_file);

    unordered_map<pair<uint32_t, bool>, uint32_t, PairHash> &node_map = fm.node_map;
    uint32_t n_nodes = node_map.size();
    cerr << "Number of nodes: " << n_nodes << endl;

    unordered_map<string, uint32_t> &expr_map = fm.expr_map;
    uint32_t n_expr = expr_map.size();

    cerr << "Number of expressions: " << n_expr << endl;

    vector<vector<uint32_t> >&graph = fm.graph;

    uint32_t n_rows = fm.getNumberOfRows();

    uint32_t ** permutations_labels, ** permutations_levels;

    permutations_labels = new uint32_t * [n_samples];
    permutations_levels = new uint32_t * [n_samples];

    for (int i = 0; i < n_samples; i++) { 
        permutations_labels[i] = new uint32_t[n_nodes]; 
        permutations_levels[i] = new uint32_t[n_nodes];
    }
    

    for (int i = 0; i < n_samples; i++)
        for (int j = 0; j < n_nodes; j++) {
            permutations_labels[i][j] = j;
            permutations_levels[i][j] = j;
        }

    cerr << "Samples Initialized" << endl;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    unsigned seed = 42;
    std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
    // std::mt19937 gen(rd()); 

#if not TEST
    dispatch_strategy(permutations_labels, permutations_levels, fm.labels, fm.levels, n_nodes, n_expr, n_samples, gen, strategy);
    int n_threads = omp_get_max_threads();
#else
    for (int i = 0; i < n_samples; i++){
        permutations[i][0] = 2;
        permutations[i][1] = 1;
        permutations[i][2] = 3;
        permutations[i][3] = 0; 
    }
    int n_threads = 1;
#endif

    cerr << "Samples Generated" << endl;

    double * p_vals  = new double[n_rows];
    for (int i = 0; i < n_rows; i++) p_vals[i] = 0.0;

    cerr << "Maximal number of threads: " << n_threads << endl;

    omp_set_num_threads(n_threads);

    #pragma omp parallel for
    for (int j = 0; j < n_samples; j++) {
        uint32_t * rev_perm = new uint32_t[n_expr * 2];
        memset(rev_perm, -1, n_expr * sizeof(uint32_t) * 2);
        for (int k = 0; k < n_nodes; k++) {
            bool is_high = fm.levels[permutations_labels[j][k]];
            uint32_t label = fm.labels[permutations_labels[j][k]];
            rev_perm[label + (n_expr * is_high) ] = k;
        }
        #if TEST
        // print rev_perm
        for (int i = 0; i < n_expr; i++) {
            cerr << "Expr " << fm.exprs[i] << ": ";
            for (int high = 0; high < 2; high++){
                if (rev_perm[i + (n_expr * high)] != -1) {
                    cerr << rev_perm[i + (n_expr * high)] << " ";
                }
            }
            cerr << endl;
        }
        #endif
        for (int i = 0; i < n_rows; i++) {
            uint32_t node_from_og = fm.edges[i][0];
            uint32_t node_to_og   = fm.edges[i][1];
            uint32_t label_from_og = fm.labels[node_from_og];
            uint32_t level_from_og = fm.levels[node_from_og];
            for (int high = 0; high < 2; high++){
                uint32_t node_from_permutated = rev_perm[label_from_og + (n_expr * high)];
                if (node_from_permutated == -1 || fm.levels[permutations_levels[j][node_from_permutated]] != level_from_og) continue;
                for (auto it = graph[node_from_permutated].begin(); it != graph[node_from_permutated].end(); it++) {
                    uint32_t label_to_og = fm.labels[node_to_og];
                    uint32_t level_to_og = fm.levels[node_to_og];
                    uint32_t node_to_permutated = permutations_labels[j][*it];
                    uint32_t label_to_permutated = fm.labels[node_to_permutated];
                    uint32_t level_to_permutated = fm.levels[permutations_levels[j][*it]];
                    if (label_to_permutated == label_to_og &&
                        level_to_permutated == level_to_og) {
                        #pragma omp atomic
                        p_vals[i] += 1.0 / n_samples;
                        break;
                    }
                }
            }
        }
        delete[] rev_perm;
    }

    int cnt = 0;
    ofstream out(significance_file);
    out << "FromLabel\tFromLevel\tToLabel\tToLevel\tP-value" << endl;
    for (int i = 0; i < n_rows; i++) {
        if (p_vals[i] <= p_value_threshold) {
            out << fm.exprs[fm.labels[fm.edges[i][0]]] << "\t" << fm.levels[fm.edges[i][0]] << "\t" << fm.exprs[fm.labels[fm.edges[i][1]]] << "\t" << fm.levels[fm.edges[i][1]] << "\t" << p_vals[i] << endl;
            cnt++;
        }
    }

    cerr << "Number of significant edges: " << cnt << " out of " << n_rows << " percent: " << (double) cnt / n_rows << endl;

    for (int i = 0; i < n_samples; i++) 
        delete[] permutations_labels[i];    

    delete[] permutations_labels;
    delete[] p_vals;

    return 0;
}