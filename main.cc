#include <iostream>
#include <thread>
#include <ctime>
#include <cstdlib>

#include "seq_tree.h"
#include "dynamic_locker.h"

using namespace std;

const int NUM_TEST = 10000;

const int MOD_FACTOR = 10000;

void time_it(int arg) {
  Sequential_Tree st;
  clock_t start;
  start = clock();
  for (int i = 0; i < arg; i++) {
    auto temp = rand() % MOD_FACTOR;
//    cout << "INSERT: " << temp << "";
    st.insert(temp, i*2);
  }
  cout << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
  st.print_all();
}

int main() {
  cout << "Size, Time\n";
  srand(0);
 /* int shift = 7;
  for (int i = 4; i < 1<<shift; i = i * 2) {
    time_it(i);
    cout << "\n~~~~~~~TRIAL~~~~~~~\n";
  }*/
  time_it(NUM_TEST);
  return 0;
}
