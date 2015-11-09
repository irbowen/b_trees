#include "dynamic_locker.h"

void Dyanmic_Locker::create_lock(int inode) {
    std::unique_lock<std::mutex> lock(m);
    if (lock_count.count(inode) == 0 || lock_count.at(inode) == 0) {
        lock_table[inode] = std::make_shared<Reader_Writer_Lock>();
        lock_count[inode] = 0;
    }
    lock_count[inode]++;
}

void Dyanmic_Locker::delete_lock(int inode) {
    std::unique_lock<std::mutex> lock(m);
    lock_count[inode]--;
    if (lock_count.at(inode) == 0) {
        lock_table.erase(inode);
    }
}

void Dyanmic_Locker::read_lock(int inode) {
    create_lock(inode);
    lock_table[inode]->read_lock();
}

void Dyanmic_Locker::read_unlock(int inode) {
    lock_table[inode]->read_unlock();
    delete_lock(inode);
}

void Dyanmic_Locker::write_lock(int inode) {
    create_lock(inode);
    lock_table[inode]->write_lock();
}

void Dyanmic_Locker::write_unlock(int inode) {
    lock_table[inode]->write_unlock();
    delete_lock(inode);
}