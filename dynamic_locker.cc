#include "dynamic_locker.h"

void DyanmicLocker::createLock(int inode) {
    std::unique_lock<std::mutex> lock(m);
    if (rw_lock_count.count(inode) == 0 || rw_lock_count.at(inode) == 0) {
        rw_lock_table[inode] = std::make_shared<ReadWriteLock>();
        rw_lock_count[inode] = 0;
    }
    rw_lock_count[inode]++;
}

void DyanmicLocker::deleteLock(int inode) {
    std::unique_lock<std::mutex> lock(m);
    rw_lock_count[inode]--;
    if (rw_lock_count.at(inode) == 0) {
        rw_lock_table.erase(inode);
    }
}

void DyanmicLocker::read_lock(int inode) {
    createLock(inode);
    rw_lock_table[inode]->read_lock();
}

void DyanmicLocker::read_unlock(int inode) {
    rw_lock_table[inode]->read_unlock();
    deleteLock(inode);
}

void DyanmicLocker::write_lock(int inode) {
    createLock(inode);
    rw_lock_table[inode]->write_lock();
}

void DyanmicLocker::write_unlock(int inode) {
    rw_lock_table[inode]->write_unlock();
    deleteLock(inode);
}