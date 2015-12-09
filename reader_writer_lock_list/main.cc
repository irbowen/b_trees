#include <iostream> 
#include <thread>
#include <ctime>
#include <cstdlib>

#include "tree.h"

using namespace std;

const int NUM_TEST = 100000;

const int MOD_FACTOR = 1000000;

Tree st;

void insert(int num_test, int rand_mod_factor, int read_percent) {
  for (int i = 0; i < num_test; i++) {
    int temp = rand() % rand_mod_factor;
    if (rand() % 100 > read_percent) {
      st.insert(temp, i*2);
    }
    else {
      st.get_value(temp);
      //auto value = st.get_value(temp);
      //cout << "Value: " << value << endl;
    }
  }
}

void time_it(int num_threads, int num_test, int rand_mod_factor, int read_percent) {
  clock_t start;
  start = clock();
  
  vector<unique_ptr<thread>> thread_vector;
  for (auto i = 0; i < num_threads; i++) {
    thread_vector.push_back(
      unique_ptr<thread>(new thread(insert, num_test, rand_mod_factor, read_percent)));
  }
  for (auto& t : thread_vector) {
    t->join();
  }
   
  auto time_taken = clock() - start; 
  //st.print_all();
  cout << "Time: " << time_taken / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
}

/*  Num Threads
    Num inputs per thread
    Rand Factor
    Read Percentage  
*/
int main() {
  cout << "Size, Time\n";
  srand(0);
  int num_threads = 0, num_test = 0, rand_mod_factor = 0, read_percent = 0;
  cin >> num_threads;
  cin >> num_test;
  cin >> rand_mod_factor;
  cin >> read_percent;
  time_it(num_threads, num_test, rand_mod_factor, read_percent);
  return 0;
}
