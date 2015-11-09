#include "read_write_lock.h"

void ReadWriteLock::read_lock() {
    std::unique_lock<std::mutex> lock(m);
    while (write_flag) {
        cv.wait(lock);
    }
    read_count++;
}

void ReadWriteLock::read_unlock() {
    std::unique_lock<std::mutex> lock(m);
    read_count--;
    if (read_count == 0) {
        cv.notify_all();
    }
}

void ReadWriteLock::write_lock() {
    std::unique_lock<std::mutex> lock(m);
    while (write_flag || read_count > 0) {
        cv.wait(lock);
    }
    write_flag = true;
}

void ReadWriteLock::write_unlock() {
    std::unique_lock<std::mutex> lock(m);
    write_flag = false;
    cv.notify_all();
}