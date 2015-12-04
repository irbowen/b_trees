#include "dynamic_locker.h"

/*  If the objects hasn't been seen yet, or its current
    lock count is zero, then me need to make a new lock object.
    We always increment the count */
void Dyanmic_Locker::create_lock(int i) {
    std::unique_lock<std::mutex> lock(m);
    if (lock_count.count(i) == 0 || lock_count.at(i) == 0) {
        lock_table[i] = std::make_unique<Reader_Writer_Lock>();
        lock_count[i] = 0;
    }
    lock_count[i]++;
}

/*  Decrement the lock count.  
    If we hit zero, delete the lock object */
void Dyanmic_Locker::delete_lock(int i) {
    std::unique_lock<std::mutex> lock(m);
    lock_count[i]--;
    if (lock_count.at(i) == 0) {
        lock_table.erase(i);
    }
}

/* */
void Dyanmic_Locker::read_lock(int i) {
    create_lock(i);
    lock_table[i]->read_lock();
}

/* */
void Dyanmic_Locker::read_unlock(int i) {
    lock_table[i]->read_unlock();
    delete_lock(i);
}

/* */
void Dyanmic_Locker::write_lock(int i) {
    create_lock(i);
    lock_table[i]->write_lock();
}

/* */
void Dyanmic_Locker::write_unlock(int i) {
    lock_table[i]->write_unlock();
    delete_lock(i);
}
