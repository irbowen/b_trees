#include <iostream>
#include <thread>
#include <ctime>
#include <cstdlib>

#include "link_tree.h"

using namespace std;

const int NUM_TEST = 1000000;
const int MOD_FACTOR = 10000;

Link_Tree lt;

void insert(int arg, int thread_num, int read_num) {
	ostringstream oss;
	int last_val;
  for (int i = 0; i < arg; i++) {
    auto temp = rand() % MOD_FACTOR;
    
    oss.str("");
    oss << "INSERTING VALUE " << i + 1 << " OF " << arg << endl;
    safe_cout(oss.str(), thread_num);
    
    if(i > 0 && i % read_num == 0){
    	lt.search(last_val);
    }
    else{
	    lt.insert(temp, temp + 1, thread_num);
	  }
    last_val = temp;
  }
}

void time_it(int arg) {
  clock_t start;
  start = clock();
  thread t1(insert, arg, 1, arg);
//  thread t2(insert, arg, 2, arg);
//  thread t3(insert, arg, 3, arg);
//  thread t4(insert, arg, 4, arg);
//  thread t5(insert, arg, 4, arg);
//  thread t6(insert, arg, 4, arg);
//  thread t7(insert, arg, 4, arg);
//  thread t8(insert, arg, 4, arg);
  t1.join();
//  t2.join();
//  t3.join();
//  t4.join();
//  t5.join();
//  t6.join();
//  t7.join();
//  t8.join();
  cout << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;
  lt.print_all();
}

int main() {
  cout << "Size, Time\n";
  srand(0);
  time_it(NUM_TEST);
  /*clock_t start;
  start = clock();
  insert(NUM_TEST, 1, NUM_TEST);
  cout << (clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << endl;*/
  cout << "Done\n";
 	//lt.root->print_link(1);
 	//cout << lt.search(9170) << endl;
	//cout << lt.search(9956) << endl;
  
  return 0;
}
