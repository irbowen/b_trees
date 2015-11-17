#include <iostream>
#include <thread>
#include <ctime>

#include "seq_tree.h"
#include "dynamic_locker.h"

using namespace std;

const int NUM_TEST = 100;

void time_it(int arg) {
  Sequential_Tree st;
  clock_t start;
  start = clock();
  for (int i = 0; i < arg; i++) {
    st.insert(i, i*2);
  }
  cout << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
}

int main() {
  cout << "Size, Time\n";
  for (int i = 4; i < 1<<26; i = i * 2) {
    time_it(i);
  }
  return 0;
}
