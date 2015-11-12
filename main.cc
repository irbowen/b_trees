#include <iostream>
#include <thread>

#include "seq_tree.h"
#include "dynamic_locker.h"

using namespace std;

const int NUM_TEST = 50;

int main() {
  cout << "Hi\n";

  Dyanmic_Locker dynamic_lock_manager;
  dynamic_lock_manager.read_lock(1);
  dynamic_lock_manager.read_unlock(1);
  
  Sequential_Tree st;
  for (int i = 0; i < NUM_TEST - 3; i++) {
    cout << "CALLING INSERT key: " << i << "\n";
    st.insert(i, i*2);
  }
  for (int i = 0; i < NUM_TEST - 3; i++) {
    cout << "CALLING INSERT key: " << NUM_TEST - i +1 << "\n";
    st.insert(NUM_TEST - i + 1, i + 3);
  }
  st.print_all();
  std::cout << std::endl;
  return 0;
}
