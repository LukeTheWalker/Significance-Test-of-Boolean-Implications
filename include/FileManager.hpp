#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <vector>
#include <string>
#include <unordered_map>

struct PairHash {
    std::size_t operator()(const std::pair<uint32_t, bool>& p) const {
        return (size_t) p.first << 32 | (unsigned int) p.second;
    }
};
 
class FileManager{
    public:
        FileManager();
        FileManager(std::string filename);
        ~FileManager();
        void readFile(std::string file);
        int getNumberOfRows() const;
        int getNumberOfColumns() const;
        uint32_t (* edges) [2];
        std::unordered_map<std::string, uint32_t> expr_map;
        std::vector<std::string> exprs;
        std::vector<uint32_t> labels;
        std::vector<bool> levels;
        std::unordered_map<std::pair<uint32_t, bool>, uint32_t, PairHash> node_map;
        std::vector<std::vector<uint32_t> > graph;
    private:
        int n_rows = -1;
        int n_columns = -1;
        int getNumberOfColumns(std::string);
        int getNumberOfRows(std::string);
};

#endif
