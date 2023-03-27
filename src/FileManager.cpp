#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "FileManager.hpp"
#include "util.hpp"

using namespace std;

FileManager::FileManager() {
    std::cerr << "FileManager created" << std::endl;
}

FileManager::FileManager(string filename){
    FileManager();
    readFile(filename);
}

FileManager::~FileManager(){
    // cudaError_t err = cudaFreeHost(linear_graph);
    // cuda_err_check(err, __FILE__, __LINE__);

    free(edges);
    std::cerr << "FileManager destroyed" << std::endl;
}

int FileManager::getNumberOfColumns(string file){
    ifstream in(file);
    string line;
    string delimiter = "\t";

    getline(in, line);

    size_t pos = 0;
    string token;
    int i = 0;
    while ((pos = line.find(delimiter)) != string::npos) {
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        i++;
    }
    in.close();
    return i + 1; // accounting for last column which does not have a delimiter at the end of the line
}

int FileManager::getNumberOfRows(string file){
    ifstream in(file);
    string line;
    int i = 0;
    while(getline(in, line)){
        i++;
    }
    in.close();
    return i;
}

void FileManager::readFile(string file){
    string line;
    string delimiter = "\t";

    ifstream in(file);
    if (!in.is_open()){
        cerr << "Could not open file " << file << endl;
        exit(1);
    }
    in.close();

    n_columns = getNumberOfColumns(file);
    n_rows = getNumberOfRows(file) - 1; // accounting for header

    edges = static_cast<uint32_t (*)[2]>(std::malloc(n_rows * sizeof(*edges)));

    in.open(file);

    cerr << "Reading file with " << n_rows << " rows and " << n_columns << " columns" << endl;

    getline(in, line); // get rid of headers

    string expr1, expr2;
    string impl;
    float stat, pval;
    bool lvls[2];
    for (int i = 0; i < n_rows; i++){
        in >> expr1 >> expr2 >> impl >> stat >> pval;
        if (expr_map.find(expr1) == expr_map.end()){
            expr_map[expr1] = expr_map.size();
            exprs.push_back(expr1);
        }
        if (expr_map.find(expr2) == expr_map.end()){
            expr_map[expr2] = expr_map.size();
            exprs.push_back(expr2);
        }

        get_impl_from_string(impl, lvls);

        if (node_map.find({expr_map[expr1], lvls[0]}) == node_map.end()){
            node_map[{expr_map[expr1], lvls[0]}] = node_map.size();
            labels.push_back(expr_map[expr1]);
            levels.push_back(lvls[0]);
            graph.push_back({});
        }
        if (node_map.find({expr_map[expr2], lvls[1]}) == node_map.end()){
            node_map[{expr_map[expr2], lvls[1]}] = node_map.size();
            labels.push_back(expr_map[expr2]);
            levels.push_back(lvls[1]);
            graph.push_back({});
        }

        graph[node_map[{expr_map[expr1], lvls[0]}]].push_back(node_map[{expr_map[expr2], lvls[1]}]);
        edges[i][0] = node_map[{expr_map[expr1], lvls[0]}];
        edges[i][1] = node_map[{expr_map[expr2], lvls[1]}];
    }

    in.close();
}

int FileManager::getNumberOfRows() const{
    return n_rows;
}

int FileManager::getNumberOfColumns() const{
    return n_columns;
}