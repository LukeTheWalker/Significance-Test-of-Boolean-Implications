#ifndef UTILITY_H
#define UTILITY_H

#include <string>

// void cuda_err_check(cudaError_t err, const char *file, int line);
void get_impl_from_string (std::string impl, bool * levels);


#endif