#include "reader_writer_lock.h"

/*  Whiel there are writers, we can't read.
    So go to sleep - upon wake, increment reader count */
void Reader_Writer_Lock::read_lock() {
  std::unique_lock<std::mutex> lock(m);
  while (write_flag) {
    cv.wait(lock);
  }
  read_count++;
}

/*  Decrement the read counter -
    If we're at zero, we lead any waiting writer's know */
void Reader_Writer_Lock::read_unlock() {
  std::unique_lock<std::mutex> lock(m);
  read_count--;
  if (read_count == 0) {
      cv.notify_all();
  }
}

/*  If there are readers, or a single writer, go to sleep
    Afterwards, set write flag */
void Reader_Writer_Lock::write_lock() {
  std::unique_lock<std::mutex> lock(m);
  while (write_flag || read_count > 0) {
    cv.wait(lock);
  }
  write_flag = true;
}

/*  Remove writer flag, and let any waiting readers know */
void Reader_Writer_Lock::write_unlock() {
  std::unique_lock<std::mutex> lock(m);
  write_flag = false;
  cv.notify_all();
}
