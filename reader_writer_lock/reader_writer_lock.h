#ifndef READ_WRITE_LOCK_H
#define READ_WRITE_LOCK_H

#include <mutex>
#include <condition_variable>
#include <thread>

/*  Textbook mutex/cv implmentation,
    using the C++11 features */
class Reader_Writer_Lock {
  /* */
  std::mutex m;
  std::condition_variable cv;
  int read_count{0};
  bool write_flag{false};
public:
  /* */
  void read_lock();
  void read_unlock();
  void write_lock();
  void write_unlock();
  void upgrade_lock();
};

#endif
