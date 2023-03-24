#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "util.hpp"

using namespace std;

// void cuda_err_check (cudaError_t err, const char *file, int line)
// {
//     if (err != cudaSuccess)
//     {
//         fprintf (stderr, "CUDA error: %s (%s:%d)\n", cudaGetErrorString (err), file, line);
//         exit (EXIT_FAILURE);
//     }
// }

// 0: gene1 low  => gene2 low
// 1: gene1 low  => gene2 high
// 2: gene1 high => gene2 low
// 3: gene1 high => gene2 high
// 4: equivalence (gene1 low <=> gene2 low)  && (gene1 high <=> gene2 high) => 0 && 3
// 5: opposite    (gene1 low <=> gene2 high) && (gene1 high <=> gene2 low)  => 1 && 2

void get_impl_from_string (string impl, bool * levels){
    if (impl == "low-low"){
        levels[0] = 0;
        levels[1] = 0;
    }
    else if (impl == "low-high"){
        levels[0] = 0;
        levels[1] = 1;
    }
    else if (impl == "high-low"){
        levels[0] = 1;
        levels[1] = 0;
    }
    else if (impl == "high-high"){
        levels[0] = 1;
        levels[1] = 1;
    }
    else{
        fprintf (stderr, "Error: unknown implementation %s\n", impl.c_str());
        exit (EXIT_FAILURE);
    }
}