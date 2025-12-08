#pragma once
#include <atomic>
class LockWrapper {
    friend class CVWrapper;

    pthread_mutex_t mutex;

public:
    LockWrapper() {
        auto rc = pthread_mutex_init(&mutex, nullptr);
        if (rc != 0) {
            throw_errno("Failed to create mutex");
        }
    }

    ~LockWrapper() {
        pthread_mutex_destroy(&mutex);
    }

    void lock() {
        auto rc = pthread_mutex_lock(&mutex);
        if (rc != 0) {
            throw_errno("Failed to lock mutex");
        }
    }

    void unlock() {
        auto rc = pthread_mutex_unlock(&mutex);
        if (rc != 0) {
            throw_errno("Failed to unlock mutex");
        }
    }
};

class CVWrapper {
    pthread_cond_t cv;

public:
    CVWrapper() {
        auto rc = pthread_cond_init(&cv, nullptr);
        if (rc != 0) {
            throw_errno("Failed to create condvar");
        }
    }

    ~CVWrapper() {
        pthread_cond_destroy(&cv);
    }

    void wait(LockWrapper& lock) {
        auto rc = pthread_cond_wait(&cv, &lock.mutex);
        if (rc != 0) {
            throw_errno("Failed to wait cv");
        }
    }

    void broadcast() {
        auto rc = pthread_cond_broadcast(&cv);
        if (rc != 0) {
            throw_errno("Failed to broadcast cv");
        }
    }
};

class AtomicInt {
private:
    std::atomic<int> value;

public:
    AtomicInt(int v = 0): value(v) {}
    void change(int delta) {
        value.fetch_add(delta);
    }

    int get() {
        return value.load();
    }
};

