#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <iostream>
#include <mutex>

extern std::mutex cout_mutex;

void safe_cout(std::string);

#endif
