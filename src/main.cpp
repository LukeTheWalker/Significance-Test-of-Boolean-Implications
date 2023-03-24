#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <chrono>
#include <omp.h>


#include "FileManager.hpp"
#include "InputParser.hpp"

using namespace std;

void parse_arguments(int argc, char * argv[], string & implication_file, string& significance_file, uint32_t & n_samples, double & p_value_threshold) {
    InputParser pars(argc, argv);

    if (pars.cmdOptionExists("-h") || !pars.cmdOptionExists("-i")) {cerr << "Usage: " << argv[0] << " -i <expression_file> -s <statistic_threshold> -p <p-value_threshold> -o <implication_file>" << endl; exit(0);}
    if (pars.cmdOptionExists("-i")) implication_file = pars.getCmdOption("-i");  else cerr << "Warning: no expression file specified, using default: " << implication_file << endl;
    if (pars.cmdOptionExists("-ns")) n_samples = stoi(pars.getCmdOption("-ns"));  else cerr << "Warning: no number of samples specified, using default: " << n_samples << endl;
    if (pars.cmdOptionExists("-p")) p_value_threshold = stod(pars.getCmdOption("-p"));  else cerr << "Warning: no p-value threshold specified, using default: " << p_value_threshold << endl;
    if (pars.cmdOptionExists("-o")) significance_file = pars.getCmdOption("-o");  else cerr << "Warning: no significance file specified, using default: " << significance_file << endl;
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
    parse_arguments(argc, argv, implication_file, significance_file, n_samples, p_value_threshold);

    FileManager fm;
    fm.readFile(implication_file);

    unordered_map<pair<uint32_t, bool>, uint32_t, PairHash> &node_map = fm.node_map;
    uint32_t n_nodes = node_map.size();
    cerr << "Number of nodes: " << n_nodes << endl;

    unordered_map<string, uint32_t> &expr_map     = fm.expr_map;
    uint32_t n_expr = expr_map.size();

    cerr << "Number of expressions: " << n_expr << endl;

    vector<vector<uint32_t> >&graph = fm.graph;

    uint32_t n_rows = fm.getNumberOfRows();

    int * permutations[n_samples];

    for (int i = 0; i < n_samples; i++) {
        permutations[i] = new int[n_nodes];
    }

    for (int i = 0; i < n_samples; i++)
        for (int j = 0; j < n_nodes; j++) 
            permutations[i][j] = j;

    cerr << "Samples Initialized" << endl;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    unsigned seed = 42;
    // std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen(rd()); 

    for (int i = 0; i < n_samples; i++){
        shuffle(permutations[i], permutations[i] + n_nodes, gen);
        // permutations[i][0] = 2;
        // permutations[i][1] = 0;
        // permutations[i][2] = 1;
        // permutations[i][3] = 3;        
    }

    cerr << "Samples Generated" << endl;

    double * p_vals  = new double[n_rows];
    for (int i = 0; i < n_rows; i++) p_vals[i] = 0.0;

    int n_threads = omp_get_max_threads();

    cerr << "Maximal number of threads: " << n_threads << endl;

    omp_set_num_threads(n_threads);

    unordered_map<uint32_t, uint32_t> * rev_perm;

    rev_perm = new unordered_map<uint32_t, uint32_t>[n_threads];

    #pragma omp parallel for
    for (int j = 0; j < n_samples; j++) {
        int thread_id = omp_get_thread_num();
        for (int k = 0; k < n_nodes; k++) rev_perm[thread_id][permutations[j][k]] = k;
        for (int i = 0; i < n_rows; i++) {
            uint32_t from = fm.edges[i][0];
            uint32_t to = fm.edges[i][1];
            uint32_t from_perm = rev_perm[thread_id][from];
            for (auto it = graph[from_perm].begin(); it != graph[from_perm].end(); it++) {
                if (rev_perm[thread_id][to] == *it) {
                    p_vals[i] += 1.0 / n_samples;
                    break;
                }
            }
        }
        rev_perm[thread_id].clear();
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
        delete[] permutations[i];    

    delete[] p_vals;

    return 0;
}