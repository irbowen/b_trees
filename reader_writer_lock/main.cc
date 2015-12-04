#include <iostream>
#include <thread>
#include <ctime>
#include <cstdlib>

#include "tree.h"
#include "reader_writer_lock.h"

using namespace std;

const int NUM_TEST = 100000;

const int MOD_FACTOR = 1000000;

Tree st;

void insert(int arg) {
  for (int i = 0; i < arg; i++) {
    auto temp = rand() % MOD_FACTOR;
    st.insert2(temp, i*2);
  }
}

void time_it(int arg) {
  clock_t start;
  start = clock();
  thread t1(insert, arg);
  thread t2(insert, arg);
  thread t3(insert, arg);
  thread t4(insert, arg);
  thread t5(insert, arg);
  thread t6(insert, arg);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  auto time_taken = clock() - start; 
  st.print_all();
  cout << "Time: " << time_taken / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
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
