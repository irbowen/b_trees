#include <iostream>
#include <thread>

#include "seq_tree.h"
#include "dynamic_locker.h"

using namespace std;

const int NUM_TEST = 10;

int main() {
  cout << "Hi\n";

  Dyanmic_Locker dynamic_lock_manager;
  dynamic_lock_manager.read_lock(1);
  dynamic_lock_manager.read_unlock(1);
  
  Sequential_Tree st;
  for (int i = 0; i < NUM_TEST; i++) {
    cout << "\nCALLING INSERT key: " << i << "\n";
    st.insert(i, i*2);
  }
  st.print_all();
  std::cout << std::endl;
  return 0;
}
