#include <iostream>
#include <thread>

#include "seq_tree.h"
#include "dynamic_locker.h"

using namespace std;

int main() {
  cout << "Hi\n";

  DyanmicLocker dynamic_lock_manager;

  dynamic_lock_manager.read_lock(1);
  cout << "Currently locked\n";
  dynamic_lock_manager.read_unlock(1);
  cout << "Unlocked\n";
  return 0;
}