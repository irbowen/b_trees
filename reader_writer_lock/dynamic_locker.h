#ifndef DYNAMIC_LOCKER_H
#define DYNAMIC_LOCKER_H

#include <map>
#include <mutex>
#include <memory>

#include "reader_writer_lock.h"

/*  Creates locks objects as they are needed, and deletes them
    when there are no current readers or writers */
class Dyanmic_Locker {
  /*  */
  std::mutex m;
  std::map<int, std::unique_ptr<Reader_Writer_Lock>> lock_table;
  std::map<int, int> lock_count;
  /* */
  void create_lock(int);
  void delete_lock(int);
public:
  /* */
  void read_lock(int);
  void read_unlock(int);
  void write_lock(int);
  void write_unlock(int);
};

#endif
