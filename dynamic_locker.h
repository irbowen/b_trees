#ifndef DYNAMIC_LOCKER_H
#define DYNAMIC_LOCKER_H

#include "read_write_lock.h"
#include <map>
#include <mutex>
#include <memory>

class DyanmicLocker {
    std::mutex m;
    std::map<int, std::shared_ptr<ReadWriteLock>> rw_lock_table;
    std::map<int, int> rw_lock_count;
    void createLock(int inode);
    void deleteLock(int inode);
public:
    void read_lock(int inode);
    void read_unlock(int inode);
    void write_lock(int inode);
    void write_unlock(int inode);
};

#endif