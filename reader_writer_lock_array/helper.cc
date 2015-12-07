#include "helper.h"

using namespace std;

mutex cout_mutex;

void safe_cout(string str) {
    unique_lock<mutex> lock(cout_mutex);
    cout << str << endl;
}
